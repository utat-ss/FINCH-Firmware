/*
 * h743zi2.c
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#include <nucleo_h743zi2/h743zi2.h>
#include <nucleo_h743zi2/h743zi2_config.h>

void h743zi2_init(H743ZI2 *dev_kit) {
	mcu_init(&dev_kit->mcu, H743ZI2_BOARD);

	uart_init_for_board(&dev_kit->uart, &dev_kit->mcu);
	log_init(&dev_kit->log, &dev_kit->uart);

	gpio_input_init(&dev_kit->blue_button, &dev_kit->mcu, H743ZI2_BLUE_BUTTON_PORT,
			H743ZI2_BLUE_BUTTON_PIN, GPIO_NOPULL);
	gpio_output_init(&dev_kit->red_led, &dev_kit->mcu, H743ZI2_RED_LED_PORT,
			H743ZI2_RED_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_PIN_RESET);

	info(&dev_kit->log, "Initialized Nucleo H743ZI2 dev kit");
}
