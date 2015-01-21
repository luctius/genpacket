#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug_print.h"

enum debug_levels global_debug_lvl = GP_DEBUG_LEVEL_DEBUG;

void gp_printf(enum debug_levels dbg_lvl, const char *module, int line, const char* format, ... ) {
    va_list args;
    va_start( args, format );
    if (isatty(fileno(stdout))) gp_printf_basic(dbg_lvl, module, line, format, args);
    else gp_printf_basic(dbg_lvl, module, line, format, args);
    va_end( args );
}

void gp_printf_basic(enum debug_levels dbg_lvl, const char *module, int line, const char* format,  va_list args) {
    FILE *fd = stderr;
    const char *pre_format;

    if (dbg_lvl > global_debug_lvl)  return;

    if (dbg_lvl >= GP_DEBUG_LEVEL_DEBUG) {
        fd = stdout;
        if (isatty(fileno(stdout))) pre_format = GP_COLOR_RESET "[%s:%d][" GP_COLOR_MAGENTA "Debug"  GP_COLOR_RESET "] ";
        else pre_format = "[%s:%d][Debug] ";
    }
    else if (dbg_lvl >= GP_DEBUG_LEVEL_INFORMATIONAL) {
        fd = stdout;
        if (isatty(fileno(stdout))) pre_format = GP_COLOR_RESET "[%s:%d][" GP_COLOR_BLUE "Info"  GP_COLOR_RESET "] ";
        else pre_format = "[%s:%d] ";
    }
    else if (dbg_lvl >= GP_DEBUG_LEVEL_WARNING) {
        fd = stderr;
        if (isatty(fileno(stdout))) pre_format = GP_COLOR_RESET "[%s:%d][" GP_COLOR_YELLOW "Warn"  GP_COLOR_RESET "] ";
        else pre_format = "[%s:%d]" "[Warning] "; /* should be cyan*/
    }
    else if (dbg_lvl >= GP_DEBUG_LEVEL_ERROR) {
        fd = stderr;
        if (isatty(fileno(stdout))) pre_format = GP_COLOR_RESET "[%s:%d][" GP_COLOR_RED "Error"  GP_COLOR_RESET "] ";
        else pre_format = "[%s:%d" "[Error] "; /* should be red*/
    }
    else {
        fd = stdout;
        if (isatty(fileno(stdout))) pre_format = GP_COLOR_RESET "[%s:%d][" GP_COLOR_BOLD "???"  GP_COLOR_RESET "] ";
        else pre_format = "[%s:%d" "[Unknown] ";
    }

    fprintf(fd, pre_format, module, line);
    vfprintf(fd, format, args);
    if (isatty(fileno(stdout))) fprintf(fd, GP_COLOR_RESET );
    fprintf(fd, "\n");
}

