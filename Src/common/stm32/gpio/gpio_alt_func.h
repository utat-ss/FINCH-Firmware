/*
 * gpio_alt_func.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_ALT_FUNC_H_
#define COMMON_STM32_GPIO_GPIO_ALT_FUNC_H_

#include <common/stm32/mcu/hal.h>

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOAltFunc;

void gpio_init_alt_func_pp(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed);
void gpio_init_alt_func_od(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed);

#endif /* COMMON_STM32_GPIO_GPIO_ALT_FUNC_H_ */