#include "head.h"

#define clear() printf("\033[H\033[J")
#define MAXL 1000

struct termios orig_termios;

char SysName[MAXL];
char UserName[MAXL];
char wrkdir[MAXL];
char strdir[CMAX];
char prevdir[CMAX];
char **arg;
int x;
int nop = 0;
long lastfg = 0;
char fgcomm[1000] = "";
double time_taken = 0;
int fg = 0;
long int pid[1000];
char bgcomm[100][1000];
int bgc = 0;
int ind[1000];

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int check(char *arg, int st)
{
    int n = strlen(arg);
    for (int i = st; i < n; i++)
    {
        if (arg[i] == '&')
            return i;
    }
    return n;
}

void printprompt()
{
    green();
    printf("<%s@", UserName);
    blue();
    printf("%s:", SysName);
    red();
    printf("%s", wrkdir);
    if (time_taken >= 1)
    {
        printf("took %ds", (int)time_taken);
    }
    printf(">");
    clr_rst();
}
int gc = 0;
void func()
{
    int status;
    if (gc > 1)
    {
        // printf("lol\n");
    }
    for (int i = 0; i < 100; i++)
    {
        if (pid[i])
        {
            if (waitpid(pid[i], &status, WNOHANG) > 0)
            {
                char ab[][15] = {"normally", "abnormally"};

                int cflag = 1;
                if (WIFEXITED(status))
                {
                    cflag = 0;
                }
                fprintf(stderr, "\nThe child %s pid %ld has exited %s\n", bgcomm[i], pid[i], ab[cflag]);
                // fprintf(stdout, "lol\n");
                if (!fg)
                {
                    printprompt();
                    nop = 1;
                }
                bgc--;

                pid[i] = 0;
            }
        }
    }
}
void RunShell();
void CrtlC()
{
    if (fg)
    {
        kill(lastfg, 9);
        printf("\n");
        fg = 0;
    }
    printf("\n");
}
void RunShell()
{
    while (1)
    {
        setbuf(stdout, NULL);
        enableRawMode();
        change(wrkdir, strdir);
        fg = 0;

        size_t sz = 1000;
        char *tcmd = init();
        int tlen = 0;
        char tempc;
    pleasework:
        if (!nop)
        {
            printprompt();
            printf("%s", tcmd);
        }
        else
        {
            nop = 0;
        }
        while (read(STDIN_FILENO, &tempc, 1) == 1)
        {
            if (iscntrl(tempc))
            {
                if (tempc == 10)
                    break;
                else if (tempc == 27)
                {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2)
                    { // length of escape code
                        printf("\nSorry did not implment it :(");
                        break;
                    }
                }
                else if (tempc == 127)
                { // backspace
                    if (tlen > 0)
                    {
                        if (tcmd[tlen - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        tcmd[--tlen] = '\0';
                        printf("\b \b");
                    }
                }
                else if (tempc == 9)
                {
                    int pllafl = autocmp(tcmd, wrkdir, strdir, tlen);
                    if (pllafl)
                    {
                        tlen = strlen(tcmd);
                        if (pllafl > 1)
                            goto pleasework;
                    }
                }
                else if (tempc == 4)
                {
                    exit(0);
                }
                else
                {
                    printf("%d\n", tempc);
                }
            }
            else
            {
                tcmd[tlen++] = tempc;
                printf("%c", tempc);
            }
        }
        printf("\n");
        disableRawMode();
        // printf("lol\n");
        fg = 1;
        addHistory(tcmd);
        char *part[1000];
        int c = 0;
        part[c] = strtok(tcmd, ";");
        time_t etime, stime;
        etime = time(&stime);
        while (part[c] != NULL)
        {
            c++;
            part[c] = strtok(NULL, ";");
        }
        for (int i = 0; i < c; i++)
        {
            nicecmd(&part[i]);
            int len = strlen(part[i]);
            int blf = check(part[i], 0);
            char temp[strlen(part[i])];
            strcpy(temp, "");
            strcpy(temp, part[i]);
            int ac = 0;
            char *vscmd[1000];
            vscmd[ac] = strtok(part[i], "&");
            while (vscmd[ac] != NULL)
            {
                ac++;
                vscmd[ac] = strtok(NULL, "&");
            }
            int porgo = dup(1), porgi = dup(0);

            for (int k = 0; k < ac; k++)
            {
                char *scmd[1000];
                int vc = 0;
                char strcmd[1000];
                memset(strcmd, 0, 1000);
                strcpy(strcmd, vscmd[k]);
                scmd[vc] = strtok(vscmd[k], "|");
                while (scmd[vc] != NULL)
                {
                    vc++;
                    scmd[vc] = strtok(NULL, "|");
                }

                int pipes[2][2];

                for (int j = 0; j < vc; j++)
                {
                    int out = 0, in = 0;
                    int o = 0, id = 0;
                    int bg = 0;

                    if (blf != len)
                    {
                        bg = 1;
                    }
                    nicecmd(&scmd[j]);

                    // close(o);
                    // close(i);
                    if (strcmp(scmd[j], "\n") && strcmp(scmd[j], ""))
                    {
                        if (j == vc - 1)
                        {
                            o = 0;
                        }
                        else
                        {
                            if (pipe(pipes[j % 2]) < 0)
                            {
                                red();
                                perror("The fuck is wrong with pipes\n");
                                clr_rst();
                                continue;
                            }
                            o = pipes[j % 2][1];
                            // dup2(o, 1);
                        }
                        if (j == 0)
                        {
                            id = 0;
                        }
                        else
                        {
                            id = pipes[(j + 1) % 2][0];
                            // char pipe_content[105];
                            // read(id, pipe_content, 100);
                            // write(porgo, pipe_content, 105);
                            // dup2(pipes[(j + 1) % 2][0], 0);
                            // char pl[1000];
                            // read(id, pl, 1000);
                            // write(porgo, pl, 1000);
                            // read(0, pl, 1000);
                            // write(porgo, pl, 1000);
                        }

                        arg = (char **)malloc(sizeof(char *) * MaxArg);
                        x = cmd_parse(scmd[j], &arg);
                        int flag = 1;
                        flag = inout(arg, x, &out, &in, &o, &id, wrkdir, strdir);
                        // part = strtok(NULL, ";");
                        char **parg;
                        parg = (char **)malloc(sizeof(char *) * MaxArg);

                        int pc = 0;
                        for (int i = 0; i < x; i++)
                        {
                            if (strcmp(arg[i], ">") && strcmp(arg[i], ">>") && strcmp(arg[i], "<"))
                            {
                                parg[pc] = (char *)calloc(50, sizeof(char));
                                strcpy(parg[pc++], arg[i]);
                            }
                            else
                            {
                                i++;
                            }
                        }
                        x = pc;
                        if (flag)
                        {
                            if (!strcmp(parg[0], "quit"))
                            {
                                return;
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "cd"))
                            {
                                cd(parg, x, wrkdir, strdir, prevdir);
                                // time_taken = 0;
                            }

                            else if (!strcmp(parg[0], "pwd"))
                            {
                                if (wrkdir[0] == '~')
                                {
                                    char path[CMAX];
                                    path[0] = '\0';
                                    strcat(path, strdir);
                                    for (int i = 1; i < strlen(wrkdir); i++)
                                    {
                                        char t[2];
                                        t[0] = wrkdir[i];
                                        t[1] = '\0';
                                        strcat(path, t);
                                    }
                                    printf("%s\n", path);
                                }
                                else
                                {
                                    printf("%s\n", wrkdir);
                                }
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "echo"))
                            {
                                echo(parg, x);
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "ls"))
                            {
                                path(parg, wrkdir, strdir, x);
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "pinfo"))
                            {
                                pinfo(parg, x, wrkdir, strdir);
                                // time_taken = 0;
                            }
                            else if (!strcmp(arg[0], "discover"))
                            {
                                discover(strdir, wrkdir, parg, x);
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "history"))
                            {
                                History(10);
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "bg"))
                            {
                                if (pid[atol(parg[1]) - 1])
                                {
                                    kill(pid[atol(parg[1]) - 1], SIGCONT);
                                }
                                // time_taken = 0;
                            }
                            else if (!strcmp(arg[0], "sig"))
                            {
                                if (x != 3)
                                {
                                    red();
                                    perror("Error in command\n");
                                    clr_rst();
                                }
                                else
                                {
                                    if (pid[atol(parg[1]) - 1])
                                    {
                                        kill(pid[atol(parg[1]) - 1], atol(parg[2]));
                                        printf("%ld\n", pid[atol(parg[1]) - 1]);
                                    }
                                }
                                // time_taken = 0;
                            }
                            else if (!strcmp(parg[0], "fg"))
                            {
                                fgHandle(atoi(parg[1]), pid, &bgc);
                            }
                            else if (!strcmp(parg[0], "jobs"))
                            {
                                jobs(parg, x, bgcomm, pid, ind);
                                // time_taken = 0;
                            }
                            else
                            {
                                parg[pc] = NULL;
                                char temp[1000];
                                strcpy(temp, wrkdir);
                                pwd(strdir, wrkdir);
                                chdir(wrkdir);
                                int forkr = fork();

                                if (forkr == 0)
                                {
                                    if (bg)
                                        setpgid(0, 0);
                                    if (execvp(parg[0], parg) == -1)
                                    {
                                        // printf("lol :(\n");
                                        red();
                                        printf("The command didn't work :(\n");
                                        clr_rst();
                                        chdir(strdir);
                                        strcpy(wrkdir, temp);
                                        exit(0);
                                    }
                                }
                                else
                                {
                                    if (!bg)
                                    {

                                        int status;

                                        lastfg = forkr;
                                        strcpy(fgcomm, parg[0]);
                                        waitpid(0, &status, WUNTRACED);
                                    }
                                    else
                                    {
                                        bgc++;
                                        gc++;
                                        // printf("%d\n", gc);
                                        fprintf(stderr, "[%d] %d\n", bgc, forkr);
                                        pid[bgc - 1] = forkr;
                                        ind[bgc - 1] = bgc;
                                        strcpy(bgcomm[bgc - 1], strcmd);
                                        // time_taken = 0;
                                    }
                                    chdir(strdir);
                                    strcpy(wrkdir, temp);
                                }
                            }
                        }
                        del(&parg, x);
                        del(&arg, x);

                        back(out, in, o, id);
                    }
                    blf = check(temp, blf + 1);
                }

                //     scmd = strtok(NULL, "&");
                // }
            }
            dup2(porgo, 1);
            dup2(porgi, 0);
        }
        etime = time(&etime);
        // func();
        time_taken = etime - stime;
        cmddel(&tcmd);
    }
}
void CrtlZ()
{
    if (fg)
    {
        int frt = fork();
        if (frt == 0)
        {
            setpgid(lastfg, 0);
            kill(lastfg, SIGSTOP);
            exit(0);
        }
        else
        {
            bgc++;
            gc++;
            // printf("%d\n", gc);
            fprintf(stderr, "[%d] %ld\n", bgc, lastfg);
            pid[bgc - 1] = lastfg;
            ind[bgc - 1] = bgc;
            strcpy(bgcomm[bgc - 1], fgcomm);
            fg = 0;
        }
    }
}
int main(void)
{
    gethostname(SysName, MAXL);
    getlogin_r(UserName, MAXL);
    struct passwd p;
    signal(SIGCHLD, func);
    signal(SIGTSTP, CrtlZ);
    signal(SIGINT, CrtlC);
    for (int i = 0; i < 100; i++)
    {
        pid[i] = 0;
    }
    strcpy(wrkdir, "~");
    strcpy(prevdir, "~");
    getcwd(strdir, CMAX);

    RunShell();
    return 0;
}