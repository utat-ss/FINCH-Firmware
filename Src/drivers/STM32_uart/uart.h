/*
 * usart.h
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
 */
typedef struct {
	UART_HandleTypeDef handle;
	GPIO_ALT tx;
	GPIO_ALT rx;
	GPIO_ALT de;
} UART;


#endif /* DRIVERS_STM32_UART_UART_H_ */
