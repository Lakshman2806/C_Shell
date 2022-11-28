#ifndef _CD_H_
#define _CD_H_

// executes cd
void cd(char **arg, int no, char *wrkdir, char *strdir, char *prevdir);

void Mkdir(char **arg, char *wrkdir);
void change(char *wrkdir, char *strdir);

#endif