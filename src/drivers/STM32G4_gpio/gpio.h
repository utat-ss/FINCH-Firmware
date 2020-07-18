/*
 * gpio.h
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 */

#ifndef DRIVERS_STM32G4_GPIO_GPIO_H_
#define DRIVERS_STM32G4_GPIO_GPIO_H_

#include "../../Core/Inc/main.h"

// Example constant
#define PA1 1
#define OUTPUT 1
#define INPUT 0

// Example object
typedef struct {
	void (*gpio_init)(GPIO_TypeDef *port);
	GPIO_TypeDef *port;
	int value;
} pin_func;

/* Functions */
void gpio_init(GPIO_TypeDef *port);
void gpio_set_direction(GPIO_TypeDef *port, uint16_t pin, uint8_t value);
void gpio_set_pin(GPIO_TypeDef *port, uint16_t pin, uint8_t value);
void gpio_set_pin_high(GPIO_TypeDef *port, uint16_t pin);
void gpio_set_pin_low(GPIO_TypeDef *port, uint16_t pin);
void gpio_toggle_pin(GPIO_TypeDef *port, uint16_t pin);

#endif /* DRIVERS_STM32G4_GPIO_GPIO_H_ */
