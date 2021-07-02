#include <drivers/STM32_uart/log.h>
#include <string.h>

#ifdef STM32H743xx
#include <drivers/STM32H7_clock/clock.h>
// Declare pin variables for STM32H7 devkit
#define UART_INST 		USART3
#define UART_ALT  		GPIO_AF7_USART3
#define UART_TX_PORT    GPIOD
#define UART_TX_PIN     GPIO_PIN_8
#define UART_RX_PORT    GPIOD
#define UART_RX_PIN     GPIO_PIN_9
#endif

#ifdef STM32G474xx
#include <drivers/STM32G4_clock/clock.h>
// Declare pin variables for STM32G4 devkit
#define UART_INST       LPUART1
#define UART_ALT        GPIO_AF8_LPUART1	// Might be GPIO_AF12_LPUART1 instead
#define UART_TX_PORT    GPIOA
#define UART_TX_PIN     GPIO_PIN_2
#define UART_RX_PORT    GPIOA
#define UART_RX_PIN     GPIO_PIN_3
#endif




int main() {
    // Initialization
    HAL_Init();
    clock_init();

    UART uart;
    uart_init(&uart, UART_INST, 115200, UART_ALT,
            UART_TX_PORT, UART_TX_PIN, UART_RX_PORT, UART_RX_PIN);
    Log logger;
    log_init(&logger, &uart);
    info(&logger, "Starting UART RX test");


    info(&logger, "testing dma 1");
    info(&logger, "testing dma 2");
    info(&logger, "testing dma 3");
    info(&logger, "testing dma 4");
    info(&logger, "testing dma 5");

    while (1) {
        info(&logger, "Enter uint:");
        uint32_t uint = uart_read_uint(&uart);
        info(&logger, "Read %lu", uint);

        info(&logger, "Enter int:");
        int32_t sint = uart_read_int(&uart);
        info(&logger, "Read %ld", sint);

        info(&logger, "Enter char:");
        char c = uart_read_char(&uart);
        info(&logger, "Read %c", c);
    }

    return 0;
}
