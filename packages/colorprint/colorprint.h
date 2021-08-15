#ifndef _COLORPRINT_H
#define _COLORPRINT_H

//Font color or foreground color
#define CL_BLACK                    "\033[30m"
#define CL_RED                      "\033[31m"
#define CL_GREEN                    "\033[32m"
#define CL_BROWN                    "\033[33m"
#define CL_BLUE                     "\033[34m"
#define CL_PURPLE                   "\033[35m"
#define CL_CYAN                     "\033[36m"
#define CL_WHITE                    "\033[37m"

#define CL_BRIGHT_BLACK             "\033[90m"
#define CL_BRIGHT_RED               "\033[91m"
#define CL_BRIGHT_GREEN             "\033[92m"
#define CL_BRIGHT_BROWN             "\033[93m"
#define CL_BRIGHT_BLUE              "\033[94m"
#define CL_BRIGHT_PURPLE            "\033[95m"
#define CL_BRIGHT_CYAN              "\033[96m"
#define CL_BRIGHT_WHITE             "\033[97m"

#define CL_BLACK_BOLD               "\033[30;1m"
#define CL_RED_BOLD                 "\033[31;1m"
#define CL_GREEN_BOLD               "\033[32;1m"
#define CL_BROWN_BOLD               "\033[33;1m"
#define CL_BLUE_BOLD                "\033[34;1m"
#define CL_PURPLE_BOLD              "\033[35;1m"
#define CL_CYAN_BOLD                "\033[36;1m"
#define CL_WHITE_BOLD               "\033[37;1m"

//Special effects
#define CL_RESET                    "\033[0m"       //Close all style
#define CL_BOLD                     "\033[1m"
#define CL_WEAKEN                   "\033[2m"
#define CL_ITALICS                  "\033[3m"
#define CL_UNDERLINE                "\033[4m"
#define CL_TWINKLE                  "\033[5m"
#define CL_QUICK_TWINKLE            "\033[6m"
#define CL_INVERT                   "\033[7m"       //Foreground and background colors swap
#define CL_HIDE                     "\033[8m"
#define CL_DELETE                   "\033[9m"
#define CL_MAIN_FONT                "\033[10m"
#define CL_FONT1                    "\033[11m"
#define CL_FONT2                    "\033[12m"
#define CL_FONT3                    "\033[13m"
#define CL_FONT4                    "\033[14m"
#define CL_FONT5                    "\033[15m"
#define CL_FONT6                    "\033[16m"
#define CL_FONT7                    "\033[17m"
#define CL_FONT8                    "\033[18m"
#define CL_FONT9                    "\033[19m"
#define CL_ANGULAR                  "\033[20m"
#define CL_FRAMED                   "\033[51m"
#define CL_ENCIRCLED                "\033[52m"
#define CL_OVERLINE                 "\033[53m"
#define CL_IDEOGRAPH_UNDERLINE      "\033[60m"
#define CL_IDEOGRAPH_DB_UNDERLINE   "\033[61m"
#define CL_IDEOGRAPH_OVERLINE       "\033[62m"
#define CL_IDEOGRAPH_DB_OVERLINE    "\033[63m"
#define CL_IDEOGRAPH_FOCUS          "\033[64m"

//Cancel or delete
#define CL_CANCEL_BOLD              "\033[21m"
#define CL_CANCEL_COLOR             "\033[22m"
#define CL_CANCEL_ITALICS           "\033[23m"
#define CL_CANCEL_UNDERLINE         "\033[24m"
#define CL_CANCEL_TWINKLE           "\033[25m"
#define CL_CANCEL_INVERT            "\033[27m"
#define CL_CANCEL_HIDE              "\033[28m"
#define CL_CANCEL_DELETE            "\033[29m"
#define CL_CANCEL_FRAMED            "\033[54m"
#define CL_CANCEL_OVERLINE          "\033[55m"
#define CL_CANCEL_IDEOGRAPH         "\033[65m"

