/*
 * G474REConfig.h
 *
 *  Created on: Jul. 27, 2021
 *      Author: bruno
 * 
 * I/O and settings configuration information for the STM32G474RE dev kit.
 */

#ifndef NUCLEO_G474RE_G474RECONFIG_H_
#define NUCLEO_G474RE_G474RECONFIG_H_

// General
#define G474RE_BOARD	MCU_BOARD_NUCLEO_G474RE

// UART
#define G474RE_UART_INST    LPUART1
#define G474RE_UART_TX_PORT	GPIOA
#define G474RE_UART_TX_PIN  GPIO_PIN_2
#define G474RE_UART_RX_PORT GPIOA
#define G474RE_UART_RX_PIN  GPIO_PIN_3

#if defined(STM32G4)
// Real mapping
#define G474RE_UART_AF      GPIO_AF8_LPUART1    // Might be GPIO_AF12_LPUART1 instead
#elif defined(STM32H7)
// Doesn't matter what this is, just need it to compile properly since
// GPIO_AF8_LPUART1 does not exist in the H7 HAL
#define G474RE_UART_AF      GPIO_AF8_LPUART
#endif

// User button
#define G474RE_BLUE_BUTTON_PORT	GPIOC
#define G474RE_BLUE_BUTTON_PIN 	GPIO_PIN_13

// Green LED
#define G474RE_GREEN_LED_PORT	GPIOA
#define G474RE_GREEN_LED_PIN 	GPIO_PIN_5

#endif /* NUCLEO_G474RE_G474RECONFIG_H_ */
