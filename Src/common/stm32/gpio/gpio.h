/*
 * gpio.h
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 */

#ifndef DRIVERS_STM32_GPIO_GPIO_H_
#define DRIVERS_STM32_GPIO_GPIO_H_

#include <stdint.h>

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#endif

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIO_INPUT;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t mode;
	uint16_t pin;
} GPIO_OUTPUT;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t mode;
	uint16_t pin;
	uint8_t alternate;
} GPIO_ALT;

/* Functions */
GPIO_OUTPUT gpio_init_output(GPIO_TypeDef *port, uint16_t pin);
GPIO_OUTPUT gpio_init_output_od(GPIO_TypeDef *port, uint16_t pin, uint32_t pull);
GPIO_ALT gpio_init_alt(GPIO_TypeDef *port, uint16_t pin, uint8_t alt);
GPIO_ALT gpio_init_alt_od(GPIO_TypeDef *port, uint16_t pin, uint32_t pull, uint8_t alt);
GPIO_INPUT gpio_init_input(GPIO_TypeDef *port, uint16_t pin);
void gpio_set_high(GPIO_OUTPUT gpio);
void gpio_set_low(GPIO_OUTPUT gpio);
void gpio_set_value(GPIO_OUTPUT gpio, GPIO_PinState state);
void gpio_toggle(GPIO_OUTPUT gpio);
GPIO_PinState gpio_read(GPIO_INPUT gpio);

#endif /* DRIVERS_STM32_GPIO_GPIO_H_ */
