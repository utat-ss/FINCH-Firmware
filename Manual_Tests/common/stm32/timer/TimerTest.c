/* TimerTest.c

Date created: 2021-11-19
Last modified: 2022-05-29
Author: Cameron Rodrgiuez

Manual test for timer wrapper code. Uses callback to light up LED on timeout.
*/

#include <common/stm32/gpio/GPIO.h>
#include <common/stm32/timer/Timer.h>
#include <common/stm32/uart/Log.h>
#include <nucleo_g431rb/G431RBConfig.h>
#include <nucleo_g474re/G474REConfig.h>
#include <nucleo_h743zi2/H743ZI2Config.h>

// Used to toggle LED
GPIOOutput g_test_led;
Log g_log;
Timer g_timer;

// Passes interrupts from TIM5 to interrupt handler
void TIM5_IRQHandler() {
    HAL_TIM_IRQHandler(&(g_timer.handle));
}

// Timer callback function to toggle LED after timer runs out
void toggle_led(TIM_HandleTypeDef *htim) {
    debug(&g_log, "Currently in callback");
    if(htim->Instance == TIM5) {
        gpio_toggle(&g_test_led);
    }
}

// Main function for test function
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
    log_init(&g_log, &uart);

    info(&g_log, "Enabling GPIO");
    if(mcu.board == MCU_BOARD_NUCLEO_H743ZI2) {
        gpio_output_init(&g_test_led, &mcu, H743ZI2_GREEN_LED_PORT,
                H743ZI2_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
                GPIO_PIN_RESET);
    } else if (mcu.board == MCU_BOARD_NUCLEO_G474RE) {
        gpio_output_init(&g_test_led, &mcu, G474RE_GREEN_LED_PORT,
                G474RE_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
                GPIO_PIN_RESET);
    } else if (mcu.board == MCU_BOARD_NUCLEO_G431RB) {
        gpio_output_init(&g_test_led, &mcu, G431RB_GREEN_LED_PORT,
                G431RB_GREEN_LED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL,
                GPIO_PIN_RESET);
    } else {
        error(&g_log, "Board not supported, ending test function");
        return -1;
    }

    // APB1 clock runs at 240 MHz for TIM5, scaling to 4 Hz
    info(&g_log, "Enabling timer");
    timer_setup(&g_timer, 7999, 7499, 1);
    HAL_StatusTypeDef status = timer_init(&g_timer);
    if(status != HAL_OK) {
        error(&g_log, "Encountered HAL status %d while initializing timer", status);
        return -1;
    }
    status = timer_setup_callback(&g_timer, toggle_led);
    if(status != HAL_OK) {
        error(&g_log, "Encountered HAL status %d during timer callback registration", status);
        return -1;
    }

    info(&g_log, "Starting timer");
    status = timer_start(&g_timer);
    if(status != HAL_OK) {
        error(&g_log, "Encountered HAL status %d during timer startup", status);
        return -1;
    }

    while(1){
    }

    return 0;
}
