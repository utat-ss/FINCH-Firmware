/*
 * log.c
 *
 *  Created on: May 6, 2021
 *      Author: bruno
 */

#include <common/stm32/uart/log.h>

// Code with variable arguments is based on
// https://github.com/HeronMkII/lib-common/blob/master/src/uart/log.c

// This is the "default" Log struct
// It can be used in places where there is no individual Log available (e.g.
// interrupt handlers)
Log *g_log_def = NULL;

// Global log level
// This is not an override, but it can be used to force all Log structs to
// display more verbose output
LogLevel g_log_global_level = LOG_LEVEL_INFO;

void log_init(Log *log, UART *uart) {
	log->uart = uart;
	// Want a level of info by default
	log->level = LOG_LEVEL_INFO;
	// This will only be printed if the global log level is debug or higher
	debug(log, "Initialized individual log");

	if (g_log_def == NULL) {
	    g_log_def = log;
        info(g_log_def, "Set default log");
    }
}

char* log_get_level_string(LogLevel level) {
    if (level == LOG_LEVEL_NONE) {
        return "NONE";
    } else if (level == LOG_LEVEL_ERROR) {
        return "ERROR";
    } else if (level == LOG_LEVEL_WARNING) {
        return "WARNING";
    } else if (level == LOG_LEVEL_INFO) {
        return "INFO";
    } else if (level == LOG_LEVEL_DEBUG) {
        return "DEBUG";
    } else if (level == LOG_LEVEL_VERBOSE) {
        return "VERBOSE";
    } else {
        return "?";
    }
}

void log_set_level(Log *log, LogLevel level) {
    log->level = level;
    info(log, "Set individual log level to %s", log_get_level_string(level));
}

void log_set_global_level(LogLevel level) {
    g_log_global_level = level;
    info(g_log_def, "Set global log level to %s", log_get_level_string(level));
}

/*
 * Calls strncat(), calculating the maximum number of characters to copy.
 *
 * Calling sizeof(destination) within this function would not work because this
 * function only has a pointer to destination. It cannot know the size of the
 * array, so the caller must pass in sizeof(destination) as the
 * sizeof_destination argument.
 */
void log_safe_strncat(char *destination, size_t sizeof_destination,
		char *source) {
	// See https://www.cplusplus.com/reference/cstring/strncat/

	// Note that the third argument to strncat() is NOT the total size of
	// the destination buffer, but rather it is the maximum number of
	// characters (NOT including the null character) to copy from the source
	// to the destination

	// To calculate that number, take the buffer size, subtract 1 (for the
	// new null character), then subtract the length of the string already
	// in the buffer (not including the existing null character)
	size_t count = (sizeof_destination - 1) - strlen(destination);

	// Note size_t is an unsigned integer
	// If the TX buffer is full,
	// i.e. strlen(destination) == sizeof_destination - 1,
	// count will evaluate to 0 so we shouldn't have to worry about underflow

	strncat(destination, source, count);
}

/*
 * Writes a message (already formatted) to UART, prefixed with a timestamp and
 * log level and ended with a newline.
 *
 * Note this may not work correctly if you call it from an ISR (see
 * uart_write_dma()).
 */
