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
#define NO_OF_BUILTINS          13
#define ARG_SIZE                100
#define PATH_MAX				4096
#define EXIT_PROMPT				"\x1b[36mGood Bye\n\x1b[0m"
#define MAX_BACKGROUND_PROCESS	100
#define MAX_NO_OF_PIPES			10
int nof_bg_proc;
int nof_bg_stpd_proc;
int job_number;
int cur_fgnd_proc;
static volatile int keepRunning;
typedef struct background_process
{
	int pid;
	char* pname;
	char* pstat;
	int pstatus;
	int pjob_num;
} jobs_s;

jobs_s bg_proc[100];
jobs_s stpd_process[100];
// jobs_s sspnd_proc[100];

typedef struct {
	int status;
	int index;
	int append;
	char filename[PATH_MAX];
} op_s;

typedef struct {
	int status;
	int index;
	char filename[PATH_MAX];
} ip_s;

typedef struct {
	int status;
	int index;
} pipe_s;


//Declare struct for command
typedef struct
{
	char **args;
	int noa;
	int background;
	op_s output_red;
	ip_s input_red;
	pipe_s pipe;
} arg_ret;

typedef struct 
{
	arg_ret *cmds;
	int no_of_cmds;
} cmds_ret;



//Declare Builtin command functions
int bltn_cd(arg_ret cmd);
int bltn_echo(arg_ret cmd);
int bltn_pwd(arg_ret cmd);
int bltn_exit(arg_ret cmd);
int bltn_ls(arg_ret cmd);
int bltn_pinfo(arg_ret cmd);
int bltn_jobs(arg_ret cmd);
int bltn_setenv(arg_ret cmd);
int bltn_unsetenv(arg_ret cmd);
int bltn_kjob(arg_ret cmd);
int bltn_overkill(arg_ret cmd);
int bltn_fg(arg_ret cmd);
int bltn_bg(arg_ret cmd);
void print_errno();

//Declare signal handlers
void sigint_handler(int a);
void sigtstp_handler(int a);

//Declare Environment variable functions
int set_pwd();
int set_rwd();
int set_home();
// int sav_stdout;
// int sav_stdin;

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
