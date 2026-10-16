# Mini Linux Shell in C

A simple Unix-like command-line shell implemented in C. It reads user commands, parses them, executes external programs using system calls, supports built-in commands, and handles output redirection.

This project demonstrates core Linux system-programming concepts such as process creation, program execution, process waiting, file descriptors, and file redirection.

## Features

- Interactive shell prompt
- Command parsing using whitespace tokenization
- External command execution using `fork()` and `execvp()`
- Parent process waits for child completion using `waitpid()`
- Built-in commands:
  - `cd`
  - `pwd`
  - `exit`
- Output redirection:
  - `>` overwrites a file
  - `>>` appends to a file
- File descriptor handling using `open()` and `dup2()`
- Basic error handling for invalid commands and failed system calls

## Supported Commands

### Built-in Commands

| Command | Description |
|---|---|
| `cd <directory>` | Changes the current working directory |
| `pwd` | Prints the current working directory |
| `exit` | Exits the shell |

### External Commands

Any command available in the system `PATH` can be executed, such as:

```bash
ls
ls -l
gcc --version
cat file.txt
date
whoami
```

### Output Redirection

Overwrite a file:

```bash
ls -l > output.txt
```

Append to a file:

```bash
echo "Hello, Rahul" >> output.txt
```

## How It Works

1. The shell displays a prompt and reads a line of input.
2. The input is tokenized into command arguments.
3. The shell checks whether the command is built in.
4. If it is not built in, the shell creates a child process using `fork()`.
5. The child process uses `execvp()` to replace itself with the requested command.
6. If output redirection is requested, the child opens the target file and duplicates its file descriptor onto standard output using `dup2()`.
7. The parent process waits for the child to finish using `waitpid()`.

### Process Flow

```text
rahul-shell> ls -l > files.txt
        |
        +--> fork()
                |
                +--> Child: open("files.txt")
                |           |
                |           +--> dup2(fd, STDOUT_FILENO)
                |           |
                |           +--> execvp("ls", args)
                |
                +--> Parent: waitpid(child_pid)
```

## Project Structure

```text
.
├── include/
│   └── shell.h
├── src/
│   └── main.c
├── Makefile
└── README.md
```

## Build and Run

### Prerequisites

- GCC or Clang
- Make
- Linux or another POSIX-compatible operating system

### Clone the Repository

```bash
git clone [https://github.com/rahulcmca25-hub/mini-linux-shell.git](https://github.com/rahulcmca25-hub/mini-linux-shell.git)
cd mini-linux-shell
```

### Build

```bash
make
```

### Run

```bash
./rahul-shell
```

### Clean Build Files

```bash
make clean
```

## Example Session

```text
rahul-shell> pwd
/home/rahul/projects/mini-linux-shell

rahul-shell> ls
include  Makefile  README.md  src

rahul-shell> echo "Hello from Rahul Shell" > hello.txt

rahul-shell> cat hello.txt
Hello from Rahul Shell

rahul-shell> echo "Appending another line" >> hello.txt

rahul-shell> cat hello.txt
Hello from Rahul Shell
Appending another line

rahul-shell> cd src

rahul-shell> pwd
/home/rahul/projects/mini-linux-shell/src

rahul-shell> exit
```

## System Calls Used

| System Call / Function | Purpose |
|---|---|
| `fork()` | Creates a child process |
| `execvp()` | Replaces the child process with a new program |
| `waitpid()` | Makes the parent wait for the child process |
| `chdir()` | Changes the current working directory |
| `getcwd()` | Gets the current working directory |
| `open()` | Opens or creates a file for redirection |
| `dup2()` | Redirects standard output to a file |
| `close()` | Closes unused file descriptors |

## Current Limitations

- Does not support input redirection using `<`
- Does not support pipes using `|`
- Does not support background execution using `&`
- Does not support quoted arguments, such as `echo "hello world"`
- Does not support multiple redirections or command chaining
- Simple whitespace-based parser, so filenames containing spaces are not fully supported

## Future Improvements

- Add input redirection using `<`
- Add pipe support using `|`
- Add background process execution using `&`
- Handle signals such as `Ctrl+C`
- Support quoted arguments and escape characters
- Add command history using GNU Readline
- Improve parser into a proper tokenizer and parser design

## Author

**Rahul Chadar**  
GitHub: [rahulcmca25-hub](https://github.com/rahulcmca25-hub)