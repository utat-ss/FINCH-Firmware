/*
 * gpio.h
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 */

#ifndef COMMON_STM32_GPIO_GPIO_H_
#define COMMON_STM32_GPIO_GPIO_H_

#include <stdint.h>

#ifdef STM32G474xx
#include <stm32g4xx_hal.h>
#endif

#ifdef STM32H743xx
#include <stm32h7xx_hal.h>
#endif

// Don't need to save the GPIO_InitTypeDef struct in our GPIO structs because it
// is only used for HAL_GPIO_Init()
// All other HAL GPIO functions only require the GPIO_TypeDef and the pin

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOInput;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOOutput;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} GPIOAltFunc;

/* Functions */
void gpio_init_output_pp(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin,
		GPIO_PinState state);
void gpio_init_output_od(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin, uint32_t pull,
		GPIO_PinState state);
void gpio_init_alt_func_pp(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed);
void gpio_init_alt_func_od(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed);
void gpio_init_input(GPIOInput *input, GPIO_TypeDef *port, uint16_t pin,
		uint32_t pull);
void gpio_set_high(GPIOOutput *gpio);
void gpio_set_low(GPIOOutput *gpio);
void gpio_set(GPIOOutput *gpio, GPIO_PinState state);
void gpio_toggle(GPIOOutput *gpio);
GPIO_PinState gpio_read(GPIOInput *gpio);

#endif /* COMMON_STM32_GPIO_GPIO_H_ */
