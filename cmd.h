#ifndef _CMD_H_
#define _CMD_H_

#define CMAX 1000
#define MaxArg 30

// initates a char*
char *init();
void nicecmd(char **cmd);
// deletes a cmd intiated from the heap
void cmddel(char **x);

void del(char ***arg, int no);

// parses the command into seperate strings
int cmd_parse(char *cmd, char ***arg);

#endif
