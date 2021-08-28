/*
 * gpio_test.c
 *
 *  Created on: Jul. 18, 2020
 *      Author: brytni
 *
 *  GPIO testing functions
 *  Test will manually blink light on NUCLEO-H74/G474 devkit. When blue button is pressed,
 *  the light (red for H7, green for G4) stop blinking
 */

#include <drivers/STM32_gpio/gpio.h>

#ifdef STM32H743xx
#include <drivers/STM32H7_clock/clock.h>
// Declare pin variables for STM32H7 devkit
#define BLUE_BUTTON_PIN GPIO_PIN_13
#define BLUE_BUTTON_PORT GPIOC
#define LED_PIN GPIO_PIN_14
#define LED_PORT GPIOB
#endif

#ifdef STM32G474xx
#include <drivers/STM32G4_clock/clock.h>
// Declare pin variables for STM32G4 devkit
#define BLUE_BUTTON_PIN GPIO_PIN_13
#define BLUE_BUTTON_PORT GPIOC
#define LED_PIN GPIO_PIN_5
#define LED_PORT GPIOA
#endif

int main() {
	// Initialization
	HAL_Init();
	clock_init();

	// Create GPIO instances
	GPIO_INPUT blue_button = gpio_init_input(BLUE_BUTTON_PORT, BLUE_BUTTON_PIN);
	GPIO_OUTPUT led_light = gpio_init_output(LED_PORT, LED_PIN);

	// Blink light
	while (1) {
		// If button not pressed, blink light
		if (gpio_read(blue_button) == GPIO_PIN_RESET) {
			gpio_set_high(led_light);
			HAL_Delay(100);
		}
		gpio_set_low(led_light);
		HAL_Delay(100);
	}
	return 1;
}
