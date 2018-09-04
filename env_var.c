#include "main.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int set_pwd() {
    if ((int)strlen(PWD) != 0)
        strcpy(OLDPWD, PWD);
    if (getcwd(PWD, PATH_MAX) == NULL) {
        // perror("setting pwd error");
        perror("sash");
        // printf("setting pwd errord");
        // print_errno();
        return 1;
   }
}

int set_home() {
    if (getcwd(HOME, PATH_MAX) == NULL) {
        perror("sash");
        // printf("setting pwd errord");
        print_errno();
        return 1;
   }
}

int set_rwd() {
    int n = strlen(HOME);
    int m = strlen(PWD);
    // printf("%d %d", n, m);
    if (n > m) {
        strcpy(RWD, PWD);
        // printf("fgdgdfhgfdjnbj");
        return 0;
    }
    int i;
    for(i=0;i<n;i++)
    {
        if (PWD[i] != HOME[i]) break;
    }
    if (i >= n)
    {
        if (n == m)
        {
            strcpy(RWD,"~");
            return 0;
        }
        if (n < m)
        {
            if (PWD[i] == '/')
            {
                RWD[0] = '~';
                strcpy(&RWD[1], &PWD[i]);
                return 0;
            }
            else {
                strcpy(RWD, PWD);
                return 0;
            }
        }

    }
    else {
        strcpy(RWD, PWD);
        return 0;
    }
}
