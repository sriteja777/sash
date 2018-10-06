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
#include<signal.h>

//Commands
char* bltn_cmds[] = {
    "cd",
    "echo",
    "pwd",
    "exit",
    "ls",
    "pinfo",
    "jobs",
    "setenv",
    "unsetenv",
    "kjob",
    "overkill",
    "fg",
    "bg"
};

int (*bltn_func[]) (arg_ret) = {
    &bltn_cd,
    &bltn_echo,
    &bltn_pwd,
    &bltn_exit,
    &bltn_ls,
    &bltn_pinfo,
    &bltn_jobs,
    &bltn_setenv,
    &bltn_unsetenv,
    &bltn_kjob,
    &bltn_overkill,
    &bltn_fg,
    &bltn_bg
};

extern int errno;



void print_errno() { printf("Error no: %d\n", errno); }


int check(char letter)
{
    if (letter != ' ' && letter != '\n' && letter != '\t')
        return 1;
    else return 0;
}

cmds_ret parse(char* line)
{
	arg_ret cmd;
    cmds_ret line_str;
    line_str.no_of_cmds = 0;
    // arg_ret* cmds;

    line_str.cmds =(arg_ret*)malloc(sizeof(arg_ret)*10);
    for (int i=0;i<MAX_NO_OF_PIPES;i++) {
        line_str.cmds[i].background = 0;
        line_str.cmds[i].input_red.status = 0;
        line_str.cmds[i].output_red.status = 0;
        line_str.cmds[i].output_red.append = 0;
        line_str.cmds[i].pipe.status = 0;
    }
	// cmd.background = 0;
    // cmd.input_red.status = 0;
    // cmd.output_red.status = 0;
    // cmd.output_red.append = 0;
    // cmd.pipe.status = 0;
    int change=0;
    int buf_size = ARG_SIZE;
    int noa = 50;
    int n = strlen(line);
    int position[MAX_NO_OF_PIPES];
    for (int i=0;i<MAX_NO_OF_PIPES;i++) {
        line_str.cmds[i].args = (char**)malloc(ARG_SIZE*sizeof(char*));
        position[i]=0;
    }// char cmd_args[50][100];
    // cmd.args = (char**)malloc(ARG_SIZE*sizeof(char*));
    char* arg = (char*)malloc(ARG_SIZE*sizeof(char));
    int i, j = 0, k=0, l=0;
    
    for (i=0;i<n;i++)
    {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\t') break;
    }
    // printf("dfhnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
    // printf("i->%d\n",i);
    if (i==n)  {
        // line_str.no_of_cmds = 0;
        return line_str;
    }
    // printf("%d, %d\n", i, n);
    for (;i<n;i++)
    {
        if (check(line[i]))
        {
        	if (line[i] == '&')
        	{
        		// cmd.background = 1;
                line_str.cmds[l].background = 1;
        		// printf("captured &\n");
        		continue;
        	}
            if (line[i] == '>') {
                if (i< n-1 && line[i+1]=='>') {
                    // cmd.output_red.append = 1;
                    line_str.cmds[l].output_red.append = 1;
                    i++;
                }
                // cmd.output_red.status = 1;
                // cmd.output_red.index = position;
                line_str.cmds[l].output_red.status = 1;
                line_str.cmds[l].output_red.index = position[l];
                // printf("arg -> %s,\n",arg);
                if (strlen(arg) > 0)
                {
                    // cmd.args[position] = arg;
                    // cmd.output_red.index += 1;
                    line_str.cmds[l].args[position[l]] = arg;
                    line_str.cmds[l].output_red.index += 1;
                    // strcpy(cmd_args[position], arg);
                    // for (int z=0;z<strlen(arg);z++, cmd_args[position]++, arg++)
                    // {
                    //       *cmd_args[position] = *arg;
                    // }
                    // memcpy(cmd_args[position], arg, strlen(arg));
                    position[l]++;
                    // for (int z=0;z<position;z++) printf("%s\n", cmd_args[z]);
                    arg = (char*)malloc(ARG_SIZE*sizeof(char));
                    j = 0;
                    if (position[l] >= buf_size) {
                        buf_size += ARG_SIZE;
                        // cmd.args = realloc(cmd.args,  buf_size* sizeof(char*));
                        line_str.cmds[l].args = realloc(line_str.cmds[l].args, buf_size*sizeof(char*));
                    }
                }
                // cmd.input_red.filename = (char*)malloc(PATH_MAX*sizeof(char));
                // k=0;
                // while(check(line[i])) {
                    

                // }
                continue;
            }
            if (line[i] == '<') {
                // cmd.input_red.status = 1;
                // cmd.input_red.index = position;
                line_str.cmds[l].input_red.status = 1;
                line_str.cmds[l].input_red.index = position[l];
                if (strlen(arg) > 0)
                {
                    // cmd.args[position] = arg;
                    // cmd.input_red.index += 1;
                    line_str.cmds[l].args[position[l]] = arg;
                    line_str.cmds[l].input_red.index += 1;
                    // strcpy(cmd_args[position], arg);
                    // for (int z=0;z<strlen(arg);z++, cmd_args[position]++, arg++)
                    // {
                    //     *cmd_args[position] = *arg;
                    // }
                    // memcpy(cmd_args[position], arg, strlen(arg));
                    position[l]++;
                    // for (int z=0;z<position;z++) printf("%s\n", cmd_args[z]);
                    arg = (char*)malloc(ARG_SIZE*sizeof(char));
                    j = 0;
                    if (position[l] >= buf_size) {
                        buf_size += ARG_SIZE;
                        // cmd.args = realloc(cmd.args,  buf_size* sizeof(char*));
                        line_str.cmds[l].args = realloc(line_str.cmds[l].args,  buf_size* sizeof(char*));
                    }
                }
                continue;
            }
            if (line[i] == '|') {
                cmd.pipe.status = 1;
                line_str.cmds[l].pipe.status = 1;
                line_str.cmds[l].pipe.index = position[l];
                if (strlen(arg) > 0)
                {
                    line_str.cmds[l].args[position[l]] = arg;
                    line_str.cmds[l].pipe.index += 1;
                    // strcpy(cmd_args[position], arg);
                    // for (int z=0;z<strlen(arg);z++, cmd_args[position]++, arg++)
                    // {
                    //     *cmd_args[position] = *arg;
                    // }
                    // memcpy(cmd_args[position], arg, strlen(arg));
                    position[l]++;
                    // for (int z=0;z<position;z++) printf("%s\n", cmd_args[z]);
                    arg = (char*)malloc(ARG_SIZE*sizeof(char));
                    j = 0;
                    if (position[l] >= buf_size) {
                        buf_size += ARG_SIZE;
                        // cmd.args = realloc(cmd.args,  buf_size* sizeof(char*));
                        line_str.cmds[l].args = realloc(line_str.cmds[l].args, buf_size* sizeof(char*));
                    }
                }
                l++;
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
                // cmd.args[position] = arg;
                line_str.cmds[l].args[position[l]] = arg;
                // strcpy(cmd_args[position], arg);
                // for (int z=0;z<strlen(arg);z++, cmd_args[position]++, arg++)
                // {
                //     *cmd_args[position] = *arg;
                // }
                // memcpy(cmd_args[position], arg, strlen(arg));
                position[l]++;
                // for (int z=0;z<position;z++) printf("%s\n", cmd_args[z]);
                arg = (char*)malloc(ARG_SIZE*sizeof(char));
                j = 0;
                if (position[l] >= buf_size) {
                    buf_size += ARG_SIZE;
                    // cmd.args = realloc(cmd.args,  buf_size* sizeof(char*));
                    line_str.cmds[l].args = realloc(line_str.cmds[l].args,  buf_size* sizeof(char*));
                }
            }
        }

    }
    if (strlen(arg) > 0)
    {
        line_str.cmds[l].args[position[l]] = arg;
        position[l]++;
    }
    for(int z=0;z<=l;z++)  {
        // printf("came here");
        line_str.cmds[z].noa = position[z];
    }
    line_str.no_of_cmds = l+1;
    // printf("position -> %d\n", position);
    // for (i=0;i<position[0];i++)
    //     printf("%d  %s\n",line_str.cmds[0].noa, line_str.cmds[0].args[i]);

    
    // if (cmd.output_red.status == 1) printf("index of output redir is %d\n",cmd.output_red.index);
    // if (cmd.input_red.status == 1) printf("index of input redir is %d\n", cmd.input_red.index);
    return line_str;

}

