#include "echo.h"
#include "head.h"

void echo(char **arg, int no)
{
    for (int i = 1; i < no; i++)
    {
        printf("%s ", arg[i]);
    }
    printf("\n");
    return;
}
