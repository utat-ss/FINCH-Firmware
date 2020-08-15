/*
 * gpio.c
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 *
 *  Overview:
 *  Wrapper code for the GPIO pins on the STM32 MCU
 *
 *  By default, the output pins speed is set to GPIO_SPEED_FREQ_LOW (reduces noise and
 *  power consumption); The max frequency is 8MHz
 */

#include "gpio.h"

/*
Initialize a GPIO pin to output push-pull state
Push-pull pin output values are high or low.
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@return GPIO_OUTPUT output_pin - a struct of the initialized pin to be used in
								 other gpio functions
*/
GPIO_OUTPUT gpio_init_output(GPIO_TypeDef *port, uint16_t pin) {
	// Create GPIO struct
	GPIO_OUTPUT output_pin;
	output_pin.port = port;
	output_pin.pin = pin;
	output_pin.mode = GPIO_MODE_OUTPUT_PP;

	// Initialize pin with HAL library
	GPIO_InitTypeDef hal_gpio_handler;
	hal_gpio_handler.Pin = pin;
	hal_gpio_handler.Pull = GPIO_NOPULL;
	hal_gpio_handler.Mode = GPIO_MODE_OUTPUT_PP;
	hal_gpio_handler.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &hal_gpio_handler);

	return output_pin;
}

/*
Initialize a GPIO pin to output open drain state
Open drain pin output values are high impedance or low.
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint32_t pull - the internal pull-up/down gpio resistor state;
					   possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@return GPIO_OUTPUT output_pin - a struct of the initialized pin to be used in
								 other gpio functions
*/
GPIO_OUTPUT gpio_init_output_od(GPIO_TypeDef *port, uint16_t pin, uint32_t pull) {
	// Create GPIO struct
	GPIO_OUTPUT output_pin;
	output_pin.port = port;
	output_pin.pin = pin;
	output_pin.mode = GPIO_MODE_OUTPUT_OD;

	// Initialize pin with HAL library
	GPIO_InitTypeDef hal_gpio_handler;
	hal_gpio_handler.Pin = pin;
	hal_gpio_handler.Pull = pull;
	hal_gpio_handler.Mode = GPIO_MODE_OUTPUT_OD;
	hal_gpio_handler.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &hal_gpio_handler);

	return output_pin;
}

/*
Initialize a GPIO pin to input state
Push-pull pin output values are high impedance or low.
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@return GPIO_INPUT input_pin - a struct of the initialized pin to be used in
								other gpio functions
*/
GPIO_INPUT gpio_init_input(GPIO_TypeDef *port, uint16_t pin) {
	// Create GPIO struct
	GPIO_INPUT input_pin;
	input_pin.port = port;
	input_pin.pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef hal_gpio_handler;
	hal_gpio_handler.Pin = pin;
	hal_gpio_handler.Pull = GPIO_NOPULL;
	hal_gpio_handler.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(port, &hal_gpio_handler);

	return input_pin;
}

/*
Set an output pin to high or high impedance
@param GPIO_OUTPUT gpio - the pin's GPIO_OUTPUT struct
*/
void gpio_set_high(GPIO_OUTPUT gpio) {
	HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_SET);
}

/*
Set an output pin to low
@param GPIO_OUTPUT gpio - the pin's GPIO_OUTPUT struct
*/
void gpio_set_low(GPIO_OUTPUT gpio) {
	HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_RESET);
}

/*
Set an output pin to a certain value
@param GPIO_OUTPUT gpio - the pin's GPIO_OUTPUT struct
@param GPIO_PinState state - the pin's GPIO_PinState; value can be either
							 GPIO_PIN_RESET or GPIO_PIN_RESET
*/
void gpio_set_value(GPIO_OUTPUT gpio, GPIO_PinState state) {
	HAL_GPIO_WritePin(gpio.port, gpio.pin, state);
}

/*
Return the input value of a pin
@param GPIO_INPUT gpio - the pin's GPIO_INPUT struct
@param GPIO_PinState state - the pin's GPIO_PinState; value will be either
							 GPIO_PIN_SET or GPIO_PIN_RESET
*/
GPIO_PinState gpio_read(GPIO_INPUT gpio) {
	return HAL_GPIO_ReadPin(gpio.port, gpio.pin);
}
