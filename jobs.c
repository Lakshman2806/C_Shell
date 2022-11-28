#include "head.h"

static int strCompare(const void *a, const void *b)
{
    const BgState *x = a;
    const BgState *y = b;
    return strcmp(x->comm, y->comm);
}

char jpinfo(char p[])
{
    char dir[1000] = "/proc/";
    char file[1000] = "";

    strcat(dir, p);

    strcpy(file, dir);
    strcat(file, "/stat");
    FILE *f = fopen(file, "r");
    char status;
    long long int pgrp, tpgid, mem;

    if (f)
    {
        fscanf(f, "%*s %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lld",
               &status, &pgrp, &tpgid, &mem);

        // printf("pid : %ld\n", pid);
        // printf("Status: %c", status);
        if (pgrp == tpgid)
        {
            // printf("+");
        }
        return status;
        // printf("\n");
        // printf("Memory: %lld\n", mem);
    }
    else
    {
        red();
        printf("Error\n");
        clr_rst();
        return ' ';
    }
}

void jobs(char **arg, int no, char bgcomm[100][1000], long int pid[1000], int ind[1000])
{
    int sf = 0, rf = 0;
    if (no > 4)
    {
        red();
        perror("Error in command\n");
        clr_rst();
        return;
    }
    for (int i = 1; i < no; i++)
    {
        if (arg[i][0] == '-')
        {
            if (strlen(arg[i]) > 2)
            {
                red();
                perror("Error in flags\n");
                clr_rst();
                return;
            }
            if (arg[i][1] == 's')
                sf = 1;
            else if (arg[i][1] == 'r')
                rf = 1;
            else
            {
                red();
                perror("Error in flags\n");
                clr_rst();
                return;
            }
        }
    }
    if (!rf && !sf)
    {
        rf = 1;
        sf = 1;
    }
    BgState A[100];
    int c = 0;
    for (int i = 0; i < 100; i++)
    {
        if (pid[i])
        {
            char x[20];
            sprintf(x, "%ld", pid[i]);
            char status = jpinfo(x);
            if (status == 'S')
            {
                sprintf(A[c].state, "Running");
            }
            else if (status == 'R')
            {
                sprintf(A[c].state, "Running");
            }
            else if (status == 'T')
            {
                sprintf(A[c].state, "Stopped");
            }
            else
            {
                perror("Error\n");
                return;
            }
            if (!strcmp(A[c].state, "Stopped") && sf)
            {
                A[c].pid = pid[i];
                A[c].in = ind[i];
                strcpy(A[c].comm, bgcomm[i]);
                c++;
            }
            else if (!strcmp(A[c].state, "Running") && rf)
            {
                A[c].pid = pid[i];
                A[c].in = ind[i];
                strcpy(A[c].comm, bgcomm[i]);
                c++;
            }
        }
    }
    qsort(A, c, sizeof(const BgState), strCompare);
    for (int i = 0; i < c; i++)
    {
        printf("[%d] %s %s [%ld]\n", A[i].in, A[i].state, A[i].comm, A[i].pid);
    }
}