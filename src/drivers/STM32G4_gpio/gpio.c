/*
 * gpio.c
 *
 *  Created on: Jul. 8, 2020
 *      Author: brytni
 *
 *  Overview:
 *  Wrapper code for the GPIO pins on the STM32G4 MCU
 */

#include "gpio.h"

/*
Some initialization function that will probably be needed
*/
void gpio_init(GPIO_TypeDef *port) {
	// Yet to be determined
}

/*
Set a pin to either high or low
@param GPIO_TypeDef *port - the gpio port the pin is on
@param uint16_t pin - the pin number
@param uint8_t value - the value to set the direction to
*/
void gpio_set_direction(GPIO_TypeDef *port, uint16_t pin, uint8_t value) {
	// State checking such as assert

	// Functionality
	port->MODER ^= (-value ^ port->IDR) & (1 << pin) ;
}

/*
Set a specific pin to either high or low
@param GPIO_TypeDef *port - the port the pin is on
@param uint16_t pin - the pin number
@param uint8_t value - the value to set the pin to
*/
void gpio_set_pin(GPIO_TypeDef *port, uint16_t pin, uint8_t value) {
	port->ODR ^= (-value ^ port->IDR) & (1 << pin) ;
}

/*
Set a specific pin to high
@param GPIO_TypeDef *port - the port the pin is on
@param uint16_t pin - the pin number
*/
void gpio_set_pin_high(GPIO_TypeDef *port, uint16_t pin) {
	port->ODR |= (1 << pin) ;
}

/*
Set a specific pin to low
@param GPIO_TypeDef *port - the port the pin is on
@param uint16_t pin - the pin number
*/
void gpio_set_pin_low(GPIO_TypeDef *port, uint16_t pin) {
	port->ODR &= ~(1 << pin);
}

/*
Toggle a specific pin
@param GPIO_TypeDef *port - the port the pin is on
@param uint16_t pin - the pin number
*/
void gpio_toggle_pin(GPIO_TypeDef *port, uint16_t pin) {
	port->ODR ^= 1 << pin;
}

