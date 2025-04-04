#include "logger.h"
#include <stdlib.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static LogLevel current_log_level = LOG_LEVEL_INFO;

void set_log_level(LogLevel level) {
    current_log_level = level;
}

void log_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void log_error_v(const char *format, ...) {
    if (current_log_level <= LOG_LEVEL_ERROR) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "%sERROR: ", ANSI_COLOR_RED);
        vfprintf(stderr, format, args);
        fprintf(stderr, "%s\n", ANSI_COLOR_RESET);
        va_end(args);
    }
}

void log_warning_v(const char *format, ...) {
    if (current_log_level <= LOG_LEVEL_WARNING) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "%sWARNING: ", ANSI_COLOR_YELLOW);
        vfprintf(stderr, format, args);
        fprintf(stderr, "%s\n", ANSI_COLOR_RESET);
        va_end(args);
    }
}

void log_info_v(const char *format, ...) {
    if (current_log_level <= LOG_LEVEL_INFO) {
        va_list args;
        va_start(args, format);
        printf("%sINFO: ", ANSI_COLOR_BLUE);
        vprintf(format, args);
        printf("%s\n", ANSI_COLOR_RESET);
        va_end(args);
    }
}

void log_debug_v(const char *format, ...) {
    if (current_log_level <= LOG_LEVEL_DEBUG) {
        va_list args;
        va_start(args, format);
        printf("DEBUG: ");
        vprintf(format, args);
        printf("\n");
        va_end(args);
    }
}
