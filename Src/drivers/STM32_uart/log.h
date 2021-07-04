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

// See common.h for enum and struct definitions

extern Log *g_log_def;

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
