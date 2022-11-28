#include <stdio.h>
#include "colours.h"

void green()
{
    printf("\033[0;32m");
}

void red()
{
    printf("\033[0;31m");
}

void blue()
{
    printf("\033[0;34m");
}
void cyan()
{
    printf("\033[0;32m");
}

void clr_rst()
{
    printf("\033[0m");
}