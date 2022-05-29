/*
 * UARTRXTest.c
 *
 *  Created on: Sept. 28, 2021
 *      Author: bruno
 *  Last modified: May 29, 2022
 *
 * Tests reading various types of input data from UART (sent from a laptop).
 */

#include <common/stm32/uart/Log.h>

int main() {
    // Try to automatically detect board based on MCU UID
    MCUBoard board = mcu_get_board();
    // If the UID is not matched, try to guess the board based on the device ID
    if (board == MCU_BOARD_NONE) {
        MCUDevID dev_id = mcu_get_dev_id();
        if (dev_id == MCU_DEV_ID_STM32G431_441) {
            board = MCU_BOARD_NUCLEO_G431RB;
        } else if (dev_id == MCU_DEV_ID_STM32G471_473_474_483_484) {
            board = MCU_BOARD_NUCLEO_G474RE;
        } else if (dev_id == MCU_DEV_ID_STM32H742_743_753_750) {
            board = MCU_BOARD_NUCLEO_H743ZI2;
        }
    }

    MCU mcu;
    mcu_init(&mcu, board);
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);


    info(&log, "Starting UART RX test");
    info(&log, "testing dma 1");
    info(&log, "testing dma 2");
    info(&log, "testing dma 3");
    info(&log, "testing dma 4");
    info(&log, "testing dma 5");
    info(&log, "float %f", 7.963);

    while (1) {
        uint32_t uint = uart_read_uint(&uart, "Enter uint:");
        info(&log, "Read %lu", uint);

        int32_t sint = uart_read_int(&uart, "Enter %s:", "int");
        info(&log, "Read %ld", sint);

        double doub = uart_read_double(&uart, "Enter double:");
        info(&log, "Read %lf", doub);

        char c = uart_read_char(&uart, "Enter char:");
        info(&log, "Read %c", c);

        uart_wait_for_key_press(&uart);
    }

    return 0;
}
