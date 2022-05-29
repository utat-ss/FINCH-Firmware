/*
G431RB.c

Date Created: May 28, 2022
Last Modified: May 29, 2022

Code for the STM32G431RB dev kit.

Author: Eric Leus
*/

#include <nucleo_g431rb/G431RB.h>
#include <nucleo_g431rb/G431RBConfig.h>

void g431re_init(G431RB* dev_kit) {
    mcu_init(&dev_kit->mcu, G431RB_BOARD);

    uart_init_for_board(&dev_kit->uart, &dev_kit->mcu);
    log_init(&dev_kit->log, &dev_kit->uart);

    gpio_input_init(&dev_kit->blue_button, &dev_kit->mcu,
            G431RB_BLUE_BUTTON_PORT, G431RB_BLUE_BUTTON_PIN, GPIO_NOPULL);
    gpio_output_init(&dev_kit->green_led, &dev_kit->mcu, G431RB_GREEN_LED_PORT,
            G431RB_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
            GPIO_PIN_RESET);

    info(&dev_kit->log, "Initialized Nucleo G431RB dev kit");
}