/* TimerTest.c

Date created: 2021-11-19
Last modified: 2021-11-19
Author: Cameron Rodrgiuez

Manual test for timer wrapper code. Uses callback to light up LED on timeout.
*/

#include <common/stm32/gpio/GPIO.h>
#include <common/stm32/timer/Timer.h>
#include <common/stm32/uart/Log.h>
#include <nucleo_g474re/G474REConfig.h>
#include <nucleo_h743zi2/H743ZI2Config.h>

// Used to toggle LED
GPIOOutput g_test_led;

// Timer callback function to toggle LED after timer runs out
void toggle_led() {
    gpio_toggle(&g_test_led);
}

// Main function for test function
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
    UART uart;
    uart_init_for_board(&uart, &mcu);
    Log log;
    log_init(&log, &uart);

    info(&log, "Enabling GPIO");
    if(mcu.board == MCU_BOARD_NUCLEO_H743ZI2) {
        gpio_output_init(&g_test_led, &mcu, H743ZI2_GREEN_LED_PORT,
                H743ZI2_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
                GPIO_PIN_RESET);
    } else if (mcu.board == MCU_BOARD_NUCLEO_G474RE) {
        gpio_output_init(&g_test_led, &mcu, G474RE_GREEN_LED_PORT,
                G474RE_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
                GPIO_PIN_RESET);
    } else {
        error(&log, "Board not supported, ending test function");
        return -1;
    }

    info(&log, "Enabling timer");

    return 0;
}