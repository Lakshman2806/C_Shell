#include "head.h"

// // void die(const char *s)
// {
//     perror(s);
//     exit(1);
// }

void chesub(char *a, char *b, char *f)
{
    int min = strlen(a) > strlen(b) ? strlen(b) : strlen(a);
    char latemp[1000];
    memset(latemp, 0, 1000);
    for (int i = 0; i < min; i++)
    {
        if (a[i] != b[i])
            break;
        else
        {
            char et[2];
            et[0] = a[i];
            et[1] = 0;
            strcat(latemp, et);
        }
    }
    strcpy(f, latemp);
}

int this(char *rdir, char *f, char *tcmd)
{
    DIR *dir;
    struct dirent *file;
    struct stat lstat;

    char lacheck[1000];
    memset(lacheck, 0, 1000);
    strcpy(lacheck, f);
    dir = opendir(rdir);
    if (EACCES == errno)
    {
        printf("Not enough permissions\n");
        free(file);
        return 0;
    }
    file = readdir(dir);
    int dirf = 0;
    int auflag = 0;
    char first[1000];
    memset(first, 0, 1000);
    while (file != NULL)
    {
        char b[1000];
        memset(b, 0, 1000);
        sprintf(b, "%s/%s", rdir, file->d_name);
        stat(b, &lstat);
        if (!strncmp(f, file->d_name, strlen(f)))
        {
            if (!auflag)
            {
                auflag += 1;
                if (S_ISDIR(lstat.st_mode))
                    dirf = 1;
                strcpy(first, file->d_name);
            }
            else
            {
                if (auflag == 1)
                {
                    printf("\n");
                    printf("%s\n", first);
                }
                chesub(first, file->d_name, lacheck);
                auflag++;
                printf("%s\n", file->d_name);
            }
        }
        file = readdir(dir);
    }
    if (auflag == 1)
    {
        char app[1000];
        memset(app, 0, 1000);
        int plapp = 0;
        for (int i = strlen(f); i < strlen(first); i++)
        {
            printf("%c", first[i]);
            app[plapp++] = first[i];
        }
        strcat(tcmd, app);

        if (dirf)
        {
            printf("/");
            strcat(tcmd, "/");
        }
        else
        {
            printf(" ");
            strcat(tcmd, " ");
        }
    }
    else if (auflag > 1)
    {
        if (strcmp(lacheck, f))
        {
            char app[1000];
            memset(app, 0, 1000);
            int plapp = 0;
            for (int i = strlen(f); i < strlen(lacheck); i++)
            {
                // printf("%c", first[i]);
                app[plapp++] = first[i];
            }
            strcat(tcmd, app);
        }
    }
    return auflag;
}

int autocmp(char *tcmd, char *wrkdir, char *strdir, int tlen)
{
    char temp[1000];
    strcpy(temp, wrkdir);
    pwd(strdir, wrkdir);
    chdir(wrkdir);
    char rev[tlen], or [tlen];
    memset(rev, 0, tlen);
    memset(or, 0, tlen);
    // strcpy(or, "");
    int rlen = 0;
    for (int i = tlen - 1; i >= 0; i--)
    {
        if (tcmd[i] == ' ')
            break;

        else
        {
            rev[rlen++] = tcmd[i];
        }
    }
    if (rlen == 0)
    {
        chdir(strdir);
        strcpy(wrkdir, temp);
        return 0;
    }
    int orlen = 0;
    // for (int i = rlen - 1; i >= 0; i--)
    // {
    //     or [orlen++] = rev[i];
    // }
    char dir[rlen], f[rlen];
    memset(dir, 0, rlen);
    memset(f, 0, rlen);

    int fi = 0;
    for (int i = 0; i < rlen; i++)
    {
        if (rev[i] != '/')
        {
            or [orlen++] = rev[i];
        }
        else
        {
            fi = i + 1;
            break;
        }
    }
    for (int i = 0; i < orlen; i++)
    {
        f[i] = or [orlen - 1 - i];
    }
    int x = 0;
    for (int i = rlen - 1; i >= fi; i--)
    {
        dir[x++] = rev[i];
    }

    int retval;
    if (fi == 0)
    {
        retval = this(".", f, tcmd);
    }
    else
    {
        retval = this(dir, f, tcmd);
    }
    chdir(strdir);
    strcpy(wrkdir, temp);
    return retval;
}
