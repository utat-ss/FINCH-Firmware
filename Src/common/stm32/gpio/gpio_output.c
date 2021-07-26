/*
 * gpio_output.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_output.h>

/*
Initialize a GPIO pin to output state
Push-pull pin output values are high or low.
Open-drain pin output values are high impedance or low.
@param GPIOOutput *gpio - a struct of the initialized pin to be used in
						  other gpio functions
@param MCU *mcu - initialized MCU struct
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint32_t mode - output mode; must be GPIO_MODE_OUTPUT_PP (push-pull) or
                       GPIO_MODE_OUTPUT_OD (open-drain)
@param uint32_t pull - the internal pull-up/down gpio resistor state;
                       for mode=GPIO_MODE_OUTPUT_PP, must be GPIO_NOPULL;
                       for mode=GPIO_MODE_OUTPUT_OD, must be GPIO_NOPULL or GPIO_PULLUP
@param GPIO_PinState state - initial state to set the pin to; value can be either
							 GPIO_PIN_RESET (low) or GPIO_PIN_SET (high or high impedance)
*/
void gpio_output_init(GPIOOutput *gpio, MCU *mcu, GPIO_TypeDef *port,
		uint16_t pin, uint32_t mode, uint32_t pull, GPIO_PinState state) {
	// Create GPIO struct
	gpio->mcu = mcu;
	gpio->port = port;
	gpio->pin = pin;

	// Set initial state to be safe
	// (e.g. don't assert a reset signal initially by accident)
	// Do this before calling HAL_GPIO_Init() to prevent glitches
	gpio_set(gpio, state);

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = pull;
	init.Mode = mode;
	init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &init);
}

/*
Set an output pin to a certain value
@param GPIOOutput *gpio - the pin's GPIOOutput struct
@param GPIO_PinState state - the pin's GPIO_PinState; value can be either
							 GPIO_PIN_RESET (low) or GPIO_PIN_SET (high or high impedance)
*/
void gpio_set(GPIOOutput *gpio, GPIO_PinState state) {
	HAL_GPIO_WritePin(gpio->port, gpio->pin, state);
}

/*
Set an output pin to low
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_set_low(GPIOOutput *gpio) {
	gpio_set(gpio, GPIO_PIN_RESET);
}

/*
Set an output pin to high or high impedance
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_set_high(GPIOOutput *gpio) {
	gpio_set(gpio, GPIO_PIN_SET);
}

/*
Toggle the value of an output gpio pin
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_toggle(GPIOOutput *gpio) {
	HAL_GPIO_TogglePin(gpio->port, gpio->pin);
}
