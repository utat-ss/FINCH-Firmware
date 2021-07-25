/*
 * nucleo_g474re.h
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#ifndef NUCLEO_G474RE_NUCLEO_G474RE_H_
#define NUCLEO_G474RE_NUCLEO_G474RE_H_

#include <common/stm32/mcu/mcu.h>

// I/O configuration information for the STM32G474 dev kit

#define G474RE_BOARD			MCU_BOARD_NUCLEO_G474RE

#define G474RE_UART_INST       	LPUART1
#define G474RE_UART_TX_PORT    	GPIOA
#define G474RE_UART_TX_PIN     	GPIO_PIN_2
#define G474RE_UART_RX_PORT    	GPIOA
#define G474RE_UART_RX_PIN     	GPIO_PIN_3

#if defined(STM32G4)
#define G474RE_UART_AF        	GPIO_AF8_LPUART1	// Might be GPIO_AF12_LPUART1 instead
#elif defined(STM32H7)
// Doesn't matter what this is, just need it to compile properly since
// GPIO_AF8_LPUART1 does not exist in the H7 HAL
#define G474RE_UART_AF        	GPIO_AF8_LPUART
#endif


// Struct to represent the MCU on the dev kit and all peripherals/configuration
typedef struct {
	MCU mcu;
	UART uart;
	Log log;
} G474RE;

void g474re_init(G474RE *dev_kit);
void g474re_init_uart(UART *uart);

#endif /* NUCLEO_G474RE_NUCLEO_G474RE_H_ */
