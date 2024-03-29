/*
 * GPIOInput.h
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#ifndef COMMON_STM32_GPIO_GPIOINPUT_H_
#define COMMON_STM32_GPIO_GPIOINPUT_H_

#include <common/stm32/mcu/MCU.h>
#include <stdbool.h>

typedef struct {
    MCU* mcu;
    GPIO_TypeDef* port;
    uint16_t pin;
} GPIOInput;

void gpio_input_init(GPIOInput* gpio, MCU* mcu, GPIO_TypeDef* port,
        uint16_t pin, uint32_t pull);

GPIO_PinState gpio_read(GPIOInput* gpio);
bool gpio_is_low(GPIOInput* gpio);
bool gpio_is_high(GPIOInput* gpio);
bool gpio_wait_for_state(GPIOInput* gpio, GPIO_PinState state,
        uint32_t timeout_ms);
bool gpio_wait_for_low(GPIOInput* gpio, uint32_t timeout_ms);
bool gpio_wait_for_high(GPIOInput* gpio, uint32_t timeout_ms);

#endif /* COMMON_STM32_GPIO_GPIOINPUT_H_ */
