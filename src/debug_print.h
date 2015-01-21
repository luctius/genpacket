#pragma once
#ifndef DEBUG_PRINT_H_
#define DEBUG_PRINT_H_

#include <stdarg.h>

#define GP_COLOR_NORMAL          ""
#define GP_COLOR_RESET           "\033[m"
#define GP_COLOR_BOLD            "\033[1m"
#define GP_COLOR_RED             "\033[31m"
#define GP_COLOR_GREEN           "\033[32m"
#define GP_COLOR_YELLOW          "\033[33m"
#define GP_COLOR_BLUE            "\033[34m"
#define GP_COLOR_MAGENTA         "\033[35m"
#define GP_COLOR_CYAN            "\033[36m"
#define GP_COLOR_BOLD_RED        "\033[1;31m"
#define GP_COLOR_BOLD_GREEN      "\033[1;32m"
#define GP_COLOR_BOLD_YELLOW     "\033[1;33m"
#define GP_COLOR_BOLD_BLUE       "\033[1;34m"
#define GP_COLOR_BOLD_MAGENTA    "\033[1;35m"
#define GP_COLOR_BOLD_CYAN       "\033[1;36m"
#define GP_COLOR_BG_RED          "\033[41m"
#define GP_COLOR_BG_GREEN        "\033[42m"
#define GP_COLOR_BG_YELLOW       "\033[43m"
#define GP_COLOR_BG_BLUE         "\033[44m"
#define GP_COLOR_BG_MAGENTA      "\033[45m"
#define GP_COLOR_BG_CYAN         "\033[46m"

enum debug_levels {
    GP_DEBUG_LEVEL_ERROR,
    GP_DEBUG_LEVEL_WARNING,
    GP_DEBUG_LEVEL_INFORMATIONAL,
    GP_DEBUG_LEVEL_DEBUG,
};

extern enum debug_levels global_debug_lvl;

void gp_printf(enum debug_levels dbg_lvl, const char *module, int line, const char* format, ... );
void gp_printf_basic(enum debug_levels dbg_lvl, const char* module, int line, const char* format,  va_list args);

#define gp_err(fmt, ...)   do {gp_printf(GP_DEBUG_LEVEL_ERROR,         __FILE__, __LINE__, GP_COLOR_RED     fmt GP_COLOR_RESET,  ##__VA_ARGS__);} while (0);
#define gp_warn(fmt, ...)  do {gp_printf(GP_DEBUG_LEVEL_WARNING,       __FILE__, __LINE__, GP_COLOR_YELLOW  fmt GP_COLOR_RESET,  ##__VA_ARGS__);} while (0);
#define gp_info(fmt, ...)  do {gp_printf(GP_DEBUG_LEVEL_INFORMATIONAL, __FILE__, __LINE__, GP_COLOR_BLUE    fmt GP_COLOR_RESET,  ##__VA_ARGS__);} while (0);
#define gp_debug(fmt, ...) do {gp_printf(GP_DEBUG_LEVEL_DEBUG,         __FILE__, __LINE__, GP_COLOR_MAGENTA fmt GP_COLOR_RESET,  ##__VA_ARGS__);} while (0);

#endif /*DEBUG_PRINT_H_*/
