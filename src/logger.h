#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
} LogLevel;

void set_log_level(LogLevel level);
void log_message(const char *format, ...);
void log_error_v(const char *format, ...);
void log_warning_v(const char *format, ...);
void log_info_v(const char *format, ...);
void log_debug_v(const char *format, ...);

#define log_error(...) log_error_v(__VA_ARGS__)
#define log_warning(...) log_warning_v(__VA_ARGS__)
#define log_info(...) log_info_v(__VA_ARGS__)
#define log_debug(...) log_debug_v(__VA_ARGS__)

#endif
