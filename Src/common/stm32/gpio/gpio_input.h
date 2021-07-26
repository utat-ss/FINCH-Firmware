/*
 * gpio_input.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_INPUT_H_
#define COMMON_STM32_GPIO_GPIO_INPUT_H_

#include <common/stm32/uart/log.h>

#include <stdbool.h>

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOInput;

void gpio_input_init(GPIOInput *gpio, GPIO_TypeDef *port, uint16_t pin,
		uint32_t pull);

GPIO_PinState gpio_read(GPIOInput *gpio);
bool gpio_is_low(GPIOInput *gpio);
bool gpio_is_high(GPIOInput *gpio);
bool gpio_wait_for_state(GPIOInput *gpio, GPIO_PinState state,
		uint32_t timeout_ms);
bool gpio_wait_for_low(GPIOInput *gpio, uint32_t timeout_ms);
bool gpio_wait_for_high(GPIOInput *gpio, uint32_t timeout_ms);

#endif /* COMMON_STM32_GPIO_GPIO_INPUT_H_ */
