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

void sigchd_handler()
{
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	// if (!(int)WEXITSTATUS(status)) {
		int i, flag=0;
		for (i=0;i<nof_bg_proc;i++) {
			if (bg_proc[i].pid == (int) pid) {
				bg_proc[i].pstatus = 0;
				flag = 1;
				break;
			}
		}
		if (flag == 1) {
			nof_bg_proc--;
			int exit_status = WEXITSTATUS(status);
			if (exit_status == 0) fprintf(stderr, "[%d] Done normally\t\t%s\n",(int)pid, bg_proc[i].pname);
			else if(exit_status == 1) fprintf(stderr, "[%d] Done with impermisable opertaion errors \t\t%s\n",(int)pid, bg_proc[i].pname);
			else if (exit_status == 126) fprintf(stderr, "[%d] Done with permission problem \t\t%s\n",(int)pid, bg_proc[i].pname);
		}
	// }
}

void jobs() {
	// for (i=0;i<nof_bg_proc;i++)
	// {
		
		// wt_pid = waitpid((pid_t)bg_proc[i].pid, bg, WUNTRACED);
		// waitpid(bg_proc[i].pid, bg_proc[i].pstatus, WNOHANG);
		// waitpid(bg_proc[i].pid, &val, WNOHANG);
		// printf("%d, val -> %d\n",*bg_proc[i].pstatus, val );

		// // if (WIFEXITED(*bg_proc[i].pstatus) || WIFSIGNALED(*bg_proc[i].pstatus)) {
		// printf("came here\n");
		// // if (WIFEXITED(*bg_proc[i].pstatus)) {
		// if (WIFEXITED(val)) {
		// 	printf("%d Done\n",bg_proc[i].pid);
		// }
		// printf("come\n");
		// pid_t pid = wait(NULL);
		// if (pid!=-1) {
		// 	printf("%d Done\n",pid);
		// }
		signal(SIGCHLD, sigchd_handler); 

					
		// }
	// }
}