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
void sighan(); //ricky
int main();     // ricky




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


int execute_cmd(char *cmd) {


    int bgp = 0;
    char *argv[BUFFER_SIZE];

    if (strncmp(cmd, "cd", 2) == 0 && (cmd[2] == ' ' || cmd[2] == '\0')) {
        char *dir = strtok(cmd + 3, " "); 
        if (dir == NULL) {
            dir = getenv("HOME"); 
        }

        
        if (chdir(dir) != 0) {
            perror("cd failed");
        }
        return 0;
    }

    // check for background & cmd
    if (cmd[strlen(cmd) - 1] == '&') {
        bgp = 1;
        cmd[strlen(cmd) - 1] = '\0'; 
    }

    // start time
    time_t start_time;
    time(&start_time);

    // this if returns the first occurance of `|` in the command. if it is anything but zero, we start handling a pipe
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
        if (!bgp
    ) {
            wait(NULL);
        }
        // while we haven't reached max history, the shell will update the history struct with the current command details for future reference
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
    else {
        //error handlin
        perror("Fork failed");
        return -1;
    }

    return 0;
}



void signal_interrupt_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n");
        show_cmd_history();
        exit(0);
    }
}


// signal handler code. this initialises it
void sighan() {
    struct sigaction sa;
    sa.sa_handler = signal_interrupt_handler;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}


// entrypoint
int main() {
    // Initialize signal handling
    sighan();

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

