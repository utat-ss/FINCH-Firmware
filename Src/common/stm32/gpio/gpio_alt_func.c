/*
 * gpio_alt_func.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_alt_func.h>

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
