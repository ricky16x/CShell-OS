# SimpleShell in C - Assignment 2 for OS - CSE231 Monsoon 2024


You can find the course code in - [CShell-OS](https://github.com/ricky16x/CShell-OS)


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

### **Note**: 

This should create the shell loop as below:

```bash
CSE231@GP44:~$> 
```
where you can run your commands. Each command starts a process with a unique PID, which can be seen using the `bash_history` command.


To cleanup, run:
```bash
make clean
```


---
