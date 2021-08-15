#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdarg.h>
#include "colorprint.h"

static int (*_print)(const char *, ...) = printf;
static char _bar[MAX_BAR_LENGTH];

void clprogress_init(progress_t *pro, char chr, char *title, int len, int style, int color, int bgcolor, FILE *stream, char *bar){
    pro->chr = (chr == '\0' ? '#' : chr);
    pro->title = title;
    pro->len = (len > MAX_BAR_LENGTH ? MAX_BAR_LENGTH : len);
    pro->style = style;
    pro->color = color;
    pro->bgcolor = bgcolor;
    pro->stream = (stream == NULL ? stdout : stream);
    pro->pro = (bar == NULL ? _bar : bar);
    memset(pro->pro, ' ', pro->len);
    *(pro->pro + pro->len - 1) = '\0';
}

void clprogress(progress_t pro, float percent){
#define COLOR_LEN   20
    char color[COLOR_LEN];
    char bgcolor[COLOR_LEN];

    switch (pro.color)
    {
        case PROGRESS_BLACK:
            strncpy(color, CL_BLACK, COLOR_LEN);
            break;

        case PROGRESS_RED:
            strncpy(color, CL_RED, COLOR_LEN);
            break;

        case PROGRESS_GREEN:
            strncpy(color, CL_GREEN, COLOR_LEN);
            break;

        case PROGRESS_BROWN:
            strncpy(color, CL_BROWN, COLOR_LEN);
            break;

        case PROGRESS_BLUE:
            strncpy(color, CL_BLUE, COLOR_LEN);
            break;

        case PROGRESS_PURPLE:
            strncpy(color, CL_PURPLE, COLOR_LEN);
            break;

        case PROGRESS_CYAN:
            strncpy(color, CL_CYAN, COLOR_LEN);
            break;

        case PROGRESS_WHITE:
            strncpy(color, CL_WHITE, COLOR_LEN);
            break;

        default:
            strncpy(color, CL_GREEN, COLOR_LEN);
            break;
    }

    switch (pro.bgcolor)
    {
        case PROGRESS_BG_BLACK:
            strncpy(bgcolor, CL_BACK_BLACK, COLOR_LEN);
            break;

        case PROGRESS_BG_RED:
            strncpy(bgcolor, CL_BACK_RED, COLOR_LEN);
            break;

        case PROGRESS_BG_GREEN:
            strncpy(bgcolor, CL_BACK_GREEN, COLOR_LEN);
            break;

        case PROGRESS_BG_BROWN:
            strncpy(bgcolor, CL_BACK_BROWN, COLOR_LEN);
            break;

        case PROGRESS_BG_BLUE:
            strncpy(bgcolor, CL_BACK_BLUE, COLOR_LEN);
            break;

        case PROGRESS_BG_PURPLE:
            strncpy(bgcolor, CL_BACK_PURPLE, COLOR_LEN);
            break;

        case PROGRESS_BG_CYAN:
            strncpy(bgcolor, CL_BACK_CYAN, COLOR_LEN);
            break;

        case PROGRESS_BG_WHITE:
            strncpy(bgcolor, CL_BACK_WHITE, COLOR_LEN);
            break;

        default:
            strncpy(bgcolor, CL_BACK_DEFAULT, COLOR_LEN);
            break;
    }

    switch (pro.style)
    {
    case PROGRESS_NUM_STYLE:
        _print("%s%s\033[1m%s %.1f%%\033[0m\r", color, bgcolor, pro.title, percent*100);
        fflush(pro.stream);
        break;
    
    case PROGRESS_CHR_STYLE:
        {
            int offset;
            offset = (int)(pro.len * percent) - 1;
            offset = offset < 0 ? 0 : offset;
            memset(pro.pro, pro.chr, offset);
            _print("%s%s\033[1m%s [%-s] %.1f%% \033[0m\r", color, bgcolor, pro.title, pro.pro, percent*100);
            fflush(pro.stream);
        }
        break;

    case PROGRESS_BGC_STYLE:
        {
            int offset;
            memset(pro.pro, ' ', pro.len);
            offset = (int)(pro.len * percent) - 1;
            offset = offset < 0 ? 0 : offset;
            *(pro.pro + (int)(pro.len * percent) - 1) = '\0';
            *(pro.pro + offset) = '\0';
            _print("%s\033[1m%s %s %s\033[40m%s\033[49m %.2f%%\033[0m\r", color, pro.title, bgcolor, pro.pro, pro.pro+(offset+1 == pro.len ? pro.len : offset+1), percent*100);
            fflush(pro.stream);
        }
        break;
    default:
        break;
    }
}

void clprogress_print(const char *format, ...){
    struct winsize size;
    char *str;
    va_list ap;
    int len;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    str = malloc(size.ws_col);

    //clean screen
    memset(str, ' ', size.ws_col);
    *(str+size.ws_col-1) = '\0';
    _print("%s\r", str);

    va_start(ap, format);
    len = vsnprintf(str, size.ws_col, format, ap);
    va_end(ap);
    _print("%s", str);

    free(str);
}