#include <common/stm32/mcu/mcu.h>
#include <common/stm32/uart/log.h>
#include <string.h>

#ifdef STM32H743xx
// Declare pin variables for STM32H7 devkit
#define UART_INST 		USART3
#define UART_ALT  		GPIO_AF7_USART3
#define UART_TX_PORT    GPIOD
#define UART_TX_PIN     GPIO_PIN_8
#define UART_RX_PORT    GPIOD
#define UART_RX_PIN     GPIO_PIN_9
#endif

#ifdef STM32G474xx
// Declare pin variables for STM32G4 devkit
#define UART_INST       LPUART1
#define UART_ALT        GPIO_AF8_LPUART1	// Might be GPIO_AF12_LPUART1 instead
#define UART_TX_PORT    GPIOA
#define UART_TX_PIN     GPIO_PIN_2
#define UART_RX_PORT    GPIOA
#define UART_RX_PIN     GPIO_PIN_3
#endif

int main() {
    MCU mcu;
    mcu_init(&mcu);

    UART uart;
    uart_init(&uart, UART_INST, UART_BAUD_115200, UART_ALT,
            UART_TX_PORT, UART_TX_PIN, UART_RX_PORT, UART_RX_PIN);

    uint8_t buf1[20] = "testing\r\n";
    // Need to cast `buf1` or else we get an error about mismatched signedness
    uart_write(&uart, buf1, strlen((const char *) buf1));

    Log log;
    log_init(&log, &uart);

    error(&log, "error %d", 1);
    warning(&log, "warning %d", 2);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    HAL_Delay(100);
    error(&log, "error %f", 5.12);

    log_set_level(&log, LOG_LEVEL_VERBOSE);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    log_set_level(&log, LOG_LEVEL_INFO);

    log_set_global_level(LOG_LEVEL_VERBOSE);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    log_set_global_level(LOG_LEVEL_WARNING);
    info(&log, "info %d", 3);
    debug(&log, "debug %d", 4);
    verbose(&log, "verbose %d", 5);

    while (1) {
        info(&log, "done");
        HAL_Delay(5000);
    }

    return 0;
}
