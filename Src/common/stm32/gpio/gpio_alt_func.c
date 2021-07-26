/*
 * gpio_alt_func.c
 *
 *  Created on: Jul. 23, 2021
 *      Author: bruno
 */

#include <common/stm32/gpio/gpio_alt_func.h>

/*
Initialize a GPIO pin to alternate function state
@param GPIOAltFunc *gpio - a struct of the initialized alternate function pin
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint8_t alternate - the alternate function value
@param uint32_t mode - output mode; must be GPIO_MODE_AF_PP (push-pull) or
                       GPIO_MODE_AF_OD (open-drain)
@param uint32_t pull - the internal pull-up/down gpio resistor state;
                       possible values are GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
@param uint32_t speed - the GPIO speed setting (e.g. GPIO_SPEED_FREQ_LOW);
                        see gpio.h for how to choose this with max frequency
*/
void gpio_alt_func_init(GPIOAltFunc *gpio, GPIO_TypeDef *port, uint16_t pin,
		uint8_t alternate, uint32_t mode, uint32_t pull, uint32_t speed) {
	// Create GPIO struct
	gpio->port = port;
	gpio->pin = pin;

	// Initialize pin with HAL library
	GPIO_InitTypeDef init;
	init.Pin = pin;
	init.Pull = pull;
	init.Mode = mode;
	init.Speed = speed;
	init.Alternate = alternate;
	HAL_GPIO_Init(port, &init);
}
