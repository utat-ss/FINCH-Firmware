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

UART uart;
Log logger;


int main() {
    // Initialization
    HAL_Init();
    clock_init();

    uart_init(&uart, UART_INST, 115200, UART_ALT,
            UART_TX_PORT, UART_TX_PIN, UART_RX_PORT, UART_RX_PIN);
    log_init(&logger, &uart);
    info(&logger, "Starting UART RX test");


    // HAL_UART_DMAStop

    info(&logger, "testing dma 1");
    info(&logger, "testing dma 2");
    info(&logger, "testing dma 3");
    info(&logger, "testing dma 4");
    info(&logger, "testing dma 5");
    HAL_Delay(1000);

    uart_read(&uart);
    while (1) {
        info(&logger, "%u bytes", uart_get_rx_count(&uart));
        HAL_Delay(1000);
    }

    return 0;
}
