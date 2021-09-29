/*
 * Log.h
 *
 *  Created on: May 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_UART_LOG_H_
#define COMMON_STM32_UART_LOG_H_

#include <common/stm32/uart/UART.h>
#include <stdarg.h>


// See uart_log.h for enum and struct definitions

extern Log* g_log_def;


void log_init(Log* log, UART* uart);
void log_set_level(Log* log, LogLevel level);
void log_set_global_level(LogLevel level);

void log_log(Log* log, LogLevel level, char* format, va_list args);
// Chose not to prefix these names with "log_" because they are incredibly
// commonly used
void error(Log* log, char* format, ...);
void warning(Log* log, char* format, ...);
void info(Log* log, char* format, ...);
void debug(Log* log, char* format, ...);
void verbose(Log* log, char* format, ...);

void error_bytes(Log* log, uint8_t* bytes, uint32_t count,
        char* prefix_format, ...);
void warning_bytes(Log* log, uint8_t* bytes, uint32_t count,
        char* prefix_format, ...);
void info_bytes(Log* log, uint8_t* bytes, uint32_t count,
        char* prefix_format, ...);
void debug_bytes(Log* log, uint8_t* bytes, uint32_t count,
        char* prefix_format, ...);
void verbose_bytes(Log* log, uint8_t* bytes, uint32_t count,
        char* prefix_format, ...);

#endif /* COMMON_STM32_UART_LOG_H_ */
