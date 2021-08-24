/*
 * gpio_it_input.h
 *
 *  Created on: Aug 6, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_IT_INPUT_H_
#define COMMON_STM32_GPIO_GPIO_IT_INPUT_H_

#include <common/stm32/gpio/gpio_input.h>

// Number of interrupt lines, equal to the number of GPIO pins per port
#define GPIO_IT_INPUT_EXTI_COUNT 16

// Interrupt input callback type
// This syntax is weird, but this defines a type called`GPIOITInputCB`
// that points to a function with signature `void function_name(void)`
typedef void (*GPIOITInputCB)(void);

typedef struct {
    // Because an interrupt pin can also function as a normal GPIO input, store
    // a GPIOInput struct so that application code can call functions from
    // gpio_input.c that take a GPIOInput argument
    GPIOInput input;
} GPIOITInput;

void gpio_it_input_init(GPIOITInput* gpio, MCU* mcu, GPIO_TypeDef* port,
        uint16_t pin, uint32_t mode, uint32_t pull, GPIOITInputCB callback);

#endif /* COMMON_STM32_GPIO_GPIO_IT_INPUT_H_ */
