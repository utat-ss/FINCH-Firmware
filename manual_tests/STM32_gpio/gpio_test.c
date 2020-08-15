/*
 * gpio_test.c
 *
 *  Created on: Jul. 18, 2020
 *      Author: brytni
 *
 *  GPIO testing functions
 *  Test will manually blink light on NUCLEO-H743ZI2 devkit. When blue button is pressed,
 *  the yellow light will blink. Otherwise, the red light will blink.
 */

#include <STM32_gpio/gpio.h>

#if STM32H743xx
	#include <STM32H7_clock/clock.h>
#endif

int main() {
	// Initialization
	HAL_Init();
	clock_init();

	// Create GPIO instances
	GPIO_OUTPUT red_led = gpio_init_output(GPIOB, GPIO_PIN_14);
	GPIO_OUTPUT yellow_led = gpio_init_output(GPIOE, GPIO_PIN_1);
	GPIO_INPUT blue_button = gpio_init_input(GPIOC, GPIO_PIN_13);

	// Blink light
	while (1) {
		if (gpio_read(blue_button) == GPIO_PIN_SET) {
			gpio_set_high(yellow_led);
		} else {
			gpio_set_high(red_led);
		}
		HAL_Delay(100);
		gpio_set_low(red_led);
		gpio_set_low(yellow_led);
		HAL_Delay(100);
	}

	return 1;
}
