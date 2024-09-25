#define _XOPEN_SOURCE 700  // line 23x gives error with VSCode. I use Sublime, but for everyone who uses VSc, this fixed it - taken from Stack Overflow
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define MAX_HISTORY 200

//made this array for global action, used in main() and self explanatory
const char *excm[] = {"bash_history", "exit"};

//plan
int handle_piped_cmds(char *cmd); // karan
int execute_cmd(char *cmd); // ricky
void show_cmd_history(); // karan
void signal_interrupt_handler(int signo); //ricky
void record_command_history(const char *cmd, pid_t pid, time_t start_time); // Function declaration

//cmd history 
typedef struct {
    char cmd[BUFFER_SIZE];
    pid_t process_id;
    time_t start_time;
    double exec_tim;
} cmdHistory;

//creating a 'history' object array with 200 size. this will store details as in the `cmdHistory` struct for 200 cmds 
cmdHistory history[MAX_HISTORY];
int history_index = 0; //counting history

// Function to record command history
void record_command_history(const char *cmd, pid_t pid, time_t start_time) {
    if (history_index < MAX_HISTORY) {
        time_t end_time;
        time(&end_time);
        double duration = difftime(end_time, start_time);

        strncpy(history[history_index].cmd, cmd, BUFFER_SIZE);
        history[history_index].process_id = pid;
        history[history_index].start_time = start_time;
        history[history_index].exec_tim = duration;
        history_index++;
    }
}

// pipe handler
int handle_piped_cmds(char *cmd) {
    char *pipe_position;          
    int fd[2];                    
    int previous_fd = 0;          
    char *argv[BUFFER_SIZE];
    time_t start_time;
    time(&start_time);
    
    // strchr returns reference to the first position of |
    while ((pipe_position = strchr(cmd, '|')) != NULL) {
        *pipe_position = '\0';     
        
        if (pipe(fd) == -1) {
            perror("Pipe failed");
            return -1;
        }

        pid_t pid = fork(); 
        // CHILD HAS BEEN FORKED
        if (pid == 0) {
            //redirect stdin to read end of previous pipe
            dup2(previous_fd, 0);

            // check is there's a comment after pipe
            if (pipe_position[1] != '\0') {
                dup2(fd[1], 1);    // redirect stdout to write end of pipe
            }
            close(fd[0]);        // close read end  
            
            // break commands into arguments
            char *parts = strtok(cmd, " ");
            int i = 0;
            while (parts != NULL) {
                argv[i] = parts;   // Store each token in argv
                i++;
                parts = strtok(NULL, " ");
            }
            argv[i] = NULL;       
            
            // exec
            execvp(argv[0], argv);
            perror("Execution failed"); // Print error if execution fails
            exit(1);                 // Exit child process
        } else {
            // good parents shall always wait for their child
            wait(NULL); 
            record_command_history(cmd, pid, start_time); // Record history
            close(fd[1]);  // Close the write end       

            previous_fd = fd[0];    // update previous_fd to read end of current pipe
            cmd = pipe_position + 1; // Move cmd pointer to the next command after the pipe, and since it's a while loop, we can find the next pipe at L40.
        }
    }
    //CHILD FORKED :D
    pid_t pid = fork();

    // Handle the last command (after the last pipe)
    if (pid == 0) { //child code
         // Redirect input from the previous pipe
        dup2(previous_fd, 0);      
        
        // Parse the final cmd into arguments
        char *parts = strtok(cmd, " ");
        int i = 0;
        while (parts != NULL) {
            argv[i++] = parts;      
            parts = strtok(NULL, " ");
        }
        argv[i] = NULL;             
        
        execvp(argv[0], argv);     //exec
        perror("Execution failed"); 
        exit(1);                   // Exit child process
    } else {
        close(previous_fd);         // Close the read end of the last pipe
        wait(NULL);                 // Wait for the last child process to finish
        record_command_history(cmd, pid, start_time); // Record history
    }

    return 0; // Return success
}

// Command execution function
int execute_cmd(char *cmd) {
    int is_background = 0;
    char *argv[BUFFER_SIZE];

    // check for background & cmd
    if (cmd[strlen(cmd) - 1] == '&') {
        is_background = 1;
        cmd[strlen(cmd) - 1] = '\0'; 
    }

    // start time
    time_t start_time;
    time(&start_time);

    // this if returns the first occurrence of `|` in the command. if it is anything but zero, we start handling a pipe
    if (strchr(cmd, '|')) {
        return handle_piped_cmds(cmd);
    }

    // forked a child :D (do not try irl)
    pid_t pid = fork(); 
    if (pid == 0) { // fork returns 0 to child
        
        //child code
        // this breaks cmd into parts every whitespace
        // the i counts every part and they are stored in the argv array. 
        char *parts = strtok(cmd, " ");
        int i = 0;
        while (parts != NULL) {
            argv[i++] = parts;
            parts = strtok(NULL, " ");
        }
        argv[i] = NULL;

        //exec 
        execvp(argv[0], argv);
        perror("Execution failed");
        exit(1);
    } 
    else if (pid > 0) { //fork returns child's PID to parent
        // if not a background process, the parent (shell) shall wait for the child.
        if (!is_background) {
            wait(NULL);
        }
        // while we haven't reached max history, the shell will update the history struct with the current command details for future reference
        record_command_history(cmd, pid, start_time); // Record history
    }
    else {
        //error handling
        perror("Fork failed");
        return -1;
    }

    return 0;
}

//cmd history
void show_cmd_history() {
    for (int i = 0; i < history_index; i++) {
        printf("cmd: %s\nProcess ID: %d\nStart Time: %sDuration: %.2f seconds\n\n",
               history[i].cmd, history[i].process_id, ctime(&history[i].start_time), history[i].exec_tim);
    }
}

// Signal interrupt handler
void signal_interrupt_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n");
        show_cmd_history();
        exit(0);
    }
}

// entrypoint
int main() {
    struct sigaction sa;
    sa.sa_handler = signal_interrupt_handler;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // Input buffer for arguments
    char argv[BUFFER_SIZE];

    // Status variable to control the loop (1 = continue, 0 = exit)
    int status = 1;

    // Shell loop begins
    do {
        // Shell prompt
        printf("CSE231@GP44:~$> ");
        fflush(stdout);

        // Read user input
        if (fgets(argv, sizeof(argv), stdin) == NULL) {
            printf("\n");       
            status = 0;         
            break;
        }

        // Remove newline character from input
        size_t len = strlen(argv); 
        if (len > 0 && argv[len - 1] == '\n') { 
            argv[len - 1] = '\0'; 
        }

        // Handle special commands
        if (strcmp(argv, excm[0]) == 0) {
            show_cmd_history();
        } 
        else if (strcmp(argv, excm[1]) == 0) {
            show_cmd_history();
            status = 0;         
        } 
        else {
            execute_cmd(argv);
        }
    } while (status);           

    return 0;
}
