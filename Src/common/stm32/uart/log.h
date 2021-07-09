/*
 * log.h
 *
 *  Created on: May 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_UART_LOG_H_
#define COMMON_STM32_UART_LOG_H_

#include <common/stm32/uart/uart.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*
 * A Log is a struct that controls logging messages for a specific
 * purpose/function. Each Log has its own "log level" which adjusts the level of
 * verbosity (detail) printed over UART.
 *
 * There can be (and are expected to be) multiple Log structs for one UART
 * struct (connection). For example, you may have one Log that displays
 * information for SPI communication, one Log that displays information for an
 * SD card, and another Log that displays information on heater control.
 * Configuring different Log structs with different log levels is very useful
 * for debugging specific libraries/features in your code and controlling the
 * amount of operational detail logged.
 *
 * Each Log has its current "log level", which controls which logging messages
 * are actually displayed over UART. Only messages with the same or higher log
 * level are displayed. For example, if the current log level is INFO, only
 * messages that are INFO, WARNING, or ERROR are displayed over UART.
 */

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

#endif /* COMMON_STM32_UART_LOG_H_ */
