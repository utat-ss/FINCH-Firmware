/*
 * gpio_test.c
 *
 *  Created on: Jul. 18, 2020
 *      Author: brytni
 *
 *  GPIO testing functions
 *  Test will manually blink light on NUCLEO-H74/G474 devkit. When blue button is pressed,
 *  the light (red for H7, green for G4) stops blinking
 */

#include <common/stm32/gpio/gpio.h>
#include <common/stm32/mcu/init.h>
#include <nucleo_g474re/g474re_config.h>
#include <nucleo_h743zi2/h743zi2_config.h>

int main() {
	// Try to automatically detect board based on MCU UID
	MCUBoard board = mcu_get_board();
	// If the UID is not matched, try to guess the board based on the device ID
	if (board == MCU_BOARD_NONE) {
		MCUDevID dev_id = mcu_get_dev_id();
		if (dev_id == MCU_DEV_ID_STM32G471_473_474_483_484) {
			board = MCU_BOARD_NUCLEO_G474RE;
		}
		else if (dev_id == MCU_DEV_ID_STM32H742_743_753_750) {
			board = MCU_BOARD_NUCLEO_H743ZI2;
		}
	}

	MCU mcu;
	mcu_init(&mcu, board);

	// Create GPIO instances
	GPIOInput blue_button;
	GPIOOutput led;
	if (board == MCU_BOARD_NUCLEO_G474RE) {
		gpio_input_init(&blue_button, &mcu, G474RE_BLUE_BUTTON_PORT,
				G474RE_BLUE_BUTTON_PIN, GPIO_NOPULL);
		gpio_output_init(&led, &mcu, G474RE_GREEN_LED_PORT,
				G474RE_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_PIN_RESET);
	}
	else if (board == MCU_BOARD_NUCLEO_H743ZI2) {
		gpio_input_init(&blue_button, &mcu, H743ZI2_BLUE_BUTTON_PORT,
				H743ZI2_BLUE_BUTTON_PIN, GPIO_NOPULL);
		gpio_output_init(&led, &mcu, H743ZI2_RED_LED_PORT,
				H743ZI2_RED_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_PIN_RESET);
	}


	// Uncomment to test waiting for an input state
//	gpio_wait_for_high(&blue_button, 10000);
//	gpio_set_high(&led_light);
//	HAL_Delay(500);
//	gpio_set_low(&led_light);
//	HAL_Delay(500);

	// Blink light
	while (1) {
		// If button not pressed, blink light
		if (gpio_read(&blue_button) == GPIO_PIN_RESET) {
			gpio_set_high(&led);
			HAL_Delay(100);
		}
		gpio_set_low(&led);
		HAL_Delay(100);
	}
	return 0;
}
