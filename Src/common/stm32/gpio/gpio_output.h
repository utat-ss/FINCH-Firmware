/*
 * gpio_output.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIO_OUTPUT_H_
#define COMMON_STM32_GPIO_GPIO_OUTPUT_H_

#include <common/stm32/mcu/mcu.h>

typedef struct {
    MCU *mcu;
    GPIO_TypeDef *port;
    uint16_t pin;
} GPIOOutput;

void gpio_output_init(GPIOOutput *gpio, MCU *mcu, GPIO_TypeDef *port,
        uint16_t pin, uint32_t mode, uint32_t pull, GPIO_PinState state);

void gpio_set(GPIOOutput *gpio, GPIO_PinState state);
void gpio_set_low(GPIOOutput *gpio);
void gpio_set_high(GPIOOutput *gpio);
void gpio_toggle(GPIOOutput *gpio);

#endif /* COMMON_STM32_GPIO_GPIO_OUTPUT_H_ */
