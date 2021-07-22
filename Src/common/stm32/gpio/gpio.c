/*
 * gpio.c
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 *
 *  Overview:
 *  Wrapper code for the GPIO pins on the STM32 MCU
 *
 *  By default, the output pins speed is set to GPIO_SPEED_FREQ_LOW (reduces
 *  noise and power consumption); The max frequency is 8MHz
 *
 *  Standard GPIO outputs are always set to GPIO_SPEED_FREQ_LOW because they are
 *  not toggled at high frequencies
 *  GPIO speed is not applicable for inputs
 *  Alternate function GPIOs must have their speed specified by the application
 *  code because they may be toggled at higher frequencies than what
 *  GPIO_SPEED_FREQ_LOW supports
 *
 *  For example, can see for the H743 MCU in its datasheet (DS12110 Rev 8),
 *  Section 7.3.15 I/O port characteristics, Table 160 Output timing
 *  characteristics (HSLV OFF), the maximum frequency for each GPIO speed under
 *  various conditions
 *  Assuming VDD=3.3V for the H743 MCU, maximum frequencies are (worst case):
 *  - low speed (0b00) - 12MHz
 *  - medium speed (0b01) - 60MHz
 *  - high speed (0b10) - 85MHz
 *  - very high speed (0b11) - 100MHz
 *
 *  Alternate function values can be seen in the following datasheets for the following MCUs:
 *  STM32H7 - Table 10 pg 89: https://www.mouser.ca/datasheet/2/389/dm00387108-1799185.pdf
 *  STM32G4 - Table 13 pg 74: https://www.st.com/resource/en/datasheet/stm32g474ve.pdf
 */

#include "gpio.h"

/*
Initialize a GPIO pin to output push-pull state
Push-pull pin output values are high or low.
@param GPIOOutput *output - a struct of the initialized pin to be used in
						    other gpio functions
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param GPIO_PinState state - default state to set the pin to; value can be either
							 GPIO_PIN_RESET or GPIO_PIN_SET
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
							 GPIO_PIN_RESET or GPIO_PIN_SET
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
Initialize a GPIO pin to alternate function push-pull state
@param GPIOAltFunc *alt_func - a struct of the initialized alternate pin
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint8_t alternate - the alternate function value
@param uint32_t pull - the internal pull-up/down gpio resistor state;
					   possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@param uint32_t speed - the GPIO speed setting (e.g. GPIO_SPEED_FREQ_LOW)
*/
void gpio_init_alt_func_pp(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed) {
	// Create GPIO struct
	alt_func->port = port;
	alt_func->pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = pull;
	init.Mode = GPIO_MODE_AF_PP;
	init.Speed = speed;
	init.Alternate = alternate;
	HAL_GPIO_Init(port, &init);
}

/*
Initialize a GPIO pin to alternate function open drain state
@param GPIOAltFunc *alt_func - a struct of the initialized alternate pin
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint8_t alternate - the alternate function value
@param uint32_t pull - the internal pull-up/down gpio resistor state;
					   possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@param uint32_t speed - the GPIO speed setting (e.g. GPIO_SPEED_FREQ_LOW)
*/
void gpio_init_alt_func_od(GPIOAltFunc *alt_func, GPIO_TypeDef *port,
		uint16_t pin, uint8_t alternate, uint32_t pull, uint32_t speed) {
	// Create GPIO struct
	alt_func->port = port;
	alt_func->pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = pull;
	init.Mode = GPIO_MODE_AF_OD;
	init.Speed = speed;
	init.Alternate = alternate;
	HAL_GPIO_Init(port, &init);
}

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
Set an output pin to high or high impedance
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_set_high(GPIOOutput *gpio) {
	gpio_set(gpio, GPIO_PIN_SET);
}

/*
Set an output pin to low
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_set_low(GPIOOutput *gpio) {
	gpio_set(gpio, GPIO_PIN_RESET);
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
Toggle the value of an output gpio pin
@param GPIOOutput *gpio - the pin's GPIOOutput struct
*/
void gpio_toggle(GPIOOutput *gpio) {
	HAL_GPIO_TogglePin(gpio->port, gpio->pin);
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
