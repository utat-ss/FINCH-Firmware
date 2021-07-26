/*
 * nucleo_h743zi2.h
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#ifndef NUCLEO_H743ZI2_NUCLEO_H743ZI2_H_
#define NUCLEO_H743ZI2_NUCLEO_H743ZI2_H_

#include <common/stm32/uart/log.h>

// I/O configuration information for the STM32H743 dev kit

#define H743ZI2_BOARD			MCU_BOARD_NUCLEO_H743ZI2

#define H743ZI2_UART_INST 		USART3
#define H743ZI2_UART_AF  		GPIO_AF7_USART3
#define H743ZI2_UART_TX_PORT    GPIOD
#define H743ZI2_UART_TX_PIN     GPIO_PIN_8
#define H743ZI2_UART_RX_PORT    GPIOD
#define H743ZI2_UART_RX_PIN     GPIO_PIN_9


// Struct to represent the MCU on the dev kit and all peripherals/configuration
typedef struct {
	MCU mcu;
	UART uart;
	Log log;
} H743ZI2;

void h743zi2_init(H743ZI2 *dev_kit);
void h743zi2_init_uart(UART *uart, MCU *mcu);

#endif /* NUCLEO_H743ZI2_NUCLEO_H743ZI2_H_ */
