#include"main.h"
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
int bltn_pwd(arg_ret cmd)
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    }
    else {
        perror("sash: pwd");
        return 1;
   }
   return 0;
}
int bltn_echo(arg_ret cmd)
{
    int i=0;
    while(cmd.args[i]!=NULL) {
        printf("%s ", cmd.args[i]);
        i++;
    }
    printf("\n");
}

int bltn_cd(arg_ret cmd)
{
    char *dir;
    dir = (char*)malloc(PATH_MAX*sizeof(char));
    if (cmd.args[1] != NULL)
    {
        if (strcmp(cmd.args[1], "-") == 0)
        {
            if (strlen(OLDPWD) > 0) dir = OLDPWD;
            else
            {
                printf("OLDPWD not set\n");
                return 0;
            }
        }
        else if (cmd.args[1][0] == '~')
        {
            if (strlen(cmd.args[1]) > 1)
            {
                if (cmd.args[1][1] == '/')
                {
                    strcat(dir, HOME);
                    strcat(dir, &cmd.args[1][1]);
                }
                else dir = cmd.args[1];
            }
            else dir = HOME;
        }
        else dir = cmd.args[1];
    }
    else dir = HOME;

    int status = chdir(dir);

    if (status != 0)
    {
        // write(2, dir, strlen(dir));
        // write(2, ": No Such directory\n", sizeof(": No Such directory\n"));
        // printf("ERRNO: -> %d", errno);
        // print_errno();
        perror("sash: cd");
    }
    else {
        set_pwd();
        set_rwd();
    }
}

void permissions(char* file_path) {
	char* perm_strng = (char*)malloc(sizeof(char)*30);
	struct stat file;
	struct passwd *owner;
	struct group *gp;
	char t[100];
	// printf("%s\n", file_path);
	int status = lstat(file_path, &file);
	int i = 0;
	// printf("status ->%d, isdir->%d\n", status, (int)file.st_mode);
	mode_t bit = file.st_mode;

	if (S_ISDIR(bit)) perm_strng[i] = 'd';
	else if (S_ISLNK(bit)) perm_strng[i] = 'l';
	else perm_strng[i] = '-';

	i++;

	if (bit & S_IRUSR) perm_strng[i] = 'r'; else perm_strng[i] = '-';i++;
	if (bit & S_IWUSR) perm_strng[i] = 'w'; else perm_strng[i] = '-';i++;
	if (bit & S_IXUSR) perm_strng[i] = 'x'; else perm_strng[i] = '-';i++;
	if (bit & S_IRGRP) perm_strng[i] = 'r'; else perm_strng[i] = '-';i++;
	if (bit & S_IWGRP) perm_strng[i] = 'w'; else perm_strng[i] = '-';i++;
	if (bit & S_IXGRP) perm_strng[i] = 'x'; else perm_strng[i] = '-';i++;
	if (bit & S_IROTH) perm_strng[i] = 'r'; else perm_strng[i] = '-';i++;
	if (bit & S_IWOTH) perm_strng[i] = 'w'; else perm_strng[i] = '-';i++;
	if (bit & S_IXOTH) perm_strng[i] = 'x'; else perm_strng[i] = '-';

	perm_strng[i] = ' '; i++;
	perm_strng[i] = '\0';
	printf("%s",perm_strng);
	printf("%2lu ",file.st_nlink);
    owner = getpwuid(file.st_uid);
    gp = getgrgid(file.st_gid);
    printf("%s %s ",owner->pw_name,gp->gr_name);
    printf("%6ld ",file.st_size);
    // return;
    strcpy(t,ctime(&file.st_mtime));
    t[16]='\0';
    printf("%s ",t);
	return;
}



