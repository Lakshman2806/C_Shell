#include "head.h"

void real(char *ll, int df, int ff, char *rdir, int tf, char *target)
{
    DIR *dir;
    struct dirent *file;
    struct stat lstat;
    dir = opendir(ll);
    if (EACCES == errno)
    {
        printf("Not enough permissions\n");
        free(file);
        return;
    }
    file = readdir(dir);
    while (file != NULL)
    {
        if (file->d_name[0] != '.')
        {
            char b[1000];
            sprintf(b, "%s/%s", ll, file->d_name);
            stat(b, &lstat);
            char cpy[strlen(b) + 10];
            char rcpy[strlen(rdir) + 10];
            strcpy(cpy, b);
            cpy[strlen(b)] = 0;
            rcpy[strlen(rdir)] = 0;

            if (S_ISDIR(lstat.st_mode))
            {
                if (df)
                {
                    if (!tf)
                    {
                        printf("%s/", rdir);
                        for (int i = 2; i <= strlen(b); i++)
                            printf("%c", b[i]);
                        printf("\n");
                    }
                    else
                    {
                        if (!strcmp(file->d_name, target))
                        {
                            printf("%s/", rdir);
                            for (int i = 2; i <= strlen(b); i++)
                                printf("%c", b[i]);
                            printf("\n");
                        }
                    }
                }
                real(b, df, ff, rdir, tf, target);
                strcpy(b, cpy);
                b[strlen(cpy)] = 0;
            }
            else
            {
                if (ff)
                {
                    if (!tf)
                    {
                        printf("%s/", rdir);
                        for (int i = 2; i <= strlen(b); i++)
                            printf("%c", b[i]);
                        printf("\n");
                    }
                    else
                    {
                        if (!strcmp(file->d_name, target))
                        {
                            printf("%s/", rdir);
                            for (int i = 2; i <= strlen(b); i++)
                                printf("%c", b[i]);
                            printf("\n");
                        }
                    }
                }
            }
        }
        file = readdir(dir);
    }
    free(file);
}

void discover(char *strdir, char *wrkdir, char **arg, int no)
{
    int df = 0;
    int ff = 0;
    int tf = 0;
    int gdf = 0;
    char target[1000];
    for (int i = 1; i < no; i++)
    {
        if (arg[i][0] == '-')
        {
            if (strlen(arg[i]) == 2)
            {
                if (arg[i][1] == 'd')
                    df = 1;
                else if (arg[i][1] == 'f')
                    ff = 1;
                else
                {
                    red();
                    printf("Invalid flag given \n");
                    clr_rst();
                    return;
                }
            }
        }
        else if (arg[i][0] == 34)
        {
            if (!tf)
            {
                if (arg[i][strlen(arg[i]) - 1] != 34)
                {
                    red();
                    printf("Quotations not closed\n");
                    clr_rst();
                    return;
                }
                else
                {
                    strncpy(target, &arg[i][1], strlen(arg[i]) - 2);
                }
                tf = 1;
            }
            else
            {
                red();
                printf("Two Targets specified\n");
                clr_rst();
                return;
            }
        }
        else
        {
            if (!gdf)
            {
                gdf = i;
            }
            else
            {
                red();
                printf("Two/more potential directories specified\n");
                clr_rst();
                return;
            }
        }
    }
    if (!df && !ff)
    {
        df = 1;
        ff = 1;
    }
    char temp[1000];
    strcpy(temp, wrkdir);
    temp[strlen(wrkdir)] = 0;
    pwd(strdir, wrkdir);
    chdir(wrkdir);

    char ll[2000] = "";
    if (gdf)
    {
        if (arg[gdf][0] == '~')
        {
            strcpy(ll, strdir);
            if (strcmp(arg[gdf], "~"))
                strncat(ll, &arg[gdf][1], strlen(arg[gdf]) - 1);
            ll[strlen(ll)] = 0;
        }
        else
        {
            strcpy(ll, arg[gdf]);
            ll[strlen(arg[gdf])] = 0;
        }
    }
    else
    {
        strcpy(ll, ".");
    }
    if (chdir(ll))
    {
        red();
        printf("Directory does not exist\n");
        clr_rst();
    }
    else
    {
        if (df && !tf)
        {
            if (gdf)
                printf("%s\n", arg[gdf]);
            else
                printf(".\n");
        }
        if (gdf)
            real(".", df, ff, arg[gdf], tf, target);
        else
            real(".", df, ff, ".", tf, target);
    }
    chdir(strdir);
    strcpy(wrkdir, temp);
    wrkdir[strlen(temp)] = 0;

    return;
}