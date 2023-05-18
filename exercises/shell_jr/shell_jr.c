/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <sys/wait.h>
#include <sysexits.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    /*Use execvp*/
    int length = 0;
    char line[1025] = {0}, arg1[1025] = {0}, arg2[1025] = {0};
    char *line_ptr = line;
    pid_t pid;
    while(1) {
        strcpy(arg1, "");
        strcpy(arg2, "");

        printf("shell_jr: ");
        fgets(line, 1025, stdin);
        if(!feof(stdin)) {
            sscanf(line, "%s", arg1);
            length = strlen(arg1);
            line_ptr = length + line;
            sscanf(line_ptr, "%s", arg2);
        } else {
            exit(0);
        }

        if(strcmp(arg1, "exit") == 0 || strcmp(arg1, "hastalavista") == 0) {
            printf("See you\n");
            exit(0);
        } else if(strcmp(arg1, "cd") == 0) {
            if(chdir(arg2) == -1) {
                printf("Cannot change to directory ");
                printf(arg2);
                printf("\n");
            }
        } else {
            fflush(stdout);
            pid = fork();
            if(pid == -1) { /* Fork error */
                exit(EX_OSERR);
            } else if(pid == 0) { /* Child */
                argv[0] = arg1;
                if(strcmp(arg2, "") != 0) {
                    argv[1] = arg2;
                    argv[2] = NULL;
                } else {
                    argv[1] = NULL;
                }
                execvp(argv[0], argv);
                printf("Failed to execute ");
                printf(arg1);
                printf("\n");
                exit(EX_OSERR);
            } else { /* Parent */
                int status;
                wait(&status);
            }
        }
    }
    return 0;
}