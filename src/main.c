#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int parse_input(char *input, char *args[], char **output_file,
                bool *redirect_output, bool *append_output) {

    int arg_count = 0;
    char *token;

    *output_file = NULL;
    *redirect_output = false;
    *append_output = false;

    token = strtok(input, " ");

    while (token != NULL) {

        if (strcmp(token, ">") == 0) {
            *redirect_output = true;

            token = strtok(NULL, " ");

            if (token == NULL) {
                fprintf(stderr, "shell: syntax error: expected file after '>\n'");
                return -1;
            }

            *output_file = token;
            break;
        }

        if (strcmp(token, ">>") == 0) {
            *append_output = true;

            token = strtok(NULL, " ");

            if (token == NULL) {
                fprintf(stderr, "shell: syntax error: expected file after '>>'\n");
                return -1;
            }

            *output_file = token;
            break;
        }

        if (arg_count >= MAX_ARGS - 1) {
            fprintf(stderr, "shell: too many arguments\n");
            return -1;
        }

        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    args[arg_count] = NULL;
    return arg_count;
}

int execute_builtin(char *args[]) {

    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return -1;
    }

    if (strcmp(args[0], "cd") == 0) {

        if (args[1] == NULL) {
            fprintf(stderr, "cd: missing argument\n");
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }

        return 1;
    }

    if (strcmp(args[0], "pwd") == 0) {

        char cwd[MAX_INPUT_SIZE];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd");
        }

        return 1;
    }

    return 0;
}

void execute_command(char *args[], char *output_file,
                     bool redirect_output, bool append_output) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        /* Child process */

        if (redirect_output || append_output) {
            int fd;

            if (redirect_output) {
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }

            if (fd < 0) {
                perror("open");
                _exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd);
                _exit(EXIT_FAILURE);
            }

            close(fd);
        }

        if (execvp(args[0], args) < 0) {
            perror("execvp");
            _exit(EXIT_FAILURE);
        }
    }

    /* Parent process waits for the child */
    waitpid(pid, NULL, 0);
}

int main(void) {

    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *output_file;
    bool redirect_output;
    bool append_output;

    while (1) {

        printf("%s", SHELL_PROMPT);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }

        /* Remove trailing newline */
        input[strcspn(input, "\n")] = '\0';

        /* Ignore empty input */
        if (input[0] == '\0') {
            continue;
        }

        int arg_count = parse_input(
            input,
            args,
            &output_file,
            &redirect_output,
            &append_output
        );

        if (arg_count < 0) {
            continue;
        }

        if (args[0] == NULL) {
            continue;
        }

        int builtin_status = execute_builtin(args);

        if (builtin_status == -1) {
            break;
        }

        if (builtin_status == 1) {
            continue;
        }

        execute_command(
            args,
            output_file,
            redirect_output,
            append_output
        );
    }

    return 0;
}