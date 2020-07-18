/*
 * gpio_test.c
 *
 *  Created on: Jul. 18, 2020
 *      Author: brytni
 *
 *  GPIO testing functions
 */

#include "gpio_test.h"

// Manually blink light on devkit
void gpio_test_blink() {
	HAL_Init();
	gpio_init(GPIOA);
	gpio_set_direction(GPIOA, PA1, OUTPUT);

	// Blink light
	while (1) {
		gpio_toggle_pin(GPIOA, PA1);
		HAL_Delay(100);
	}
}
