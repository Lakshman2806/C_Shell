#include "cd.h"
#include "head.h"

void rev(char *temp, char *wrkdir)
{
    for (int i = strlen(wrkdir) - 1; i >= 0; i--)
    {
        char t[2];
        t[0] = wrkdir[i];
        t[1] = '\0';
        strcat(temp, t);
    }
}

void change(char *wrkdir, char *strdir)
{
    char temp[strlen(wrkdir) + 10];
    temp[0] = 0;
    for (int i = 0; i < strlen(strdir); i++)
    {
        if (wrkdir[i] != strdir[i])
        {
            return;
        }
    }
    strcat(temp, "~/");
    temp[strlen(temp)] = '\0';
    for (int i = strlen(strdir) + 1; i < strlen(wrkdir); i++)
    {
        char t[2];
        t[0] = wrkdir[i];
        t[1] = 0;
        strcat(temp, t);
    }
    if (strcmp(temp, "~/"))
        strcpy(wrkdir, temp);
    else
        strcpy(wrkdir, "~");
}

void fix(char *wrkdir, char *strdir, int flag)
{
    char temp[strlen(wrkdir) + 10 + strlen(strdir)];
    temp[0] = '\0';
    char fin[strlen(wrkdir) + 10 + strlen(strdir)];
    fin[0] = '\0';
    if (flag)
        pwd(strdir, wrkdir);
    for (int i = strlen(wrkdir) - 1; i >= 0; i--)
    {
        char t[2];
        t[0] = wrkdir[i];
        t[1] = '\0';
        strcat(temp, t);
    }
    char *token = strtok(temp, "/");
    int c = 0;
    while (token != NULL)
    {
        if (!strcmp(token, ".."))
        {
            c++;
        }
        else
        {
            if (c)
            {
                c--;
            }
            else
            {
                strcat(fin, token);
                char t[2];
                t[0] = '/';
                t[1] = '\0';
                strcat(fin, t);
            }
        }
        token = strtok(NULL, "/");
    }
    strcpy(wrkdir, "\0");
    int len = strlen(fin);
    len--;
    if (fin[strlen(fin)] == '~')
    {
        len--;
    }

    for (int i = len; i >= 0; i--)
    {
        char t[2];
        t[0] = fin[i];
        t[1] = '\0';
        strcat(wrkdir, t);
    }
    wrkdir[len + 1] = '\0';
}
void cd(char **arg, int no, char *wrkdir, char *strdir, char *prevdir)
{
    if (no > 2)
    {
        red();
        printf("cd does not take more than two arguments\n");
        return;
    }
    if (no == 1)
    {
        strcpy(wrkdir, "~");
        return;
    }
    // if (arg[1] == "..") dumb as fuck sorry hehe
    if (!strcmp(arg[1], "/"))
    {
        strcpy(prevdir, wrkdir);
        strcpy(wrkdir, "/");
        return;
    }
    if (arg[1][strlen(arg[1]) - 1] == '/')
        arg[1][strlen(arg[1]) - 1] = 0;
    if (!strcmp(arg[1], ".."))
    {
        if (!strcmp(wrkdir, "~"))
        {
            strcpy(prevdir, wrkdir);

            strcpy(wrkdir, strdir);
            int len = strlen(wrkdir);
            for (int i = len - 1; i >= 0; i--)
            {
                if (wrkdir[i] == '/')
                {
                    wrkdir[i] = 0;
                    return;
                }
            }
        }
        else
        {
            // if (!strcmp(wrkdir, "/home"))
            // {
            //     red();
            //     printf("Sorry can not go further :(\n");
            //     return;
            // }

            int len = strlen(wrkdir);
            for (int i = len - 1; i >= 0; i--)
            {
                if (wrkdir[i] == '/')
                {
                    if (i != 0)
                    {
                        strcpy(prevdir, wrkdir);
                        wrkdir[i] = 0;
                        return;
                    }
                    else
                    {
                        // red();
                        // printf("Sorry can not go further :(\n");
                        strcpy(prevdir, wrkdir);
                        wrkdir[1] = 0;
                        return;
                    }
                }
            }
        }
        return;
    }
    else if (!strcmp(arg[1], "~"))
    {
        strcpy(prevdir, wrkdir);
        strcpy(wrkdir, "~");
        return;
    }
    else if (!strcmp(arg[1], "."))
    {
        strcpy(prevdir, wrkdir);
        return;
    }
    else if (!strcmp(arg[1], "-"))
    {
        strcpy(arg[1], prevdir);
        printf("%s\n", prevdir);
        cd(arg, no, wrkdir, strdir, prevdir);
        return;
    }
    else
    {
        char *path;
        char temp[1000];
        strcpy(temp, wrkdir);
        path = (char *)calloc(1000, sizeof(char));
        if (strncmp(arg[1], "/", 1))
        {
            if (wrkdir[0] == '~')
            {
            there:
                int str = 0;
                int len = strlen(arg[1]);
                if (arg[1][0] == '~')
                {
                    strcpy(wrkdir, "~");
                    str = 2;
                    len = len - 2;
                }
                if (strcmp(wrkdir, "~"))
                {
                    for (int i = 2; i < strlen(wrkdir); i++)
                    {
                        char t[2];
                        t[0] = wrkdir[i];
                        t[1] = '\0';
                        strcat(path, t);
                    }
                    strcat(path, "/");
                }
                strncat(path, &arg[1][str], len);

                DIR *dir = opendir(path);
                if (dir)
                {
                    strcpy(prevdir, temp);
                    closedir(dir);
                    strcat(wrkdir, "/");
                    strncat(wrkdir, &arg[1][str], len);
                    fix(wrkdir, strdir, 1);
                    // printf("%s\n", wrkdir);
                    change(wrkdir, strdir);
                }
                else if (ENOENT == errno)
                {
                    red();
                    strcpy(wrkdir, temp);
                    printf("Directory does not exist\n");
                }
                else
                {
                    red();
                    printf("Error, mostly not enough permissions\n");
                }
            }
            else
            {
                if (arg[1][0] == '~')
                {
                    goto there;
                }
                int st = 0, l;
                int count = 1;
                for (int i = 0; i < strlen(strdir); i++)
                {
                    if (i >= strlen(wrkdir) && strlen(wrkdir) != 1)
                    {
                        count--;
                    }
                    if (i >= strlen(wrkdir) || strdir[i] != wrkdir[i])
                    {
                        st = i;
                        break;
                    }
                }
                l = st;
                while (st < strlen(strdir))
                {
                    if (strdir[st++] == '/')
                        count++;
                }
                while (count--)
                {
                    strcat(path, "../");
                }
                for (int i = l; i < strlen(wrkdir); i++)
                {
                    if (wrkdir[l] != '/')
                    {
                        char t[2];
                        t[0] = wrkdir[i];
                        t[1] = '\0';
                        strcat(path, t);
                    }
                }
                strcat(path, "/");
                strcat(path, arg[1]);
                DIR *dir = opendir(path);
                if (dir)
                {
                    strcpy(prevdir, temp);
                    closedir(dir);
                    strcat(wrkdir, "/");
                    strcat(wrkdir, arg[1]);
                    // strcpy(wrkdir, arg[1]);
                    fix(wrkdir, strdir, 1);
                    // printf("%s\n", wrkdir);
                    change(wrkdir, strdir);
                }
                else if (ENOENT == errno)
                {
                    red();
                    strcpy(wrkdir, temp);

                    printf("Directory does not exist\n");
                }
                else
                {
                    red();
                    strcpy(wrkdir, temp);

                    printf("Error, mostly not enough permissions\n");
                }
            }
        }
        else
        {
            int count = 0;
            for (int i = 0; i < strlen(strdir); i++)
            {
                if (strdir[i] == '/')
                    count++;
            }
            while (count--)
            {
                strcat(path, "../");
            }
            strcat(path, "/");
            strcat(path, arg[1]);
            DIR *dir = opendir(path);
            if (dir)
            {
                strcpy(prevdir, temp);
                closedir(dir);
                // strcat(wrkdir, "/");
                // strcat(wrkdir, arg[1]);
                strcpy(wrkdir, arg[1]);
                fix(wrkdir, strdir, 0);
                // printf("%s\n", wrkdir);
                change(wrkdir, strdir);
            }
            else if (ENOENT == errno)
            {
                red();
                strcpy(wrkdir, temp);

                printf("Directory does not exist\n");
            }
            else
            {
                red();
                strcpy(wrkdir, temp);

                printf("Error, mostly not enough permissions\n");
            }
        }
        free(path);
        return;
    }
}

void Mkdir(char **arg, char *wrkdir)
{
    char *path;
    path = (char *)calloc(strlen(arg[1]) + 20, sizeof(char));
    int st = 0;
    int len = strlen(arg[1]);
    if (arg[1][0] == '~')
    {
        st = 2;
        len = len - 2;
    }
    if (strcmp(wrkdir, "~"))
    {
        for (int i = 2; i < strlen(wrkdir); i++)
        {
            char t[2];
            t[0] = wrkdir[i];
            t[1] = '\0';
            strcat(path, t);
        }
        strcat(path, "/");
    }
    strncat(path, &arg[1][st], len);

    DIR *dir = opendir(path);
    if (dir)
    {
        closedir(dir);
        red();
        printf("Directory already exists\n");
    }
    else if (ENOENT == errno)
    {
        mkdir(path, 0777);
    }
}