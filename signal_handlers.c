#include<signal.h>
#include "main.h"
#include<stdio.h>

void sigint_handler(int a) {
    
    keepRunning = 0;
    signal(SIGINT, sigint_handler);
    // printf("\n");
}

void sigtstp_handler(int a) {
    

    return;
}
