#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include "main.h"

int execute(arg_ret cmd) {
	int status;
	pid_t pid, wt_pid;
	jobs();
	int noa =0;
	while(cmd.args[noa]!=NULL) noa++;
	pid = fork();
	if (!pid) {
		if (execvp(cmd.args[0], cmd.args) == -1)
		{
			printf("%s: command not found\n", cmd.args[0]);
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
			waitpid(pid, &status, WNOHANG);

			printf("[%d]\t%d\n", ++job_number, (int)pid);
			bg_proc[nof_bg_proc].pid = (int)pid;
			bg_proc[nof_bg_proc].pname = (char *)malloc(strlen(cmd.args[0])*sizeof(char));
			bg_proc[nof_bg_proc].pname = cmd.args[0];
			bg_proc[nof_bg_proc].pstatus = 1;
			bg_proc[nof_bg_proc].pjob_num = job_number;
			nof_bg_proc += 1;

		}
		else {
			waitpid(pid, &status, WUNTRACED);
		}
	}
	return 0;
}
