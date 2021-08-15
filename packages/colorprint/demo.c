#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "colorprint.h"

int main(int argc, char const *argv[])
{
    //Font color or foreground color
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

    printf(CL_BRIGHT_BLACK"BLACK\t"CL_END);
    printf(CL_BRIGHT_RED"RED\t"CL_END);
    printf(CL_BRIGHT_GREEN"GREEN\t"CL_END);
    printf(CL_BRIGHT_BROWN"BROWN\t"CL_END);
    printf(CL_BRIGHT_BLUE"BLUE\t"CL_END);
    printf(CL_BRIGHT_PURPLE"PURPLE\t"CL_END);
    printf(CL_BRIGHT_CYAN"CYAN\t"CL_END);
    printf(CL_BRIGHT_WHITE"WHITE\n"CL_END);

    ////Special effects
    printf(CL_BOLD"BOLD\t"CL_END);
    printf(CL_WEAKEN"WEAKEN\t"CL_END);
    printf(CL_ITALICS"ITALICS\t"CL_END);
    printf(CL_UNDERLINE CL_BOLD"UNDERLINE\t"CL_END);
    printf(CL_TWINKLE CL_GREEN"ITALICS\t"CL_END);
    printf(CL_QUICK_TWINKLE"QUICK_TWINKLE\n"CL_END);
    printf(CL_INVERT"INVERT\t"CL_END);
    printf(CL_HIDE"HIDE\t"CL_END);
    printf(CL_DELETE"DELETE\t"CL_END);
    printf(CL_ANGULAR"ANGULAR\t"CL_END);
    printf(CL_FRAMED"FRAMED\t"CL_END);
    printf(CL_ENCIRCLED"ENCIRCLED\t"CL_END);
    printf(CL_OVERLINE"OVERLINE\n"CL_END);
    printf(CL_IDEOGRAPH_UNDERLINE"IDEOGRAPH_UNDERLINE\t"CL_END);
    printf(CL_IDEOGRAPH_DB_UNDERLINE"IDEOGRAPH_DB_UNDERLINE\t"CL_END);
    printf(CL_IDEOGRAPH_OVERLINE"IDEOGRAPH_OVERLINE\n"CL_END);
    printf(CL_IDEOGRAPH_DB_OVERLINE"IDEOGRAPH_DB_OVERLINE\t"CL_END);
    printf(CL_IDEOGRAPH_FOCUS"IDEOGRAPH_FOCUS\t"CL_END);

    //Font
    printf(CL_MAIN_FONT"MAIN_FONT\t"CL_END);
    printf(CL_FONT1"FONT1\t"CL_END);
    printf(CL_FONT2"FONT2\n"CL_END);
    printf(CL_FONT3"FONT3\t"CL_END);
    printf(CL_FONT4"FONT4\t"CL_END);
    printf(CL_FONT5"FONT5\t"CL_END);
    printf(CL_FONT6"FONT6\t"CL_END);
    printf(CL_FONT7"FONT7\t"CL_END);
    printf(CL_FONT8"FONT8\n"CL_END);
    printf(CL_FONT9"FONT9\t"CL_END);
    printf(CL_ANGULAR"ANGULAR\t"CL_END);

    //Cancel or delete
    printf(CL_UNDERLINE CL_BOLD"UNDERLINE\t"CL_CANCEL_UNDERLINE"UNDERLINE\n"CL_END);

    //Background color
    printf(CL_BACK_BLACK"BLACK\t"CL_END);
    printf(CL_BACK_RED CL_TWINKLE"RED\t"CL_END);
    printf(CL_BACK_GREEN CL_TWINKLE"GREEN\t"CL_END);
    printf(CL_BACK_BROWN"BROWN\t"CL_END);
    printf(CL_BACK_BLUE"BLUE\t"CL_END);
    printf(CL_BACK_PURPLE"PURPLE\t"CL_END);
    printf(CL_BACK_WHITE"WHITE\t"CL_END);
    printf(CL_BACK_CYAN CL_TWINKLE"CYAN\t"CL_END);
    printf(CL_BACK_DEFAULT"DEFAULT\n"CL_END);

    printf(CL_BRIGHT_BACK_BLACK"BLACK\t"CL_END);
    printf(CL_BRIGHT_BACK_RED CL_TWINKLE"RED\t"CL_END);
    printf(CL_BRIGHT_BACK_GREEN CL_TWINKLE"GREEN\t"CL_END);
    printf(CL_BRIGHT_BACK_BROWN"BROWN\t"CL_END);
    printf(CL_BRIGHT_BACK_BLUE"BLUE\t"CL_END);
    printf(CL_BRIGHT_BACK_PURPLE"PURPLE\t"CL_END);
    printf(CL_BRIGHT_BACK_WHITE"WHITE\t"CL_END);
    printf(CL_BRIGHT_BACK_CYAN CL_TWINKLE"CYAN\t"CL_END);
    printf(CL_BRIGHT_BACK_DEFAULT"DEFAULT\n"CL_END);

    progress_t pro;
    int i;

    clprogress_init(&pro, '#', "PROGRESS_NUM_STYLE", 80, PROGRESS_NUM_STYLE, PROGRESS_RED, 0, NULL, NULL);
    for(i = 0; i < 150; i = i + 40){
        //clprogress_print("TEST\n");
        clprogress(pro, 1.0 * i / 150);
        sleep(1);
    }
    clprogress(pro, 1);
    printf("\n");

    clprogress_init(&pro, '#', "PROGRESS_CHR_STYLE", 80, PROGRESS_CHR_STYLE, PROGRESS_BROWN, 0, NULL, NULL);
    for(i = 0; i < 150; i = i + 40){
        //clprogress_print("TEST\n");
        clprogress(pro, 1.0 * i / 150);
        sleep(1);
    }
    clprogress(pro, 1);
    printf("\n");

    clprogress_init(&pro, '#', "PROGRESS_BGC_STYLE", 80, PROGRESS_BGC_STYLE, PROGRESS_BLUE, PROGRESS_BG_GREEN, NULL, NULL);
    for(i = 0; i < 150; i = i + 40){
        //clprogress_print("TEST\n");
        clprogress(pro, 1.0 * i / 150);
        sleep(1);
    }
    clprogress(pro, 1);
    printf("\n");

    return 0;
}




