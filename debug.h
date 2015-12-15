#ifndef DEBUG_H
#define DEBUG_H

#define ESC          "\033["
#define COLOR_NORMAL ""
#define END          "m"

#define _BLACK   "30"
#define _RED     "31"
#define _GREEN   "32"
#define _YELLOW  "33"
#define _BLUE    "34"
#define _MAGENTA "35"
#define _CYAN    "36"
#define _WHITE   "37"

#define _NORMAL   "00"
#define _BOLD     "01"
#define _SOULIGNE "04"

#define MK_COLOR(type, bg) type";"bg

#define WHITE MK_COLOR(_NORMAL, _WHITE)
#define DARK_MAGENTA MK_COLOR(_NORMAL, _MAGENTA)
#define DARK_YELLOW MK_COLOR(_NORMAL,  _YELLOW)
#define DARK_CYAN MK_COLOR(_NORMAL, _CYAN)
#define DARK_BLUE MK_COLOR(_NORMAL, _BLUE)
#define DARK_GREEN MK_COLOR(_NORMAL, _GREEN)

#define LIGHT_MAGENTA MK_COLOR(_BOLD, _MAGENTA)
#define LIGHT_YELLOW MK_COLOR(_BOLD, _YELLOW)
#define LIGHT_CYAN MK_COLOR(_BOLD, _CYAN)
#define LIGHT_BLUE MK_COLOR(_BOLD, _BLUE)
#define LIGHT_GREEN MK_COLOR(_BOLD, _GREEN)
#define LIGHT_RED MK_COLOR(_BOLD,  _RED)

#undef DEBUG
#define DEBUG 1
#if DEBUG > 0
        #define WITH_COLOR
  #ifdef WITH_COLOR
    #define PRINT_DEBUG(symb, level, color, ...) \
          if (symb > level) { \
                    fprintf(stderr, "%s%s%s", ESC, color, END); \
                    fprintf(stderr, __VA_ARGS__); \
                    fprintf(stderr, "%s%s%s", ESC, COLOR_NORMAL, END); \
                    fflush(stderr);\
                }
  #else
    #define PRINT_DEBUG(symb, level, color, ...) \
          if (symb > level) { \
                  fprintf(stderr, __VA_ARGS__);\
                          fflush(stderr);\
                }
  #endif
#else
        #define PRINT_DEBUG(symb, level, color, args...) do {} while(0);
#endif

#endif
