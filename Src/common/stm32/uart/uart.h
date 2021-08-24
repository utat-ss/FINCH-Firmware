/*
 * uart.h
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#ifndef COMMON_STM32_UART_UART_H_
#define COMMON_STM32_UART_UART_H_

#include <common/stm32/uart/uart_log.h>


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
enabled (RM0433 p.2046), the MCU buffers multiple bytes for UART TX and RX. Each
FIFO's size is 16 words (RM0433 p.2041).

We choose to use FIFO mode for a potential performance improvement by buffering
multiple bytes when appropriate. We are using DMA mode for UART, so the
programmed thresholds for interrupts should not have any effect on our operation
since we do not use those interrupts.

From documentation (RM0433 p.2080-2082), it seems UART + DMA does not wait for
any threshold or multiple bytes before transferring, it simply transfers one
byte whenever possible (TX FIFO is not full or RX FIFO is not empty). Since our
number of bytes to transfer varies between transfers (and is not known in
advance for RX), this suits our application.

Important note for DMA transfers: The buffer/array in memory MUST be accessible
by DMA1 for the DMA transfer to work. For example, the STM32H743 has DTCM RAM
which is not accessible by DMA1/DMA2 (only by the MDMA). If you try to use DMA1
with a buffer in DTCM, a DMA transfer error is generated. Sometimes, a project
generated by STM32CubeIDE puts .data, .bss, and/or the stack in DTCM by default,
so be careful!

DMA has its own FIFO mode that is separate from UART's FIFO mode. For DMA's FIFO
mode, there is a programmed threshold (multiple bytes) for triggering transfers
(RM0433 p.640-641,648-649). For TX, it only transfers from memory to the FIFO
when the FIFO is at or below the threshold. For RX, it only transfers from the
FIFO to memory when the FIFO is at or above the threshold.

We use the DMA FIFO mode for TX, but not RX. For TX, we know exactly how many
bytes are to be transferred and program the DMA with that number, so the DMA
transfers all of them, even if it is not a multiple of the FIFO threshold. For
RX, we use the buffer size to program the DMA with the number of bytes to
transfer (max possible number of RX bytes), but the actual number of bytes
transferred is less than that and is unknown in advance. As a result, even with
a 1/4 FIFO threshold, the number of RX bytes must be a multiple of 4 for the DMA
to transfer all of the bytes to the memory buffer. Because we want it to work
for any number of RX bytes, we disable FIFO mode for the RX DMA.

Useful links:

UART:
https://github.com/MaJerle/stm32-usart-uart-dma-rx-tx
https://stackoverflow.com/questions/48216145/dma-uart-buffer-pointer
https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/

DMA errors:
https://www.reddit.com/r/embedded/comments/95u7on/problem_with_dma_stm32h7/
https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices
https://electronics.stackexchange.com/questions/389830/tim2-dma-configuration-for-stm32h7
https://stackoverflow.com/questions/51323589/are-all-ram-sections-used-in-this-linkerscript-of-a-stm32h743-microcontroller-c
https://electronics.stackexchange.com/questions/393868/stm32f4-dma-transfer-error-with-adc
 */


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