int bltn_ls(arg_ret cmd)
{
	extern char* optarg;
	extern int optind, opterr;
	opterr = 0;
	optind = 0;
	int got_l = 0;
	int got_a = 0;
	DIR* direc;
    struct dirent *dir_struct;
    int noa =0;
    char * directory = (char *)malloc(sizeof(char)*PATH_MAX);
    while(cmd.args[noa]!=NULL) noa++;
    char option;

    	// option = getopt(noa, args, "la");

    	while ((option = getopt(noa, cmd.args,"la")) != -1) {
        switch (option) {
             case 'a' : got_a = 1;
                 break;
             case 'l' : got_l = 1;
                 break;
             default: printf("Usage: -[la] {dir}\n");
                 return -1;
        }
    }


    // printf("args[0] -> %s, noa->%d  option-> %c, got-l->%d, got-a->%d, optind->%d\n",cmd.args[0], noa, (char)option, got_l, got_a, optind);
    if (cmd.args[optind] == NULL){
    	directory = PWD;
    }
    else if (cmd.args[optind][0] == '~')
    {
        if (strlen(cmd.args[optind]) > 1)
        {
            if (cmd.args[optind][1] == '/')
            {
                strcat(directory, HOME);
                strcat(directory, &cmd.args[optind][1]);
            }
            else directory = cmd.args[optind];
        }
        else directory = HOME;
        // optind = 1;
    }
    else {
    	directory = cmd.args[optind];
  		optind = 1;
    }
    // printf("direc- >%s\n",directory );
    char *temp;
    temp = (char*)malloc(sizeof(char)*PATH_MAX);
    strcpy(temp, directory);
    direc=opendir(directory);
    if (direc!=NULL)
    {
        dir_struct = readdir(direc);
        while (dir_struct)
        {

        	if (dir_struct -> d_name[0] == '.')
        	{
        		if (got_a) {
        			if (got_l)
        			{
		        		strcat(temp, "/");
		        		strcat(temp, dir_struct->d_name);
		        		permissions(temp);
		        		// printf("%s\t", temp);
		        		temp = (char*)malloc(PATH_MAX*sizeof(char));
		        		strcpy(temp, directory);
        				// printf("%s\t", permissions(temp));
        			}
            		printf("%s\n", dir_struct -> d_name);
            	}
            }
            else
            {
            	if (got_l) {
	            	strcat(temp, "/");
	        		strcat(temp, dir_struct->d_name);
	        		permissions(temp);
	        		// printf("%s\t", temp);
	        		temp = (char*)malloc(PATH_MAX*sizeof(char));
	        		strcpy(temp, directory);
	        	}
	            printf("%s\n", dir_struct -> d_name);

            }
            dir_struct = readdir(direc);

        }
    }
    else {
        // printf("%s: No such drectory.\n", directory);
        // print_errno();
        perror("sash: ls");
    }
    // free(cmd.args);
    // free(directory);
}

void pid_int_to_char(char pid_arr[],int pid)
{
    int temp = pid;
    int count = 0;
    while(temp != 0)
    {
      count++;
      temp = temp/10;
    }
    temp = pid;
    pid_arr[count] = 0;
    while(temp != 0)
    {
      pid_arr[count-1] = temp%10 + '0';
      temp = temp/10;
      count--;
    }

    return;
}



int bltn_pinfo(arg_ret cmd)
{
	char pid[20];
	if (cmd.args[1] == NULL) sprintf(pid, "%d",getpid());
	else strcpy(pid, cmd.args[1]);
	// exit(0);
	char file[10000];
	char exec[10000] = "/proc/";
	char status[10000] = "/proc/";
	strcat(exec, pid);
	strcat(status, pid);
	strcat(status, "/stat");
	strcat(exec, "/exe");
	// printf("%s\n",file );
	int len;
	if ((len=readlink(exec, file, 1000)) == -1){
		perror("sash: pinfo");
	}
	file[len] = '\0';
	int fd = open(status, O_RDONLY);
	if(fd == -1){
	    perror("sash: pinfo");
	    return 1;
	}

	char data[300];
  	read(fd, data, 300);
  	char* val = strtok(data," \t\n\r\a");
  	int count = 1;
  	// char *state=(char*)malloc(200*sizeof(char));
  	// char *vmem=(char*)malloc(200*sizeof(char));
  	char state[200];
  	char vmem[200];


      while(val != NULL)
      {
        // if(count == 1)
        // {
        //   strcpy(Id,val);
        // }
        // printf("%d -> %s\n",count, val );
        if(count == 2)
        {

          strcpy(state,val);

        }
        if(count == 25)
        {
          strcpy(vmem,val);
        }
        count++;
        val = strtok(NULL,val);
      }

      printf("pid -- %s\nProcess Status -- %s\n%s{Virtual Memory}\nExecutable Path --  %s\n", pid, state, vmem, file);
 //  	stat = ashGetArgs(content, " \t\r\n\a");
	// printf("pid -- %s\nProcess Status -- %s\nVirtual Memory-- %s\nExecutable Path -- %s\n", stat[0], stat[2], stat[23], exePath);
	// printf("%s\n", exec);

	// exit(1);

}

int bltn_exit(arg_ret cmd)
{
	// for (int i=0; i< nof_bg_proc;i++)
	// {
	// 	execvp("kill", (char)nof_bg_proc);
	// }

	write(1, EXIT_PROMPT, sizeof(EXIT_PROMPT));

    exit(0);

}

int bltn_jobs(arg_ret cmd)
{
	printf("JOB_ID\tPID\tPROCESS\n");
	for(int i=0;i<job_number;i++)
	{
		if (bg_proc[i].pstatus)
			printf("%-7d %-7d %s\n",bg_proc[i].pjob_num, bg_proc[i].pid, bg_proc[i].pname);
	}
}


// int main() {}
