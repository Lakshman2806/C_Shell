#include "head.h"

int inout(char **arg, int no, int *out, int *in, int *o, int *id, char *wkrdir, char *strdir)
{
    // char tempdir[1000];
    // memset(tempdir, 0, 1000);
    // strcpy(tempdir, strdir);
    // pwd(strdir, wkrdir);
    // chdir(wkrdir);
    int fl1 = 0, fl2 = 0;
    for (int i = 0; i < no; i++)
    {
        if (!strcmp(arg[i], "<"))
        {
            char tempdir[1000];
            memset(tempdir, 0, 1000);
            strcpy(tempdir, wkrdir);
            pwd(strdir, wkrdir);
            chdir(wkrdir);
            *id = open(arg[i + 1], O_RDONLY);
            chdir(strdir);
            strcpy(wkrdir, tempdir);
            fl1 = 1;
            if (*id < 0)
            {
                fl1 = 0;
                red();
                fprintf(stderr, "File cannot be opened\n");
                clr_rst();
                // strcpy(wkrdir, tempdir);
                // chdir(strdir);
                return 0;
            }
        }
        if (arg[i][0] == '>')
        {
            int fd;
            fl2 = 1;
            if (strlen(arg[i]) == 1)
            {
                char tempdir[1000];
                memset(tempdir, 0, 1000);
                strcpy(tempdir, wkrdir);
                pwd(strdir, wkrdir);
                chdir(wkrdir);
                *o = open(arg[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                chdir(strdir);
                strcpy(wkrdir, tempdir);
            }
            else if (strlen(arg[i]) == 2 && arg[i][1] == '>')
            {
                char tempdir[1000];
                memset(tempdir, 0, 1000);
                strcpy(tempdir, wkrdir);
                pwd(strdir, wkrdir);
                chdir(wkrdir);
                *o = open(arg[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                chdir(strdir);
                strcpy(wkrdir, tempdir);
            }
            else
            {
                red();
                fprintf(stderr, "Wrong command\n");
                clr_rst();
                // strcpy(wkrdir, tempdir);
                // chdir(strdir);
                return 0;
            }
            if (*o < 0)
            {
                red();
                fprintf(stderr, "File cannot be opened\n");
                clr_rst();
                // strcpy(wkrdir, tempdir);
                // chdir(strdir);
                return 0;
            }
        }
    }
    if (fl2 || *o != 0)
    {
        *out = dup(STDOUT_FILENO);

        dup2(*o, 1);
    }
    if (fl1 || *id != 0)
    {
        *in = dup(STDIN_FILENO);

        dup2(*id, 0);
    }
    // strcpy(wkrdir, tempdir);
    // chdir(strdir);
    return 1;
}

void back(int out, int in, int o, int i)
{
    if (in && i)
    {
        dup2(in, STDIN_FILENO);
        close(in);
        close(i);
    }
    if (out && o)
    {
        dup2(out, STDOUT_FILENO);
        close(out);
        close(o);
    }
    // close(in);
    // close(out);
    // close(i);

    // close(o);
}