/*
 * gpio_input.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_INPUT_H_
#define COMMON_STM32_GPIO_GPIO_INPUT_H_

// TODO
#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif
#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#endif

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOInput;

void gpio_init_input(GPIOInput *input, GPIO_TypeDef *port, uint16_t pin,
		uint32_t pull);

GPIO_PinState gpio_read(GPIOInput *gpio);

#endif /* COMMON_STM32_GPIO_GPIO_INPUT_H_ */
