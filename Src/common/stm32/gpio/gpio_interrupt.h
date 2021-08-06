/*
 * gpio_interrupt.h
 *
 *  Created on: Aug 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_INTERRUPT_H_
#define COMMON_STM32_GPIO_GPIO_INTERRUPT_H_

#include <common/stm32/gpio/gpio_input.h>

// Number of interrupt lines, equal to the number of GPIO pins per port
#define GPIO_INTERRUPT_NUM_EXTI_LINES 16

// This syntax is weird, but this defines a type called`GPIOInterruptCallback`
// that points to a function with signature `void function_name(void)`
typedef void (*GPIOInterruptCallback)(void);

typedef struct {
	// Because an interrupt pin can also function as a normal GPIO input, store
	// a GPIOInput struct so that application code can call functions from
	// gpio_input.c that take a GPIOInput argument
	GPIOInput input;
} GPIOInterrupt;

void gpio_interrupt_init(GPIOInterrupt *gpio, MCU *mcu, GPIO_TypeDef *port,
		uint16_t pin, uint32_t mode, uint32_t pull,
		GPIOInterruptCallback callback);

#endif /* COMMON_STM32_GPIO_GPIO_INTERRUPT_H_ */
