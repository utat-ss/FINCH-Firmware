/*
 * G474RE.c
 *
 *  Created on: Jul. 25, 2021
 *      Author: bruno
 */

#include <nucleo_g474re/G474RE.h>
#include <nucleo_g474re/G474REConfig.h>

void g474re_init(G474RE* dev_kit) {
    mcu_init(&dev_kit->mcu, G474RE_BOARD);

    uart_init_for_board(&dev_kit->uart, &dev_kit->mcu);
    log_init(&dev_kit->log, &dev_kit->uart);

    gpio_input_init(&dev_kit->blue_button, &dev_kit->mcu,
            G474RE_BLUE_BUTTON_PORT, G474RE_BLUE_BUTTON_PIN, GPIO_NOPULL);
    gpio_output_init(&dev_kit->green_led, &dev_kit->mcu, G474RE_GREEN_LED_PORT,
            G474RE_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
            GPIO_PIN_RESET);

    info(&dev_kit->log, "Initialized Nucleo G474RE dev kit");
}
