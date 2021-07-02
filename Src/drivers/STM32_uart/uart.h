/*
 * uart.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#ifndef DRIVERS_STM32_UART_UART_H_
#define DRIVERS_STM32_UART_UART_H_

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#include <stm32h7_startup.h>
#endif

#include <drivers/STM32_gpio/gpio.h>

/*
There is lots of confusion around UART vs USART, more generically referred to as
UxART. UART is an asynchronous mode of communication, while USART is a
synchronous mode. Some hardware pins/interfaces on the MCU are labelled as
USART, meaning they can function as either a UART (asynchronous mode) or a USART
(synchronous mode). Some pins/interfaces are labelled as UART, meaning they can
only function as a UART (asynchronous mode).

Looking at the STM32H7 HAL/LL drivers manual (UM2217) for example, we see that
the HAL has completely separate libraries and functions for UART vs. USART.
Therefore, this library only implements UART mode (asynchronous). If we want to
use USART mode (synchronous) in the future, we can implement a separate library
for it.

UART supports a "FIFO mode" which is disabled by default. If this mode is
enabled, the MCU buffers multiple bytes for UART TX and RX. In this mode, you
can receive an interrupt when the number of empty slots (for TX) or the number
of full slots (for RX) reaches a specified threshold value. See reference manual
(RM0433), p.2046. We choose not to use this mode (TODO might want this?).

https://www.st.com/content/ccc/resource/training/technical/product_training/group0/b1/26/c3/87/d8/7a/42/27/STM32H7-Peripheral-USART_interface_USART/files/STM32H7-Peripheral-USART_interface_USART.pdf/_jcr_content/translations/en.STM32H7-Peripheral-USART_interface_USART.pdf
 */

#define UART_TX_BUF_SIZE 80
#define UART_RX_BUF_SIZE 80

typedef struct {
    // HAL control
	UART_HandleTypeDef handle;
    DMA_HandleTypeDef tx_dma_handle;
    DMA_HandleTypeDef rx_dma_handle;

	// GPIO pins
	GPIO_ALT tx;
	GPIO_ALT rx;
	GPIO_ALT de;

	// Store a buffer of bytes (characters) to send in this UART struct
	// Want it to be in the UART struct instead of the Log struct because we
	// expect to have many Log structs for each UART struct, so having a buffer
	// in each Log struct would be a big waste of memory
	// TODO - can this buffer be variable sized?
	char tx_buf[UART_TX_BUF_SIZE];
	// Buffer for receiving bytes by interrupt
	char rx_buf[UART_RX_BUF_SIZE];
} UART;

extern UART *g_uart_usart3;

void uart_init(UART* uart,
        USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin);
void uart_init_with_rs485(UART* uart,
        USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin,
		GPIO_TypeDef *de_port, uint16_t de_pin);

void uart_write(UART *uart, uint8_t *buf, uint32_t count);
void uart_write_dma(UART *uart, uint8_t *buf, uint32_t count);

void uart_read(UART *uart);
uint32_t uart_get_rx_count(UART *uart);

#endif /* DRIVERS_STM32_UART_UART_H_ */
