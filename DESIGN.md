# SimpleShell in C - Assignment 2 for OS - CSE231 Monsoon 2024
---


You can find the course code in - [CShell-OS](https://github.com/ricky16x/CShell-OS)

# GitHub Repository Structure
___

#### File Descriptions
___
#### Root Directory

- **Assgn2.pdf**: Assignment 2 PDF (as provided).
- **DESIGN.md**: Design documentation for the project.
- **Makefile**: Build instructions for compiling the project.
- **README.md**: Overview and instructions for using the repository.
- **fib.c**: C program file for Fibonacci sequence.
- **file.txt**: Sample text file used in the project. We have kept it to show multiple pipe handling, basically we will run `cat file.txt | grep Rick | sort | tee ans.txt` or `cat file.txt | grep Karan | sort | tee ans.txt`.
- **helloworld.c**: C program file for "Hello, World <name>!" example.
- **simple-shell.c**: Source code for the simple shell program.


Developed by **Rounak Dey** and **Karan Kumar** (Roll Nos. 2023449 and 2023270) for the course **CSE231 - Operating Systems** under **Prof. Vivek Kumar**, Monsoon 2024.

This assignment is shell program written in C. It implements command execution, handles piped commands and has support for `&` commands for background processes. You can view command history using `bash_history`.

## How to Compile

To compile the Simple Shell, go to the `CShell-OS` directory and run the following:

```bash
cd CShell-OS
ls
gcc -o <output-file-name> simple-shell.c
```

or simply run,

```bash
cd CShell-OS
make
```

## How to Run

To run, use:
```bash
ls 
./<output-file-name>
```

### **Note**: If you used `make` to build, the output-file-name is `shell`. You need to run `./shell` to run it.

This should create the shell loop as below:

```bash
CSE231@GP44:~$> 
```
where you can run your commands. Each command starts a process with a unique PID, which can be seen using the `bash_history` command.


#### To cleanup, run:
```bash
make clean
```
---
## Codeflow 
---
### Function Descriptions
---
### `main()`
This is the entry point of the shell program. It initializes signal handling and processes user input in a loop, checking for special commands like `bash_history` and `exit`. If the input is not a special command, it calls `execute_cmd()` to handle it.

### `execute_cmd(char *cmd)`
Executes a given command, supporting both background and foreground processes. It checks for piped commands and delegates them to `handle_piped_cmds()` if found. It also updates the command history with execution details.

### `handle_piped_cmds(char *cmd)`
Manages the execution of piped commands by creating child processes and pipes for each segment of the command. It sets up input/output redirection as needed and stores command details in history. It ensures proper execution of all commands in the pipeline.

### `show_cmd_history()`
Displays the history of executed commands, including details like command string, process ID, start time, and execution duration. It iterates through the `history` array up to `history_index`. This function is called either on `bash_history` command or when `SIGINT` is received.

### `signal_interrupt_handler(int signo)`
Handles `SIGINT` signals (Ctrl+C) by displaying the command history and exiting the shell gracefully. It prints a newline, calls `show_cmd_history()`, and terminates the program. This ensures that the shell handles interrupts without abruptly terminating.

---
## Limitations of the Shell Program
---

1. **No Support for I/O Redirection**:
   - Implementing I/O redirection would require handling multiple file descriptors and modifying the execution flow significantly, which was beyond the current project scope and timeline.

2. **No Support for Advanced Features**:
   - Features like command substitution and logical operators add significant complexity to the command parsing logic. Due to time constraints, we focused on ensuring reliable execution of basic and piped commands.


3. **Limited Signal Handling**:
   - Handling additional signals such as `SIGTSTP` and `SIGQUIT` would require more complex state management and interrupt handling, which we avoided to keep the implementation straightforward and focused on essential features.



---