//Background color
#define CL_BACK_BLACK               "\033[40m"
#define CL_BACK_RED                 "\033[41m"
#define CL_BACK_GREEN               "\033[42m"
#define CL_BACK_BROWN               "\033[43m"
#define CL_BACK_BLUE                "\033[44m"
#define CL_BACK_PURPLE              "\033[45m"
#define CL_BACK_CYAN                "\033[46m"
#define CL_BACK_WHITE               "\033[47m"
#define CL_BACK_DEFAULT             "\033[49m"

#define CL_BRIGHT_BACK_BLACK        "\033[100m"
#define CL_BRIGHT_BACK_RED          "\033[101m"
#define CL_BRIGHT_BACK_GREEN        "\033[102m"
#define CL_BRIGHT_BACK_BROWN        "\033[103m"
#define CL_BRIGHT_BACK_BLUE         "\033[104m"
#define CL_BRIGHT_BACK_PURPLE       "\033[105m"
#define CL_BRIGHT_BACK_CYAN         "\033[106m"
#define CL_BRIGHT_BACK_WHITE        "\033[107m"
#define CL_BRIGHT_BACK_DEFAULT      "\033[109m"

#define CL_END                      "\033[0m"

#define MAX_BAR_LENGTH              200
typedef struct{
    char chr;           //tip char
    char *title;        //tip string
    int style;          //progress style
    int len;            //length of bar
    int color;
    int bgcolor;
    FILE *stream;       //file to be flush
    float offset;
    char *pro;
}progress_t;

typedef enum{
    PROGRESS_BLACK = 0,
    PROGRESS_RED,
    PROGRESS_GREEN,
    PROGRESS_BROWN,
    PROGRESS_BLUE,
    PROGRESS_PURPLE,
    PROGRESS_CYAN,
    PROGRESS_WHITE,
}progress_cl;

typedef enum{
    PROGRESS_BG_DEFAULT = 0,
    PROGRESS_BG_BLACK,
    PROGRESS_BG_RED,
    PROGRESS_BG_GREEN,
    PROGRESS_BG_BROWN,
    PROGRESS_BG_BLUE,
    PROGRESS_BG_PURPLE,
    PROGRESS_BG_CYAN,
    PROGRESS_BG_WHITE,
}progress_bgcl;

typedef enum{
    PROGRESS_NUM_STYLE = 0,
    PROGRESS_CHR_STYLE,
    PROGRESS_BGC_STYLE,
}progress_style;

/** 
 * @brief Init struc progress_t， struc progress_t must use it to init.
 * 
 * @param pro       The point of struc progress_t
 * @param chr       Char to be used in PROGRESS_CHR_STYLE
 * @param title     The title of this progress
 * @param len       The len of bar, must be less than or equal to MAX_BAR_LENGTH
 * @param style     The style of progress
 * @param color     The color of font
 * @param bgcolor   The background color of this progress, please set it to 0 for default color
 * @param stream    The file to be flush, if it is stdout, you can set it to NULL
 * @param bar       A string to set the bar, if you not need thread safe, you can set it to NULL, or you need to provide
 *                  a buf that greater than MAX_BAR_LENGTH
 *
 * @return void
 * 
 * @author Jacken Zhang     1178187720@qq.com
 */
void clprogress_init(progress_t *pro, char chr, char *title, int len, int style, int color, int bgcolor, FILE *stream, char *bar);

/** 
 * @brief The function to show the progress
 * 
 * @param pro       A struc progress_t that init by clprogress_init
 * @param percent   Percent of the progress
 * 
 * @return void
 * 
 * @author Jacken Zhang     1178187720@qq.com
 */
void clprogress(progress_t pro, float percent);

/** 
 * @brief If you want to output somethings during using clprogress， please use this function， it will clean the screen.
 * 
 * @param format   Somethings you want to output, same as printf
 * 
 * @return void
 * 
 * @author Jacken Zhang     1178187720@qq.com
 */
void clprogress_print(const char *format, ...);

#endif