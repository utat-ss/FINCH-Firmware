/*
 * gpio_input.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_input.h>

/*
Initialize a GPIO pin to input state
@param GPIOInput *input - a struct of the initialized pin to be used in
						  other gpio functions
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint32_t pull - the internal pull-up/down gpio resistor state;
					   possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
*/
void gpio_init_input(GPIOInput *input, GPIO_TypeDef *port, uint16_t pin,
		uint32_t pull) {
	// Create GPIO struct
	input->port = port;
	input->pin = pin;

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
