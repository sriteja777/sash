main: main.c bltn.c env_var.c
	gcc bltn.c main.c env_var.c execute.c jobs.c