/*
 * gpio_input.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_input.h>
#include <common/stm32/uart/log.h>

/*
Initialize a GPIO pin to input state
@param GPIOInput *gpio - a struct of the initialized pin to be used in
						 other gpio functions
@param MCU *mcu - initialized MCU struct
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint32_t pull - the internal pull-up/down gpio resistor state; possible
					   values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
 */
void gpio_input_init(GPIOInput *gpio, MCU *mcu, GPIO_TypeDef *port,
        uint16_t pin, uint32_t pull) {
    // Create GPIO struct
    gpio->mcu = mcu;
    gpio->port = port;
    gpio->pin = pin;

    // Initialize pin with HAL library
    GPIO_InitTypeDef init;
    init.Pin = pin;
    init.Pull = pull;
    init.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(port, &init);
}

/*
Return the input value of a pin
@param GPIOInput *gpio - the pin's GPIOInput struct
@return GPIO_PinState state - the pin's GPIO_PinState; value will be either
							 GPIO_PIN_RESET or GPIO_PIN_SET
 */
GPIO_PinState gpio_read(GPIOInput *gpio) {
    return HAL_GPIO_ReadPin(gpio->port, gpio->pin);
}

/*
Check if an input pin is low
@param GPIOInput *gpio - GPIO input
@return bool - true if the input is low, false if the input is high
 */
bool gpio_is_low(GPIOInput *gpio) {
    return gpio_read(gpio) == GPIO_PIN_RESET;
}

/*
Check if an input pin is high
@param GPIOInput *gpio - GPIO input
@return bool - true if the input is high, false if the input is low
 */
bool gpio_is_high(GPIOInput *gpio) {
    return gpio_read(gpio) == GPIO_PIN_SET;
}

/*
Wait for an input pin to change to a particular state (either low or high)
@param GPIOInput *gpio - GPIO input
@param GPIO_PinState state - the pin state to wait for; value will be either
							 GPIO_PIN_RESET or GPIO_PIN_SET
@param uint32_t timeout_ms - timeout (maximum time to wait) in ms
@return bool - true if the input went to the state before the timeout expired,
               false if the input never went to the state so it timed out
 */
bool gpio_wait_for_state(GPIOInput *gpio, GPIO_PinState state,
        uint32_t timeout_ms) {
    uint32_t start = HAL_GetTick();
    while (gpio_read(gpio) != state) {
        if (HAL_GetTick() > start + timeout_ms) {
            if (g_log_def != NULL) {
                warning(g_log_def, "GPIO wait for state timed out");
            }
            return false;
        }
    }

    return true;
}

/*
Wait for an input pin to change to low
@param GPIOInput *gpio - GPIO input
@param uint32_t timeout_ms - timeout (maximum time to wait) in ms
@return bool - true if the input went low before the timeout expired,
               false if the input never went low so it timed out
 */
bool gpio_wait_for_low(GPIOInput *gpio, uint32_t timeout_ms) {
    return gpio_wait_for_state(gpio, GPIO_PIN_RESET, timeout_ms);
}

/*
Wait for an input pin to change to high
@param GPIOInput *gpio - GPIO input
@param uint32_t timeout_ms - timeout (maximum time to wait) in ms
@return bool - true if the input went high before the timeout expired,
               false if the input never went high so it timed out
 */
bool gpio_wait_for_high(GPIOInput *gpio, uint32_t timeout_ms) {
    return gpio_wait_for_state(gpio, GPIO_PIN_SET, timeout_ms);
}
