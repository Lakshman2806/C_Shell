#ifndef _LS_H_
#define _LS_H_

#include "head.h"

#define MaxLFile 1000

struct file
{
    char Prop[15];
    long int hrdlink;
    char user[MaxLFile];
    char group[MaxLFile];
    long int size;
    long int Date;
    char name[MaxLFile];
    int df;
    int xf;
};

typedef struct file File;

void path(char **arg, char *wrkdir, char *strdir, int no);

#endif