#ifndef _JOBS_H_
#define _JOBS_H_

struct bgstate
{
    int in;
    long int pid;
    char state[30];
    char comm[1000];
};
typedef struct bgstate BgState;

void jobs(char **arg, int no, char bgcomm[100][1000], long int pid[1000], int ind[1000]);

#endif