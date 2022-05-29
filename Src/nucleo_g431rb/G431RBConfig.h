/*
G431RBConfig.h

Date Created: May 28, 2022
Last Modified: May 28, 2022

I/O and settings configuration information for STM32G431RB dev kit. 

Author: Eric Leus
*/

#ifndef NUCLEO_G431RB_G431RBCONFIG_H_
#define NUCLEO_G431RB_G431RBCONFIG_H_

// General
#define G431RB_BOARD    MCU_BOARD_NUCLEO_G431RB

// UART
#define G431RB_UART_INST    LPUART1
#define G431RB_UART_TX_PORT GPIOA
#define G431RB_UART_TX_PIN  GPIO_PIN_2
#define G431RB_UART_RX_PORT GPIOA
#define G431RB_UART_RX_PIN  GPIO_PIN_3

#if defined(STM32G4)
#define G431RB_UART_AF      GPIO_AF12_LPUART1
#elif defined(STM32H7)
// GPIO_AF12_LPUART1 does not exist in H7 HAL
#define G431RB_UART_AF      GPIO_AF8_LPUART 
#endif

// User button
#define G431RB_BLUE_BUTTON_PORT GPIOC
#define G431RB_BLUE_BUTTON_PIN  GPIO_PIN_13

// Green LED
#define G431RB_GREEN_LED_PORT   GPIOA
#define G431RB_GREEN_LED_PIN    GPIO_PIN_5

#endif /* NUCLEO_G431RB_G431RBCONFIG_H_ */