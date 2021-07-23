/*
 * gpio_output.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_OUTPUT_H_
#define COMMON_STM32_GPIO_GPIO_OUTPUT_H_

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
} GPIOOutput;

void gpio_init_output_pp(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin,
		GPIO_PinState state);
void gpio_init_output_od(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin, uint32_t pull,
		GPIO_PinState state);

void gpio_set_low(GPIOOutput *gpio);
void gpio_set_high(GPIOOutput *gpio);
void gpio_set(GPIOOutput *gpio, GPIO_PinState state);
void gpio_toggle(GPIOOutput *gpio);

#endif /* COMMON_STM32_GPIO_GPIO_OUTPUT_H_ */
