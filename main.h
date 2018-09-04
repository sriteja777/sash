#ifndef SHELL_GLOBAL_INSTANCES
#define SHELL_GLOBAL_INSTANCES 7


#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_DARK_YELLOW  "\x1b[1;33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_DARK_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_RESET        "\x1b[0m"
#define ARGS_DELIMITOR          " \t\n\r\a"
#define NO_OF_BUILTINS          7
#define ARG_SIZE                100
#define PATH_MAX				4096
#define EXIT_PROMPT				"\x1b[36mGood Bye\n\x1b[0m"
#define MAX_BACKGROUND_PROCESS	100

int nof_bg_proc;
int job_number;
struct background_process
{
	int pid;
	char* pname;
	int pstatus;
	int pjob_num;
} bg_proc[100];

//Declare struct for command
typedef struct
{
	char **args;
	int noa;
	int background;
} arg_ret;

//Declare Builtin command functions
int bltn_cd(arg_ret cmd);
int bltn_echo(arg_ret cmd);
int bltn_pwd(arg_ret cmd);
int bltn_exit(arg_ret cmd);
int bltn_ls(arg_ret cmd);
int bltn_pinfo(arg_ret cmd);
int bltn_jobs(arg_ret cmd);

void print_errno();

//Declare Environment variable functions
int set_pwd();
int set_rwd();
int set_home();


void jobs();

//Functions for other commands
int execute(arg_ret cmd);

// Declare Environment Varables
char PWD[PATH_MAX];
char HOME[PATH_MAX];
char RWD[PATH_MAX];
char PROMPT[100];
char OLDPWD[PATH_MAX];

#endif