void log_write_msg(Log *log, LogLevel level, char *msg) {
	// Must prepare our bytes in a separate buffer from the UART's TX buffer
	char buf[UART_TX_BUF_SIZE];

	// Start the string in the buffer with the current system (tick) time
	// This operates very similar to `vsnprintf` except we pass in the variable
	// arguments directly rather than as a `va_list`
	snprintf(buf, sizeof(buf), "%lums: ", HAL_GetTick());

	// Add the string for the message's log level
	log_safe_strncat(buf, sizeof(buf), log_get_level_string(level));

	// Add a colon and space after the message's log level
	log_safe_strncat(buf, sizeof(buf), ": ");

	// Add the main string (the message)
	log_safe_strncat(buf, sizeof(buf), msg);

	// Add a newline after the message
	// \r is also called CR, while \n is also called LF
	// Normally we would only need \n, but we choose to include \r as well
	// because if you only use \n, some serial monitors (viewers) go to the
	// next line but do not reset the cursor all the way to the left
	log_safe_strncat(buf, sizeof(buf), "\r\n");

	// Now that the actual characters/bytes we want to send over UART are
	// ready in `buf`, send them over UART
	// Use DMA mode instead of blocking mode so we don't have to wait in this
	// function until it's done
	// Number of characters from the buffer to actually send is determined
	// using `strlen` since there is a \0 character in the buffer
	// Note the uart_write_dma() function will copy the contents of `buf` to the
	// UART TX buffer, then transfer them over DMA
	uart_write_dma(log->uart, (uint8_t *) buf, strlen(buf));
}

/*
 * Returns true if a message with the specified log level should be written to
 * UART.
 */
bool log_should_write_msg(Log *log, LogLevel level) {
	// Only send the message over UART if the log level is high enough or the
	// global log level is high enough
	return log->level >= level || g_log_global_level >= level;
}

/*
 * This function is never meant to be called directly by other code outside this
 * library
 * Can't just name the function `log` because that function exists in the math
 * library
 *
 * Need to make sure the linker has the -u_printf_float flag enabled, or else
 * formatting floating-point values does not work
 * See Project Properties > C/C++ Build > Settings > Tool Settings > MCU Settings
 * Also see https://community.st.com/s/question/0D50X0000B0AEJb/uprintffloat-where-to-set-for-clinker-in-stm32cubeide
 * Note that in the compiled binary, the -u_printf_float flag adds about
 * 6,524 bytes to `text` and 364 bytes to `data`
 *
 * Note that %llu and %lld format specifiers do not appear to work, as they just
 * print "lu" and "ld" (without any numbers) respectively
 */
void log_log(Log *log, LogLevel level, char *format, va_list args) {
	// Do this check before calling vsnprintf in case the argument
	// parsing/formatting takes a long time
	// Can bail out early if the message won't be written to UART
	if (!log_should_write_msg(log, level)) {
		return;
	}

	// Do some magic with variable arguments
	// - `format` is the format string with placeholders for values
	// - `args` are the variable argument values
	// - `msg` is the buffer where the resulting string is stored, i.e.
	//   the format string with the placeholders replaced by the values of
	//   the variable arguments
	// - `sizeof(msg)` is the size of the buffer, i.e. the number of
	//   bytes (characters) it can hold
	// - Use `vsnprintf` instead of `vsprintf` so we can specify the size of
	//   the buffer (using `sizeof`) to ensure we do not write past the
	//   buffer boundary and overflow it
	// - This automatically adds a terminating nul ('\0') character at the
	//   appropriate place in the buffer
	char msg[UART_TX_BUF_SIZE];
	vsnprintf(msg, sizeof(msg), format, args);

	log_write_msg(log, level, msg);
}

void error(Log *log, char *format, ...) {
	// Some magic stuff here to retrieve and process the list of variable arguments
	va_list args;
	va_start(args, format);
	log_log(log, LOG_LEVEL_ERROR, format, args);
	va_end(args);
}

void warning(Log *log, char *format, ...) {
	va_list args;
	va_start(args, format);
	log_log(log, LOG_LEVEL_WARNING, format, args);
	va_end(args);
}

void info(Log *log, char *format, ...) {
	va_list args;
	va_start(args, format);
	log_log(log, LOG_LEVEL_INFO, format, args);
	va_end(args);
}

void debug(Log *log, char *format, ...) {
	va_list args;
	va_start(args, format);
	log_log(log, LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}

void verbose(Log *log, char *format, ...) {
	va_list args;
	va_start(args, format);
	log_log(log, LOG_LEVEL_VERBOSE, format, args);
	va_end(args);
}
