#include "pinfo.h"
#include "head.h"
#include <unistd.h>
void pinfo(char **arg, int no, char *wrkdir, char *strdir)
{
    char dir[1000] = "/proc/";
    char file[1000] = "";
    if (no > 2)
    {
        red();
        printf("Wrong usage of pinfo\n");
        clr_rst();
        return;
    }
    else
    {
        long int pid;
        if (no == 1)
        {
            pid = getpid();
            strcat(dir, "self");
        }
        else
        {
            if (!atol(arg[1]))
            {
                red();
                printf("Wrong usage of pinfo\n");
                clr_rst();
                return;
            }
            pid = atol(arg[1]);
            strcat(dir, arg[1]);
        }
        strcpy(file, dir);
        strcat(file, "/stat");
        FILE *f = fopen(file, "r");
        char status;
        long long int pgrp, tpgid, mem;

        if (f)
        {
            fscanf(f, "%*s %*s %c %*s %lld %*s %*s %lld %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %lld",
                   &status, &pgrp, &tpgid, &mem);

            printf("pid : %ld\n", pid);
            printf("Status: %c", status);
            if (pgrp == tpgid)
            {
                printf("+");
            }
            printf("\n");
            printf("Memory: %lld\n", mem);
        }
        else
        {
            red();
            printf("Error\n");
            clr_rst();
            return;
        }
        fclose(f);
        strcpy(file, dir);
        strcat(file, "/exe");
        char path[1000];
        strcpy(path, wrkdir);
        int len = readlink(file, wrkdir, 999);
        if (len >= 0)
        {
            change(wrkdir, strdir);
            printf("Executable path: %s\n", wrkdir);
            strcpy(wrkdir, path);
        }
        else
        {
            return;
        }
    }
}