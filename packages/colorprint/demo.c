#include <stdio.h>
#include <stdlib.h>
#include "colorprint.h"

int main(int argc, char const *argv[])
{
    printf(CL_BLACK"BLACK\t"CL_END);
    printf(CL_RED"RED\t"CL_END);
    printf(CL_GREEN"GREEN\t"CL_END);
    printf(CL_BROWN"BROWN\t"CL_END);
    printf(CL_BLUE"BLUE\t"CL_END);
    printf(CL_PURPLE"PURPLE\t"CL_END);
    printf(CL_CYAN"CYAN\t"CL_END);
    printf(CL_WHITE"WHITE\n"CL_END);

    printf(CL_BLACK_BOLD"BLACK\t"CL_END);
    printf(CL_RED_BOLD"RED\t"CL_END);
    printf(CL_GREEN_BOLD"GREEN\t"CL_END);
    printf(CL_BROWN_BOLD"BROWN\t"CL_END);
    printf(CL_BLUE_BOLD"BLUE\t"CL_END);
    printf(CL_PURPLE_BOLD"PURPLE\t"CL_END);
    printf(CL_CYAN_BOLD"CYAN\t"CL_END);
    printf(CL_WHITE_BOLD"WHITE\n"CL_END);

    printf(CL_BOLD"BOLD\t"CL_END);
    printf(CL_WEAKEN"WEAKEN\t"CL_END);
    printf(CL_ITALICS"ITALICS\n"CL_END);
    printf(CL_UNDERLINE CL_BOLD"UNDERLINE\t"CL_END);
    printf(CL_TWINKLE CL_GREEN"ITALICS\n"CL_END);


    return 0;
}