int execute_all(arg_ret cmd, arg_ret cmd_act) {
    int save_stdout;
    int save_stdin;
    int fd;
    pid_t pid;
    // if (flag==1) {
    
    // if(in!=0) {
    //         dup2(in, 0);
    //         close(in);
    //     }
        
    //     if (out!=1) {
    //         dup2(out, 1);
    //         close(out);
    //     } 
    // }
    

    // save_stdout = dup(1);
    // save_stdin = dup(0);

    if (cmd.output_red.status == 1) {
        // save_stdout = dup(1);
        if (cmd.output_red.append == 0)
            fd = open(cmd.args[cmd.output_red.index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else
            fd = open(cmd.args[cmd.output_red.index], O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd == -1) {
            perror("Failed to open file");
            return 0;
        }
        if(dup2(fd, 1) != 1)
            perror("dup2 fail");
    }
    if (cmd.input_red.status == 1) {
        // save_stdin = dup(0);
        fd = open(cmd.args[cmd.input_red.index], O_RDONLY);
        if(fd == -1) {
            perror("sash");
            return 0;
        }
        // printf("fd-> %d\n",fd);
        // return 0;
        
        if(dup2(fd, 0) != 0) {
            perror("dup2 fail");
            // return 0;
        }
        // return 0;
    }

    

    for (int i = 0; i < NO_OF_BUILTINS; i++) {
        if (strcmp(cmd_act.args[0], bltn_cmds[i]) == 0) {
            (*bltn_func[i])(cmd_act);
            if (cmd.output_red.status == 1) {            
                dup2(save_stdout, 1);
                close(fd);
            }
            if (cmd.input_red.status == 1) {            
                dup2(save_stdin, 0);
                close(fd);
            }            
            return 0;
        }
    }

    execute(cmd_act);
    if (cmd.output_red.status == 1) {            
        dup2(save_stdout, 1);
        close(fd);
    }
    if (cmd.input_red.status == 1) {            
        dup2(save_stdin, 0);
        close(fd);
    
}


}



void print_parsed(cmds_ret parsed) {
    for (int i=0;i<parsed.no_of_cmds;i++) {
        printf("cmd -> %d. %s  ",i+1,parsed.cmds[i].args[0]);
        for (int j=1;j<parsed.cmds[i].noa;j++) {
            printf("%s ",parsed.cmds[i].args[j]);
        }
        printf("\n");
    }
}

int process(char* line, int num)
{

    int sav_stdout = dup(1);
    int sav_stdin = dup(0);
    // printf("sin-> %d sout-> %d",sav_stdin, sav_stdout);
    cmds_ret parsed = parse(line);
    // arg_ret* parsed = parse(line);
    if (parsed.no_of_cmds == 0) return -1;
    pid_t pid;
    int fd[2],in, j=0, out;

    in=dup(0);
    // printf("no of commands->%d\n",parsed.no_of_cmds);
    // return 0;
    for (int i=0; i<parsed.no_of_cmds; ++i) {
        // Coreeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeect
        arg_ret cmd = parsed.cmds[i];
        arg_ret cmd_act;
        
        
        cmd_act.background = cmd.background;
        j=0;
        cmd_act.args=(char**)malloc(sizeof(char*)*cmd.noa*200);
        for(int p=0;p<cmd.noa;p++)
        { 
            if (!((cmd.input_red.status!=0 && cmd.input_red.index==p) || (cmd.output_red.status!=0 && cmd.output_red.index==p))) {
                // printf("came here with i=%d, j=%d\n",i,j);
                cmd_act.args[j] = cmd.args[p];
                j++;
            } 
        }
        cmd_act.noa = j;
        //Correeeeeeeeeeeeeeeeeeeeeeeeeect
        // printf("cmd -> %s\n args-> ",cmd_act.args[0]);
        // for (int e=1;e<j;e++) printf("%s ",cmd_act.args[e]);
        // printf("\n");
        // continue;
        // printf("came here with i= %d\n",i);
        dup2(in,0);
        close(in);
        if (i==parsed.no_of_cmds-1)
        {
            out = dup(sav_stdout);
            // printf("last cmd\n");
        }
        else {
            if (pipe(fd)==-1) perror("pipe"); 
            in = fd[0];
            out=fd[1];        
        }
        // printf("came with i= %d\t%d\n",i,in);
        dup2(out, 1);
        close(out);
    
        execute_all(cmd, cmd_act);
        // printf("came i= %d\n",i);
        
    }
    dup2(sav_stdout, 1);
    close(sav_stdout);
    dup2(sav_stdin, 0);
    close(sav_stdin);
    // printf("sdfsdfsfdsdfs\n");
    // if (in!=0) { dup2(in, 0); close(in);}
    
    // arg_ret cmd = parsed.cmds[parsed.no_of_cmds-1];
        // arg_ret cmd_act;

    //     cmd_act.background = cmd.background;
    //     j=0;
    //     cmd_act.args=(char**)malloc(sizeof(char*)*cmd.noa*200);
    //     for(int i=0;i<cmd.noa;i++)
    //     { 
    //         if (!((cmd.input_red.status!=0 && cmd.input_red.index==i) || (cmd.output_red.status!=0 && cmd.output_red.index==i))) {
    //             // printf("came here with i=%d, j=%d\n",i,j);
    //             cmd_act.args[j] = cmd.args[i];
    //             j++;
    //         }
        
    //     }
    // // printf("cmd-> %s\n args->",cmd_act.args[0]);
    // // for (int i=1;i<j;i++) { printf("%s ",cmd_act.args[i]);}
    // // printf("\n");
    // execute_all(cmd, cmd_act);
    // dup2(sav_stdout, 1);
    // close(sav_stdout);
    // dup2(sav_stdin, 0);
    // close(sav_stdin);

    // // free(cmd.args);
    return 0;
}



int main() {
    // sav_stdout = dup(1);
    // sav_stdin = dup(0);
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
    nof_bg_stpd_proc=0;
    keepRunning = 1;

    //Signal Handlers
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP,sigtstp_handler);
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
        if (!keepRunning) {
            keepRunning=1;
            b= (char*)malloc(1);
            b=NULL;
            continue;
        }
        b[noc] = '\0';
        if (b[0] == '\n')  continue;
        if (b[noc-1] == '\n') b[noc-1] = '\0';
        process(b,noc);
        b= (char*)malloc(1);
        b=NULL;
        // dup2(sav_stdout, 1);
    // close(sav_stdout);
    // dup2(sav_stdin, 0);
    // close(sav_stdin);
    }

}