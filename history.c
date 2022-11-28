#include "head.h"

void nicehcmd(char **cmd)
{
    int end = strlen(*cmd) - 1;
    int f = 0;
    while (end >= 0 && ((*cmd)[end] == ' ' || (*cmd)[end] == '\t'))
    {
        f = 1;
        end--;
    }
    if (f)
        (*cmd)[end + 1] = 0;
}

void addHistory(char *tcmd)
{
    FILE *f;
    char *cmd;
    cmd = calloc(sizeof(char), 1000);
    strcpy(cmd, tcmd);
    if (cmd[strlen(cmd) - 1] == '\n')
        cmd[strlen(cmd) - 1] = 0;
    nicehcmd(&cmd);
    if (!strcmp(cmd, ""))
        return;
    if (!strcmp(cmd, "\n"))
        return;
    f = fopen("Shellhistory", "ab+");
    fclose(f);
    f = fopen("Shellhistory", "r+");
    size_t sz = 1000;
    char **hcmd;
    hcmd = (char **)malloc(sizeof(char *) * 1000);

    int c = 0;
    hcmd[c] = init();
    fgets(hcmd[c++], 1000, f);
    while (strcmp(hcmd[c - 1], ""))
    {
        if (hcmd[c - 1][strlen(hcmd[c - 1]) - 1] == '\n')
            hcmd[c - 1][strlen(hcmd[c - 1]) - 1] = 0;
        hcmd[c] = init();
        fgets(hcmd[c++], 1000, f);
    }
    c--;
    if (c < 20)
    {

        if (c == 0 || (strncmp(cmd, hcmd[c - 1], 1000) && strcmp(cmd, "\n")))
        {
            fprintf(f, "%s", cmd);
            if (cmd[strlen(cmd) - 1] != '\n')
                fprintf(f, "\n");
        }
        fclose(f);
        free(cmd);
        del(&hcmd, c);
        return;
    }
    else
    {
        fclose(f);
        if (!strncmp(cmd, hcmd[c - 1], 1000))
        {
            del(&hcmd, c);
            free(cmd);

            return;
        }
        f = fopen("Shellhistory", "w");
        for (int i = 1; i < 20; i++)
        {
            fprintf(f, "%s\n", hcmd[i]);
        }
        if (strcmp(cmd, "\n"))
            fprintf(f, "%s", cmd);
        if (cmd[strlen(cmd) - 1] != '\n')
            fprintf(f, "\n");
        del(&hcmd, c);
        free(cmd);

        fclose(f);
        return;
    }
}

void History(int m)
{
    if (m > 20)
    {
        red();
        printf("Cannot store more than 20 commands ;( \n");
        clr_rst();
        return;
    }
    else
    {
        FILE *f;

        f = fopen("Shellhistory", "r");
        size_t sz = 1000;
        char **hcmd;
        hcmd = (char **)malloc(sizeof(char *) * 1000);

        int c = 0;
        hcmd[c] = init();
        fgets(hcmd[c++], 1000, f);
        while (strcmp(hcmd[c - 1], ""))
        {
            if (hcmd[c - 1][strlen(hcmd[c - 1]) - 1] == '\n')
                hcmd[c - 1][strlen(hcmd[c - 1]) - 1] = 0;
            hcmd[c] = init();
            fgets(hcmd[c++], 1000, f);
        }
        c--;
        if (c < m)
        {
            red();
            printf("Not enough history :( \n");
            clr_rst();
            del(&hcmd, c);
            return;
        }
        else
        {
            c--;
            while (m--)
            {
                printf("%s\n", hcmd[c--]);
            }
            del(&hcmd, c);

            return;
        }
    }
}