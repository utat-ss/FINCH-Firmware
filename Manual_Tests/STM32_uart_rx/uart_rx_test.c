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

    Log log;
    log_init(&log, &uart);
    info(&log, "Starting UART RX test");

    // Polling
    while (1) {
        uint8_t rx_buf[1];
        HAL_StatusTypeDef result = HAL_UART_Receive(&uart.handle, rx_buf, 1, 1);

//        info(&log, "loop %u", result);
        if (result == HAL_OK) {
            info(&log, "char %c (%u)", rx_buf[0], rx_buf[0]);
            info(&log, "char %c", rx_buf[0]);
            info(&log, "int %u", rx_buf[0]);
        }
//        HAL_Delay(1000);
    }

    return 0;
}
