/*
 * gpio_output.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_output.h>

/*
Initialize a GPIO pin to output push-pull state
Push-pull pin output values are high or low.
@param GPIOOutput *output - a struct of the initialized pin to be used in
						    other gpio functions
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param GPIO_PinState state - default state to set the pin to; value can be either
							 GPIO_PIN_RESET (low) or GPIO_PIN_SET (high)
*/
void gpio_init_output_pp(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin,
		GPIO_PinState state) {
	// Create GPIO struct
	output->port = port;
	output->pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = GPIO_NOPULL;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &init);

	// Set default state on initialization to be safe
	// (e.g. don't assert a reset signal by default by accident)
	gpio_set(output, state);
}

/*
Initialize a GPIO pin to output open drain state
Open drain pin output values are high impedance or low.
@param GPIOOutput *output - a struct of the initialized pin to be used in
						    other gpio functions
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint32_t pull - the internal pull-up/down gpio resistor state;
					   possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@param GPIO_PinState state - default state to set the pin to; value can be either
							 GPIO_PIN_RESET (low) or GPIO_PIN_SET (high impedance)
*/
void gpio_init_output_od(GPIOOutput *output, GPIO_TypeDef *port, uint16_t pin,
		uint32_t pull, GPIO_PinState state) {
	// Create GPIO struct
	output->port = port;
	output->pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = pull;
	init.Mode = GPIO_MODE_OUTPUT_OD;
	init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &init);

	// Set default state on initialization to be safe
	// (e.g. don't assert a reset signal by default by accident)
	gpio_set(output, state);
}

/*
Set an output pin to a certain value
@param GPIOOutput *gpio - the pin's GPIOOutput struct
@param GPIO_PinState state - the pin's GPIO_PinState; value can be either
							 GPIO_PIN_RESET or GPIO_PIN_SET
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
