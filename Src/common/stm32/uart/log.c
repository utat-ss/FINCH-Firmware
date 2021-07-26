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

void log_init(Log *log, MCU *mcu, UART *uart) {
	log->mcu = mcu;
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
 * Calls strncat() with the TX buffer as the destination, calculating the
 * correct maximum number of characters to copy.
 */
void log_strncat_tx(Log *log, const char *source) {
	// Note that the third argument to strncat() is NOT the total size of
	// the destination buffer, but rather it is the maximum number of
	// characters (NOT including the null character) to copy from the source
	// to the destination

	// To calculate that number, take the buffer size, subtract 1 (for the
	// new null character), then subtract the length of the string already
	// in the buffer (not including the existing null character)
	size_t count = (sizeof(log->uart->tx_buf) - 1) - strlen(log->uart->tx_buf);

	// Note size_t is an unsigned integer
	// If the TX buffer is full, i.e.
	// strlen(log->uart->tx_buf) = sizeof(log->uart->tx_buf) - 1,
	// count will evaluate to 0 so we shouldn't have to worry about underflow

	strncat(log->uart->tx_buf, source, count);
}

/*
 * This function is never meant to be called directly by other code outside this
 * library
 * Can't just name the function `log` because that function exists in the math
 * library
 * Need to make sure the linker has the -u_printf_float flag enabled, or else
 * printing floating-point values does not work
 * (https://community.st.com/s/question/0D50X0000B0AEJb/uprintffloat-where-to-set-for-clinker-in-stm32cubeide)
 * Note that in the compiled binary, the -u_printf_float flag adds about
 * 6,524 bytes to `text` and 364 bytes to `data`
 */
void log_log(Log *log, LogLevel level, const char *format, va_list args) {
	// Only send this message over UART if the log level is high enough or the
    // global log level is high enough
	if (log->level >= level || g_log_global_level >= level) {
		// Do some magic with variable arguments
		// - `format` is the format string with placeholders for values
		// - `args` are the variable argument values
		// - `main_buf` is the buffer where the resulting string is stored, i.e.
		//   the format string with the placeholders replaced by the values of
		//   the variable arguments
		// - `sizeof(main_buf)` is the size of the buffer, i.e. the number of
		//   bytes (characters) it can hold
		// - Use `vsnprintf` instead of `vsprintf` so we can specify the size of
		//   the buffer (using `sizeof`) to ensure we do not write past the
		//   buffer boundary and overflow it
		// - This automatically adds a terminating nul ('\0') character at the
		//   appropriate place in the buffer
		char main_buf[UART_TX_BUF_SIZE];
		vsnprintf(main_buf, sizeof(main_buf), format, args);

		// Must wait until the previous TX DMA transfer has completed
		// (see uart_write_data() in uart.c for more details)
		// If we write to log->uart->tx_buf before the previous transfer has
		// completed, it overwrites the previous data and corrupts the bytes
		// that have not been sent out yet by the previous transfer
		uint32_t start = HAL_GetTick();
        while (log->uart->handle.gState != HAL_UART_STATE_READY) {
            // 100ms timeout (this should never happen)
            if (HAL_GetTick() > start + 100) {
                Error_Handler();
                return;
            }
        }

		// Start the string in the buffer with the current system (tick) time
		// This operates very similar to `vsnprintf` (see above) except we pass
		// in the variable arguments directly rather than as a `va_list`
		snprintf(log->uart->tx_buf, sizeof(log->uart->tx_buf),
		        "%lums: ", HAL_GetTick());

		// Add the string for the message's log level
		log_strncat_tx(log, log_get_level_string(level));

		// Add a colon and space after the message's log level
		log_strncat_tx(log, ": ");

		// Add the main string
		log_strncat_tx(log, main_buf);

		// Add a newline after the message
		// \r is also called CR, while \n is also called LF
		// Normally we would only need \n, but we choose to include \r as well
		// because if you only use \n, some serial monitors (viewers) go to the
		// next line but do not reset the cursor all the way to the left
		log_strncat_tx(log, "\r\n");

		// Now that the actual characters/bytes we want to send over UART are
		// ready in `log->uart->buf`, send them over UART
		// (using DMA so we don't have to wait in this function until it's done)
		// Number of characters from the buffer to actually send is determined
		// using `strlen` since there is a \0 character in the buffer
		uart_write_dma(log->uart, (uint8_t*) log->uart->tx_buf,
		        strlen(log->uart->tx_buf));

		// In the future, could modify this implementation so it doesn't have to
		// wait for the previous DMA transfer to finish before starting the next
		// one
		// Could make it pause the previous DMA transfer, discard bytes from the
		// buffer that have already been transmitted, append the bytes of the
		// new message to the buffer, then start a new DMA transfer
	}
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
