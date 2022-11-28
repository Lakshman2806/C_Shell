#include "head.h"

void fgHandle(int ind, long int pid[], int *bgc)
{
    if (!pid[ind - 1])
        return;
    int status;
    long int x = pid[ind - 1];
    setpgid(x, getpgid(0));
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    kill(x, SIGCONT);
    waitpid(x, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    pid[ind - 1] = 0;
    (*bgc)--;
}