#include "head.h"

File f[2000];

static int strCompare(const void *a, const void *b)
{
    const File *la = a;
    const File *lb = b;
    char *pa;
    char *pb;
    char fa[strlen(la->name)];
    char fb[strlen(lb->name)];
    char ta[strlen(la->name)];
    char tb[strlen(lb->name)];
    strcpy(ta, la->name);
    strcpy(tb, lb->name);
    strcpy(fa, "");
    strcpy(fb, "");
    pa = strtok(ta, ".");
    while (pa != NULL)
    {
        strcat(fa, pa);
        pa = strtok(NULL, ".");
    }
    pb = strtok(tb, ".");
    while (pb != NULL)
    {
        strcat(fb, pb);
        pb = strtok(NULL, "."); 
    }
    return strcasecmp(fa, fb);
}

void F_ls(char *path, int af, int lf, char *ls, char *rp)
{
    DIR *dir;
    struct dirent *file;
    struct stat lstat;
    dir = opendir(path);
    file = readdir(dir);
    int c = 0;
    char mons[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    while (file != NULL)
    {
        if (!strcmp(file->d_name, ls))
        {
            char b[1000];
            File f;
            sprintf(b, "%s/%s", path, file->d_name);
            stat(b, &lstat);
            f.df = 0;
            f.xf = 0;
            f.Prop[0] = S_ISDIR(lstat.st_mode) ? 'd' : '-';
            if (f.Prop[0] == 'd')
                f.df = 1;

            mode_t owner, group, other;
            owner = lstat.st_mode & S_IRWXU;
            group = lstat.st_mode & S_IRWXG;
            other = lstat.st_mode & S_IRWXO;
            f.Prop[1] = owner & S_IRUSR ? 'r' : '-';
            f.Prop[2] = owner & S_IWUSR ? 'w' : '-';
            f.Prop[3] = owner & S_IXUSR ? 'x' : '-';

            f.Prop[4] = group & S_IRGRP ? 'r' : '-';
            f.Prop[5] = group & S_IWGRP ? 'w' : '-';
            f.Prop[6] = group & S_IXGRP ? 'x' : '-';

            f.Prop[7] = other & S_IROTH ? 'r' : '-';
            f.Prop[8] = other & S_IWOTH ? 'w' : '-';
            f.Prop[9] = other & S_IXOTH ? 'x' : '-';

            if (f.Prop[3] == 'x' || f.Prop[6] == 'x' || f.Prop[9] == 'x')
            {
                f.xf = 1;
            }

            strcpy(f.name, file->d_name);
            strcpy(f.user, getpwuid(lstat.st_uid)->pw_name);
            strcpy(f.group, getgrgid(lstat.st_gid)->gr_name);
            f.Date = lstat.st_ctime;
            f.hrdlink = lstat.st_nlink;
            f.size = lstat.st_size;
            if (lf)
            {
                printf("%s %ld %s %s %ld ", f.Prop, f.hrdlink, f.user, f.group, f.size);
                struct tm *ft = localtime(&f.Date);
                printf("%s %d %d : %d", mons[ft->tm_mon], ft->tm_mday, ft->tm_hour, ft->tm_min);
                if (f.df)
                {
                    blue();
                    printf(" %s\n", rp);
                    clr_rst();
                }
                else if (f.xf)
                {
                    cyan();
                    printf(" %s\n", rp);
                    clr_rst();
                }
                else
                {
                    printf(" %s\n", rp);
                }
            }
            else
            {
                if (f.df)
                {
                    blue();
                    printf("%s ", rp);
                    clr_rst();
                }
                else if (f.xf)
                {
                    cyan();
                    printf("%s ", rp);
                    clr_rst();
                }
                else
                {
                    printf("%s ", rp);
                }
                printf("\n");
            }
            break;
        }
        file = readdir(dir);
    }
}

void L_ls(char *path, int af, int lf)
{
    DIR *dir;
    struct dirent *file;
    struct stat lstat;
    dir = opendir(path);
    file = readdir(dir);
    int c = 0;
    long long int tot = 0;
    long long int mtot = 0;

    char mons[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    while (file != NULL)
    {
        char b[1000];
        sprintf(b, "%s/%s", path, file->d_name);

        stat(b, &lstat);
        f[c].df = 0;
        f[c].xf = 0;
        f[c].Prop[0] = S_ISDIR(lstat.st_mode) ? 'd' : '-';
        if (f[c].Prop[0] == 'd')
            f[c].df = 1;

        mode_t owner, group, other;
        owner = lstat.st_mode & S_IRWXU;
        group = lstat.st_mode & S_IRWXG;
        other = lstat.st_mode & S_IRWXO;
        f[c].Prop[1] = owner & S_IRUSR ? 'r' : '-';
        f[c].Prop[2] = owner & S_IWUSR ? 'w' : '-';
        f[c].Prop[3] = owner & S_IXUSR ? 'x' : '-';

        f[c].Prop[4] = group & S_IRGRP ? 'r' : '-';
        f[c].Prop[5] = group & S_IWGRP ? 'w' : '-';
        f[c].Prop[6] = group & S_IXGRP ? 'x' : '-';

        f[c].Prop[7] = other & S_IROTH ? 'r' : '-';
        f[c].Prop[8] = other & S_IWOTH ? 'w' : '-';
        f[c].Prop[9] = other & S_IXOTH ? 'x' : '-';

        if (f[c].Prop[3] == 'x' || f[c].Prop[6] == 'x' || f[c].Prop[9] == 'x')
        {
            f[c].xf = 1;
        }

        strcpy(f[c].name, file->d_name);
        strcpy(f[c].user, getpwuid(lstat.st_uid)->pw_name);
        strcpy(f[c].group, getgrgid(lstat.st_gid)->gr_name);
        f[c].Date = lstat.st_ctime;
        f[c].hrdlink = lstat.st_nlink;
        f[c].size = lstat.st_size;
        tot += lstat.st_blocks;
        if (file->d_name[0] == '.')
        {
            mtot += lstat.st_blocks;
        }
        c++;
        file = readdir(dir);
    }

    qsort(f, c, sizeof(const File), strCompare);

    if (lf)
    {
        long long int ftot;
        ftot = af ? tot : tot - mtot;
        printf("total %lld\n", ftot / 2);
        for (int i = 0; i < c; i++)
        {
            char x[30];
            if (af || (!af && f[i].name[0] != '.'))
            {
                printf("%s %ld %s %s %ld ", f[i].Prop, f[i].hrdlink, f[i].user, f[i].group, f[i].size);
                struct tm *ft = localtime(&f[i].Date);
                printf("%s %d %d:%d", mons[ft->tm_mon], ft->tm_mday, ft->tm_hour, ft->tm_min);

                if (f[i].df)
                {
                    blue();
                    printf(" %s\n", f[i].name);
                    clr_rst();
                }
                else if (f[i].xf)
                {
                    cyan();
                    printf(" %s\n", f[i].name);
                    clr_rst();
                }
                else
                {
                    printf(" %s\n", f[i].name);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < c; i++)
        {
            if (af || (!af && f[i].name[0] != '.'))
            {
                if (f[i].df)
                {
                    blue();
                    printf("%s ", f[i].name);
                    clr_rst();
                }
                else if (f[i].xf)
                {
                    cyan();
                    printf("%s ", f[i].name);
                    clr_rst();
                }
                else
                {
                    printf("%s ", f[i].name);
                }
            }
        }
        printf("\n");
    }
}

void path(char **arg, char *wrkdir, char *strdir, int no)
{
    int dir[1000];
    int dirC = 0;
    int af = 0;
    int lf = 0;
    for (int i = 1; i < no; i++)
    {
        if (arg[i][0] == '-')
        {
            if (strlen(arg[i]) == 2)
            {
                if (arg[i][1] == 'a')
                    af = 1;
                else if (arg[i][1] == 'l')
                    lf = 1;
                else
                {
                    red();
                    printf("Invalid flag\n");
                    return;
                }
            }
            if (strlen(arg[i]) == 3)
            {
                if (!strcmp(arg[i], "-la") || !strcmp(arg[i], "-al"))
                {
                    af = 1;
                    lf = 1;
                }
                else
                {
                    red();
                    printf("Invalid flag\n");
                    return;
                }
            }
        }
        else
        {
            // if (dir)
            // {
            //     red();
            //     printf("Invalid command usage\n");
            //     return;
            // }
            dir[dirC++] = i;
        }
    }
    if (dirC == 0)
    {
        char temp[1000];
        temp[0] = 0;
        strcpy(temp, "");

        strcpy(temp, wrkdir);
        pwd(strdir, wrkdir);
        chdir(wrkdir);
        L_ls(".", af, lf);
        chdir(strdir);
        strcpy(wrkdir, temp);
        return;
    }
    for (int l = 0; l < dirC; l++)
    {
        char temp[1000];
        temp[0] = 0;
        strcpy(temp, "");

        strcpy(temp, wrkdir);
        pwd(strdir, wrkdir);
        chdir(wrkdir);
        char ll[2000];
        strcpy(ll, "");
        int ls = 0;
        if (arg[dir[l]][0] == '~')
        {
            chdir(strdir);
            for (int i = 2; i < strlen(arg[dir[l]]); i++)
            {
                char t[2];
                t[0] = arg[dir[l]][i];
                t[1] = 0;
                strcat(ll, t);
            }
        }
        else
        {
            strcpy(ll, arg[dir[l]]);
        }
        int fsl = 0;
        char f[1000];
        strcpy(f, "");
        for (int i = strlen(arg[dir[l]]) - 2; i >= 0; i--)
        {
            if (ll[i] == '/')
            {
                ls = i;
                fsl = 1;
                break;
            }
        }
        strncpy(f, &ll[ls + fsl], strlen(arg[dir[l]]) - 1 - ls - fsl + 1);
        f[strlen(arg[dir[l]]) - 1 - ls - fsl + 1] = 0;
        if (dirC > 1)
        {
            printf("%s :\n", arg[dir[l]]);
        }
        if (!chdir(ll) || dir[l] == 0 || !strcmp(arg[dir[l]], "~"))
            L_ls(".", af, lf);
        else
        {
            ll[ls] = 0;

            if (chdir(ll) && strcmp(ll, ""))
            {
                red();
                printf("Directory or file does not exist\n");
                clr_rst();
            }
            else
            {
                if (!access(f, F_OK))
                {
                    F_ls(".", af, lf, f, arg[dir[l]]);
                }

                else
                {
                    red();
                    printf("Directory or file does not exist\n");
                    clr_rst();
                }
            }
        }
        chdir(strdir);
        change(wrkdir, strdir);
    }
    return;
}