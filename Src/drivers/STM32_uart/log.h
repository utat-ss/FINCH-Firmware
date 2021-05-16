/*
 * log.h
 *
 *  Created on: May 6, 2021
 *      Author: bruno
 */

#ifndef DRIVERS_STM32_UART_LOG_H_
#define DRIVERS_STM32_UART_LOG_H_

#include <drivers/STM32_uart/uart.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum {
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARNING = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_VERBOSE = 5
} LogLevel;

typedef struct {
	UART *uart;
	LogLevel level;
} Log;

extern Log g_log_def;
extern bool g_log_def_initialized;

void log_init(Log *log, UART *uart);
void log_set_level(Log *log, LogLevel level);
void log_set_global_level(LogLevel level);

// Chose not to prefix these names with "log_" because they are incredibly
// commonly used
void error(Log *log, char *format, ...);
void warning(Log *log, char *format, ...);
void info(Log *log, char *format, ...);
void debug(Log *log, char *format, ...);
void verbose(Log *log, char *format, ...);

#endif /* DRIVERS_STM32_UART_LOG_H_ */
