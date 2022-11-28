#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "echo.h"
char *init()
{
    char *cmd;
    cmd = (char *)calloc(50, sizeof(char));

    return cmd;
}
void cmddel(char **x)
{
    if (strcmp(*x, ""))
        free(*x);
}
void del(char ***arg, int no)
{
    for (int i = 0; i < no; i++)
    {
        free((*arg)[i]);
    }
    free(*arg);
}

void nicecmd(char **cmd)
{
    int st = 0;
    while ((*cmd)[st] == ' ' || (*cmd)[st] == '\t')
    {
        st++;
    }
    (*cmd) = &(*cmd)[st];
    int end = strlen(*cmd) - 1;
    int f = 0;
    while (end >= 0 && ((*cmd)[end] == ' ' || (*cmd)[end] == '\t'))
    {
        f = 1;
        (*cmd)[end] = 0;
        end--;
    }
    // if (f)
    //     (*cmd)[end + 1] = '\n';
}

int cmd_parse(char *cmd, char ***arg)
{
    int no = 0;
    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = '\0';
    int len = strlen(cmd);
    for (int i = 0; i < len;)
    {

        while ((cmd[i] == ' ' || cmd[i] == '\t') && i < len)
            i++;
        (*arg)[no] = (char *)calloc(50, sizeof(char));
        int flag = 0;
        while ((cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '<' && cmd[i] != '>') && i < len)
        {
            // if (cmd[i] != 34)
            // {
            char t[2];
            t[0] = cmd[i++];
            t[1] = '\0';
            strcat((*arg)[no], t);

            // else
            // {
            //     int c = 0;
            //     i++;
            //     while (cmd[i] != 34 && i < len)
            //     {
            //         (*arg)[no][c++] = cmd[i++];
            //     }
            //     i++;
            //     break;
            // }
        }

        int len = strlen((*arg)[no]) - 1;
        if ((*arg)[no][len] == '\n')
            (*arg)[no][len] = 0;
        if ((*arg)[no][0] != 0)
            no++;
        if (cmd[i] == '>')
        {
            (*arg)[no] = (char *)calloc(5, sizeof(char));
            if (cmd[i + 1] == '>')
            {
                strcpy((*arg)[no], ">>");
                no++;
                i += 2;
            }
            else
            {
                strcpy((*arg)[no], ">");
                no++;
                i++;
            }
        }
        else if (cmd[i] == '<')
        {
            (*arg)[no] = (char *)calloc(5, sizeof(char));
            strcpy((*arg)[no], "<");
            no++;
            i++;
        }
        while ((cmd[i] == ' ' || cmd[i] == '\t') && i < len)
            i++;
    }
    return no;
}