#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<dirent.h>
#include "main.h" 
#include<pwd.h>

//Commands
char* bltn_cmds[] = {
    "cd",
    "echo",
    "pwd",
    "exit",
    "ls",
    "pinfo",
    "jobs"
};

int (*bltn_func[]) (arg_ret) = {
    &bltn_cd,
    &bltn_echo,
    &bltn_pwd,
    &bltn_exit,
    &bltn_ls,
    &bltn_pinfo,
    &bltn_jobs
};

extern int errno;

void print_errno() { printf("Error no: %d\n", errno); }


int check(char letter)
{
    if (letter != ' ' && letter != '\n' && letter != '\t')
        return 1;
    else return 0;
}

arg_ret parse(char* line)
{
	arg_ret cmd;
	cmd.background = 0;
    int buf_size = ARG_SIZE;
    int noa = 50;
    int n = strlen(line);
    // char **cmd_args = (char**)malloc(ARG_SIZE*sizeof(char*));
    cmd.args = (char**)malloc(ARG_SIZE*sizeof(char*));
    // char cmd_args[50][100];
    char* arg = (char*)malloc(ARG_SIZE*sizeof(char));
    int i, j = 0;
    int position =0;
    for (i=0;i<n;i++)
    {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t') break;
    }
    // printf("%d, %d\n", i, n);
    for (;i<n;i++)
    {
        if (check(line[i]))
        {
        	if (line[i] == '&')
        	{
        		cmd.background = 1;
        		// printf("captured &\n");
        		continue;
        	}
            if (line[i] == '\\') {
                i++;
                if (i >= n)
                {
                    continue;
                }
            }
            
            arg[j] = line[i];
            j++;
        }
        else {
            // printf("%s\n" ,arg);
            if (strlen(arg) > 0)
            {
                cmd.args[position] = arg;
                // strcpy(cmd_args[position], arg);
                // for (int z=0;z<strlen(arg);z++, cmd_args[position]++, arg++)
                // {
                //     *cmd_args[position] = *arg;
                // }
                // memcpy(cmd_args[position], arg, strlen(arg));
                position++;
                // for (int z=0;z<position;z++) printf("%s\n", cmd_args[z]);
                arg = (char*)malloc(ARG_SIZE*sizeof(char));
                j = 0;
                if (position >= buf_size) {
                    buf_size += ARG_SIZE;
                    cmd.args = realloc(cmd.args,  buf_size* sizeof(char*));
                }
            }
        }

    }
    if (strlen(arg) > 0)
    {
        cmd.args[position] = arg;
        position++;
    }


    // for (i=0;i<position;i++)
    //     printf("%s\n",cmd.args[i]);
    return cmd;

}

int process(char* line, int num)
{
    // int buf_size = ARG_SIZE;
    // char *cmd = strtok(line, ARGS_DELIMITOR);
    // char **cmd_args = (char**)malloc(ARG_SIZE*sizeof(char*));
    
    // int i=0;
    // char* arg = strtok(NULL,ARGS_DELIMITOR);
    // while (arg != NULL)
    // {
    //     cmd_args[i] = arg;
        
    //     i++;
    //     if (i >= buf_size) {
    //         buf_size += ARG_SIZE;
    //         cmd_args = realloc(cmd_args,  buf_size* sizeof(char*));
    //     }
    //     arg = strtok(NULL, ARGS_DELIMITOR);    
    // } 
    // int no_of_arg = i;

//     for (i = 0; i < NO_OF_BUILTINS; i++) {
//     if (strcmp(cmd, bltn_cmds[i]) == 0) {
//       return (*bltn_func[i])(cmd_args);
//     }
//   }
    arg_ret cmd = parse(line);
    
    for (int i = 0; i < NO_OF_BUILTINS; i++) {
        if (strcmp(cmd.args[0], bltn_cmds[i]) == 0) {
            return (*bltn_func[i])(cmd);
        }
    }

    execute(cmd);
    // free(cmd.args);
    return 0;
}



int main() {
    char *b = NULL;
    char **args;
    size_t n = 0;
    size_t noc;
    // char* username = (char *)malloc(100);
   	// get_username();
   	job_number = 0;
   	char username[100];
   	char hostname[256];
 	gethostname(hostname, 256);
    int i;
    set_home();
    set_pwd();
    set_rwd();
    nof_bg_proc = 0;
    
    // char* username;
    // printf("%s\n",getlogin() );
   	// printf("%d\n",getlogin_r(username, 100));
   	// perror("sash");
    // exit(1);
    // strcat()
    for (i=0;i<100;i++) bg_proc[i].pstatus = 0;
    strcat(PROMPT, ANSI_COLOR_DARK_YELLOW);
    // strcat(PROMPT, "sriteja@acer:");
    strcat(PROMPT, getpwuid(geteuid())->pw_name);
    strcat(PROMPT, "@");
    strcat(PROMPT, hostname);
    strcat(PROMPT, ":");
    strcat(PROMPT, ANSI_COLOR_RESET);
    while(1)
    // {jobs();
    {
        printf("%s%s%s%s%s ", PROMPT, ANSI_COLOR_DARK_BLUE,RWD,ANSI_COLOR_RESET, "$");
        noc = getline(&b, &n, stdin);
        b[noc] = '\0';
        if (b[0] == '\n')  continue;
        if (b[noc-1] == '\n') b[noc-1] = '\0';
        process(b,noc);
        b= (char*)malloc(1);
        b=NULL;
    }

}