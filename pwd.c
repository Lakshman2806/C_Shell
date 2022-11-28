#include "head.h"

void pwd(char *strdir, char *wrkdir)
{
    char *x;
    x = (char *)calloc(1000, sizeof(char));
    x[0] = '\0';
    if (wrkdir[0] == '~')
    {
        strcat(x, strdir);
        for (int i = 1; i < strlen(wrkdir); i++)
        {
            char t[2];
            t[0] = wrkdir[i];
            t[1] = '\0';
            strcat(x, t);
        }
        strcpy(wrkdir, x);
    }
    else
    {
        return;
    }
}
