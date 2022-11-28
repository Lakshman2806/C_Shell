#ifndef _REDIRECT_H_
#define _REDIRECT_H_

int inout(char **arg, int no, int *out, int *in, int *o, int *id, char *wkrdir, char *strdir);

void back(int out, int in, int o, int i);

#endif