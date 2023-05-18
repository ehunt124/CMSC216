/* Name: Ethan Hunt  UID: 117171051  DirID: ehunt124 */
#include <sys/wait.h>
#include <sysexits.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "executor.h"

/*static void print_tree(struct tree *t);*/

static int execute_aux(struct tree *t, int p_input_fd, int p_output_fd) {
   pid_t pid, subshell_pid, pipe_pid;
   int fd, status_return, stdout_renew;
   int pipe_fd[2];
   char buffer[1024];
   if(t->conjunction == NONE) {
      if(strcmp(t->argv[0], "exit") == 0) {
         exit(0);
      } else if(strcmp(t->argv[0], "cd") == 0) {
         if(chdir(t->argv[1]) == -1) {
            return 1;
         }
         return 0;
      } else {
         fflush(stdout);
         pid = fork();
         if(pid == -1) { /* Fork error */
            return 1;
         } else if(pid == 0) { /* Child */
            if(t->input == NULL) {
               fd = p_input_fd;
            } else {
               fd = open(t->input, 0666);
            }
            if(dup2(fd, STDIN_FILENO) == -1) {
               exit(1);
            }
            if(t->output == NULL) {
               fd = p_output_fd;
            } else {
               fd = open(t->output, 0666);
            }
            if(dup2(fd, STDOUT_FILENO) == -1) {
               exit(1);
            }
            execvp(t->argv[0], t->argv);
            exit(1); /* If we reach this it is an error and we exit */
         } else { /* Parent */
            int status;
            wait(&status);
            return status;
         }
      }
   } else if(t->conjunction == AND) {
      status_return = execute_aux(t->left, p_input_fd, p_output_fd);
      if(status_return > 0) {
         return status_return;
      }
      return execute_aux(t->right, p_input_fd, p_output_fd);
   } else if(t->conjunction == PIPE) {
      fflush(stdout);
      pipe(pipe_fd);
      pid = fork();
      if(pid == -1) {
         return 1;
      } else if(pid == 0) { /* Child + Read Side */
         close(pipe_fd[1]);
         dup2(pipe_fd[0], STDIN_FILENO);
         close(pipe_fd[0]);
         execvp(t->right->argv[0], t->right->argv);
         exit(1);
      } else { /* Parent / Write Side */
         int status;
         close(pipe_fd[0]);
         pipe_pid = fork();
         if(pipe_pid == -1) { /* Fork error */
            return 1;
         } else if(pipe_pid == 0) { /* Child */
            if(t->left->input == NULL) {
               fd = p_input_fd;
            } else {
               fd = open(t->left->input, 0666);
            }
            if(dup2(fd, STDIN_FILENO) == -1) {
               exit(1);
            }
            if(t->left->output == NULL) {
               fd = p_output_fd;
            } else {
               fd = open(t->left->output, 0666);
            }
            if(dup2(fd, STDOUT_FILENO) == -1) {
               exit(1);
            }
            execvp(t->left->argv[0], t->left->argv);
            exit(1); /* If we reach this it is an error and we exit */
         } else { /* Parent */
            int pipe_status;
            wait(&pipe_status);
            exit(pipe_status);
         }
         write(pipe_fd[1], stdout, 1024);
         close(pipe_fd[1]);
         wait(&status);
         return status;
      }
   } else if(t->conjunction == SUBSHELL) {
      fflush(stdout);
      subshell_pid = fork();
      if(subshell_pid == -1) { 
         return 1;
      } else if(subshell_pid == 0) {
         pid = fork();
         if(pid == -1) { /* Fork error */
            exit(1);
         } else if(pid == 0) { /* Child */
            if(t->input == NULL) {
               fd = p_input_fd;
            } else {
               fd = open(t->input, 0666);
            }
            if(dup2(fd, STDIN_FILENO) == -1) {
               exit(1);
            }
            if(t->output == NULL) {
               fd = p_output_fd;
            } else {
               fd = open(t->output, 0666);
            }
            if(dup2(fd, STDOUT_FILENO) == -1) {
               exit(1);
            }
            execvp(t->argv[0], t->argv);
            exit(1); /* If we reach this it is an error and we exit */
         } else { /* Parent */
            int status;
            wait(&status);
            return status;
         }
      } else {
         int subshell_status;
         wait(&subshell_status);
         exit(subshell_status);
      }
   }
   return 0;
}

int execute(struct tree *t) {
   /*print_tree(t);*/
   if(t == NULL) {
      return 1;
   }
   return execute_aux(t, STDIN_FILENO, STDOUT_FILENO);
}

/* Excluded to avoid warnings
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}*/