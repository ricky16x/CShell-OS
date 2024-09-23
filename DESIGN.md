# SimpleShell in C - Assignment 2 for OS - CSE231 Monsoon 2024


## How to Compile

To compile the Simple Shell, clone this repository and go to the `CShell-OS` directory and run the following:

```bash
git clone https://github.com/ricky16x/CShell-OS
cd CShell-OS
ls
gcc -o <output-file-name> simple-shell.c
```

or simply run,

```bash
git clone https://github.com/ricky16x/CShell-OS)
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


