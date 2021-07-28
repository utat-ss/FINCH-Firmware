/*
 * h743zi2_config.h
 *
 *  Created on: Jul. 27, 2021
 *      Author: bruno
 */

#ifndef NUCLEO_H743ZI2_H743ZI2_CONFIG_H_
#define NUCLEO_H743ZI2_H743ZI2_CONFIG_H_

// I/O and settings configuration information for the STM32H743 dev kit

// General
#define H743ZI2_BOARD	MCU_BOARD_NUCLEO_H743ZI2

// UART
#define H743ZI2_UART_INST 		USART3
#define H743ZI2_UART_AF  		GPIO_AF7_USART3
#define H743ZI2_UART_TX_PORT    GPIOD
#define H743ZI2_UART_TX_PIN     GPIO_PIN_8
#define H743ZI2_UART_RX_PORT	GPIOD
#define H743ZI2_UART_RX_PIN     GPIO_PIN_9

// User button
#define H743ZI2_BLUE_BUTTON_PORT	GPIOC
#define H743ZI2_BLUE_BUTTON_PIN		GPIO_PIN_13

// Red LED
#define H743ZI2_RED_LED_PORT	GPIOB
#define H743ZI2_RED_LED_PIN		GPIO_PIN_14

#endif /* NUCLEO_H743ZI2_H743ZI2_CONFIG_H_ */
