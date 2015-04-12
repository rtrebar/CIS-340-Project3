#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "flop.h"

int main()
{
	int i, redirect_bool; // pipe_bool
	int pid;
	char **cmd;
	char buf[512];
	int command_bytes;

    while (1) {
        fprintf(stdout, "\nflop: ");
	fflush(stdout);

        cmd = (char **)malloc(20*sizeof(char *));
		for (i = 0; i < 20; i++)
			cmd[i] = (char *) malloc(128);

		if ((command_bytes = read(0, buf, 512)) < 0) {
                        fprintf(stdout, "There was an issue reading the command, please try again.\n");
			continue;
		}

		buf[command_bytes] = '\0';	/* give a terminate sign for the command */
		if (parse_cmd(buf, cmd)) {
			if (strcmp("exit", cmd[0]) == 0) {
				fprintf(stdout, "Exiting the floppy disk shell... \n");
				return EXIT_SUCCESS;
			}

			if (!strcmp("path", cmd[0])) {
				if ((strchr(cmd[1],'+') == NULL) && (strchr(cmd[1],'-') == NULL)) {
					path();
					continue;
				} else if (!strcmp("+", cmd[1])) {
					if(strchr(cmd[2],'#') == NULL){
						path_add(cmd[2]);
						strcpy(cmd[1],"");
						continue;
					} else {
						fprintf(stdout, "Error invalid argument, please try again! \n");
						continue;
					}
				} else if (!strcmp("-", cmd[1])) {
					if(strchr(cmd[2],'#') == NULL){
						path_sub(cmd[2]);
						strcpy(cmd[1],"");
						continue;
					} else{
						fprintf(stdout, "Error invalid argument, please try again! \n");
					}
				} else {
					fprintf(stdout, "Error invalid argument, please try again! \n");
				}
			} else if (!strcmp("cd", cmd[0])) {
				change_dir((const char*)cmd[1]);
				continue;
			}

			if (is_pipe(cmd)) {
				// TODO: add logic for pipe
				fprintf(stdout, "Adding logic soon\n");
			}
			else {
				if ((redirect_bool = is_redirection(cmd)) > 0) {
					// TODO: add logic for redirect
					fprintf(stdout, "Adding logic soon.\n");
				}
				if ((pid =fork()) == 0) {
					execve(cmd[0], cmd, NULL);
					fprintf(stdout, "shell command is invalid, plesae try again.\n");
					continue;
				}
				wait(NULL);
			}
			free(cmd);
		}
    }

    return 0;
}
