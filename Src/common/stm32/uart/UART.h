/*
 * UART.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#ifndef COMMON_STM32_UART_UART_H_
#define COMMON_STM32_UART_UART_H_

#include <common/stm32/uart/UARTLog.h>

// See uart_log.h for enum and struct definitions

// TX timeout (in ms)
// Conservative upper bound calculated assuming 9600 baud (~1 byte per ms),
// max 160 bytes per line, plus a lot of margin
#define UART_TX_TIMEOUT_MS 400

// Default baud rate (as high as reasonably possible)
// Choose 230,400 because it is the highest preset baud rate value in CoolTerm
// It is also a preset value in the terminal extension for STM32CubeIDE
#define UART_DEF_BAUD UART_BAUD_230400

// Baud rate is just an integer number, but make it an enum to limit possible
// baud values to only those that are practically used (to prevent typos)
typedef enum {
    UART_BAUD_9600 = 9600,
    UART_BAUD_115200 = 115200,
    UART_BAUD_230400 = 230400,
} UARTBaud;


void uart_init(UART* uart, MCU* mcu,
        USART_TypeDef* instance, UARTBaud baud, uint8_t alternate,
        GPIO_TypeDef* tx_port, uint16_t tx_pin,
        GPIO_TypeDef* rx_port, uint16_t rx_pin);
void uart_init_for_board(UART* uart, MCU* mcu);
void uart_init_with_rs485(UART* uart, MCU* mcu,
        USART_TypeDef* instance, UARTBaud baud, uint8_t alternate,
        GPIO_TypeDef* tx_port, uint16_t tx_pin,
        GPIO_TypeDef* rx_port, uint16_t rx_pin,
        GPIO_TypeDef* de_port, uint16_t de_pin);

void uart_set_baud(UART* uart, UARTBaud baud);

void uart_write(UART* uart, uint8_t* buf, uint32_t count);
void uart_write_dma(UART* uart, uint8_t* buf, uint32_t count);

void uart_restart_rx_dma(UART* uart);
uint32_t uart_get_rx_count(UART* uart);

uint32_t uart_read_uint(UART* uart, char* tx_format, ...);
int32_t uart_read_int(UART* uart, char* tx_format, ...);
double uart_read_double(UART* uart, char* tx_format, ...);
char uart_read_char(UART* uart, char* tx_format, ...);
void uart_wait_for_key_press(UART* uart);

#endif /* COMMON_STM32_UART_UART_H_ */
