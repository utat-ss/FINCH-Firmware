/*
 * common.h
 *
 *  Created on: Jul. 4, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_UART_UART_LOG_H_
#define COMMON_STM32_UART_UART_LOG_H_

/*
 * Normally these definitions would be separated into uart.h and log.h, but
 * since the UART and Log structs refer to each other, it was impossible for it
 * to work with both header files including each other in a circular dependency.
 *
 * Must have Log defined before UART because a Log contains a pointer to a UART,
 * but a UART contains a Log variable. If UART is defined before Log, it
 * produces the error "field 'log' has incomplete type".
 * See explanation:
 * https://stackoverflow.com/questions/12466055/field-has-incomplete-type-error
 */

// -----------------------------------------------------------------------------

typedef enum {
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_VERBOSE = 5
} LogLevel;

// Must forward declare this before defining Log with a pointer to it
// Can't use the typedef'ed `UART` name
struct UARTStruct;

typedef struct {
    struct UARTStruct *uart;
    LogLevel level;
} Log;

// -----------------------------------------------------------------------------

// For TX, a buffer size of 80 is not sufficient for printing out error messages
// from assert_failed() with long file paths, so 160 should be sufficient
#define UART_TX_BUF_SIZE 160
#define UART_RX_BUF_SIZE 80

// Baud rate is just an integer number, but make it an enum to limit possible
// baud values to only those that are practically used (to prevent typos)
typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_115200 = 115200
} UARTBaud;

// TODO - variable sized buffers?
typedef struct UARTStruct {
	MCU *mcu;

    // HAL control
    UART_HandleTypeDef handle;
    DMA_HandleTypeDef tx_dma_handle;
    DMA_HandleTypeDef rx_dma_handle;

    // GPIO pins
    GPIOAltFunc tx_gpio;
    GPIOAltFunc rx_gpio;
    GPIOAltFunc de_gpio;

    // Store a buffer of bytes (characters) in this UART struct to be sent by
    // the TX DMA.
    // It should be in the UART struct instead of the Log struct because we
    // expect to have many Log structs for each UART struct, so having a buffer
    // in each Log struct would be a big waste of memory.
    char tx_buf[UART_TX_BUF_SIZE];
    // Buffer for receiving bytes through the RX DMA
    char rx_buf[UART_RX_BUF_SIZE];

    // Default Log struct for this UART
    Log log;

    // RX timeout (in ms)
    uint32_t rx_timeout_ms;
} UART;

// -----------------------------------------------------------------------------

#endif /* COMMON_STM32_UART_UART_LOG_H_ */
