#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include<signal.h>
#include "main.h"


int execute(arg_ret cmd) {
	int status;
	pid_t pid, wt_pid;
	jobs();
	int noa =0;
	while(cmd.args[noa]!=NULL) noa++;
	pid = fork();
	if (!pid) {
		pid = getpid();
		if (cmd.background==1) {
			setpgid(0,0);
		}
		// printf("pid-%d\n",pid);
		// setpgid(pid,0);
		// if (isatty(0)) tcsetpgrp(0, pid);
		// if (isatty(1)) tcsetpgrp(1, pid);
		// if (isatty(2)) tcsetpgrp(2, pid);
		if (execvp(cmd.args[0], cmd.args) == -1)
		{
			fprintf(stderr, "%s: command not found\n", cmd.args[0]);
		}
		exit(1);
	}
	else if (pid < 0)
	{
		perror("sash");
		return 0;

	}
	else{
		// wt_pid = waitpid(pid, &status, WUNTRACED);
		// while (!WIFEXITED(status) && !WIFSIGNALED(status)) {
			// 		wt_pid = waitpid(pid, &status, WUNTRACED);	
		// }
		if (cmd.background)
		{
			kill(pid, SIGTTIN);
			// kill(pid, SIGTSTP);
			kill(pid, SIGCONT);
			waitpid(pid, &status, WNOHANG);
			//Dont comment this printf;
			printf("[%d]\t%d\n", ++job_number, (int)pid);
			bg_proc[nof_bg_proc].pid = (int)pid;
			bg_proc[nof_bg_proc].pname = (char *)malloc(strlen(cmd.args[0])*sizeof(char));
			bg_proc[nof_bg_proc].pstat = (char*)malloc(10*sizeof(char));
			bg_proc[nof_bg_proc].pstat = "Running";
			bg_proc[nof_bg_proc].pname = cmd.args[0];
			bg_proc[nof_bg_proc].pstatus = 1;
			bg_proc[nof_bg_proc].pjob_num = job_number;
			nof_bg_proc += 1;

		}
		else {
			// setpgid(pid, pid);
			int sig_num;
			waitpid(pid, &status, WUNTRACED);
			// printf("came\n");
			if (WIFSTOPPED(status)) {
				// printf("came here\n");
				sig_num = WSTOPSIG(status);
				// printf("%d",sig_num);
				if (sig_num == SIGTSTP) {
					printf("[%d] Suspended \t\t %s \n",++job_number, cmd.args[0]);
					bg_proc[nof_bg_proc].pid = (int)pid;
					bg_proc[nof_bg_proc].pname = (char *)malloc(strlen(cmd.args[0])*sizeof(char));
					bg_proc[nof_bg_proc].pstat = (char*)malloc(10*sizeof(char));
					bg_proc[nof_bg_proc].pstat = "Suspended";
					bg_proc[nof_bg_proc].pname = cmd.args[0];
					bg_proc[nof_bg_proc].pstatus = 1;
					bg_proc[nof_bg_proc].pjob_num = job_number;
					nof_bg_proc ++;
					
				}
				
			}
			else if (WIFSIGNALED(status)) {
				if (WTERMSIG(status) == SIGINT){
					printf("[%d] Killed \t\t %s \n",(int)pid, cmd.args[0]);
					
				}
			}
			
		 }
	}
	return 0;
}