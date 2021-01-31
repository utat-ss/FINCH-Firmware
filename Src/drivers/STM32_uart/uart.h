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
#endif

typedef struct {

	UART_HandleTypeDef handle;
	GPIO_ALT tx;
	GPIO_ALT rx;
	GPIO_ALT hw_enable;


} USART;


#endif /* DRIVERS_STM32_UART_UART_H_ */
