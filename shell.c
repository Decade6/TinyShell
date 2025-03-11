//Declan Murphy
//COP4610
//MP1: Tiny Unix Shell

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* ----------------------------------------------------------------- */
/* FUNCTION  parse:                                                  */
/*    This function takes an input line and parse it into tokens.    */
/* It first replaces all white spaces with zeros until it hits a     */
/* non-white space character which indicates the beginning of an     */
/* argument.  It saves the address to argv[], and then skips all     */
/* non-white spaces which constitute the argument.                   */
/* ----------------------------------------------------------------- */

void parse(char *line, char **argv) {
    static char* delimiter = " \n\t";
    char *token = strtok(line, delimiter);
    while (token != NULL) {
        *argv++ = token;
        token = strtok(NULL, delimiter);
    }
    *argv = NULL; // Use NULL to terminate array
}

/* ----------------------------------------------------------------- */
/* FUNCTION execute:                                                 */
/*    This function receives a commend line argument list with the   */
/* first one being a file name followed by its arguments.  Then,     */
/* this function forks a child process to execute the command using  */
/* system call execvp().                                             */
/* ----------------------------------------------------------------- */

void execute(char *line) {
    char *commands[64];
    char *command = strtok(line, ";");
    int n = 0, i = 0;
    pid_t pid, wpid;
    int status = 0;

    // Split line into separate commands
    while (command != NULL) {
        commands[n++] = command;
        command = strtok(NULL, ";");
    }
    commands[n] = NULL;

    // Check for the "quit" command
    if (strcmp(commands[0], "quit") == 0) {
        if (n > 1) {
            // Execute other commands first
            for (i = 1; i < n; i++) {
                char *argv[64];
                parse(commands[i], argv);
                if (argv[0] == NULL) continue; // Skip empty commands
                pid = fork();
                if (pid == 0) { // Child process
                    if (execvp(argv[0], argv) < 0) {
                        printf("*** ERROR: exec failed\n");
                        exit(1);
                    }
                } else if (pid < 0) {
                    printf("*** ERROR: forking child process failed\n");
                    exit(1);
                }
            }
            // Wait for all child processes to finish
            while ((wpid = wait(&status)) > 0);
        }
        exit(0); // Exit the shell
        return; // Return without attempting to execute the "quit" command
    }

    // Execute each command concurrently
    for (i = 0; i < n; i++) {
        char *argv[64];
        parse(commands[i], argv);
        if (argv[0] == NULL) continue; // Skip empty commands
        if (strcmp(argv[0], "quit") == 0) exit(0); // Handle quit command

        pid = fork();
        if (pid == 0) { // Child process
            if (execvp(argv[0], argv) < 0) {
                printf("*** ERROR: exec failed\n");
                exit(1);
            }
        } else if (pid < 0) {
            printf("*** ERROR: forking child process failed\n");
            exit(1);
        }
    }

    // Wait for all child processes to finish
    while ((wpid = wait(&status)) > 0);
}

/* ----------------------------------------------------------------- */
/*                  The main program starts here                     */
/* ----------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    char line[1024];
    FILE *batchFile = NULL;

    if (argc == 2) {
        batchFile = fopen(argv[1], "r");
        if (!batchFile) {
            fprintf(stderr, "Error opening batch file\n");
            exit(1);
        }
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchFile]\n", argv[0]);
        exit(1);
    }

    FILE *inputSource = batchFile ? batchFile : stdin;
	
	printf("Shell -> ");

    while (fgets(line, sizeof(line), inputSource)) {
        if (batchFile) {
            printf("%s", line); // Echo the command line when in batch mode
        }
        execute(line); // Execute the entire line
        if (!batchFile) {
            printf("Shell -> ");
        }
    }

    if (batchFile) {
        fclose(batchFile);
    }

    return 0;
}