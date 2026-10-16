#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

#define SHELL_PROMPT "rahul-shell> "

/* Parse the input line into command arguments and detect output redirection. */
int parse_input(char *input, char *args[], char **output_file,
                bool *redirect_output, bool *append_output);

/* Execute built-in commands: exit, cd, pwd.
   Returns 1 if the command was built-in, otherwise 0. */
int execute_builtin(char *args[]);

/* Execute an external command using fork(), execvp(), and waitpid(). */
void execute_command(char *args[], char *output_file,
                     bool redirect_output, bool append_output);

#endif