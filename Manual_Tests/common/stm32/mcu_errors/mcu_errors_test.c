#include <common/stm32/mcu/init.h>
#include <common/stm32/uart/log.h>

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
    HAL_Init();
    clock_init();

    UART uart;
    uart_init(&uart, UART_INST, UART_BAUD_115200, UART_ALT,
            UART_TX_PORT, UART_TX_PIN, UART_RX_PORT, UART_RX_PIN);

    Log log;
    log_init(&log, &uart);

    info(&log, "Starting MCU errors test");

    Error_Handler();
    assert_param(1);
    assert_param(0);
    // Must cast __FILE__ or else it produces the warning "pointer targets in
    // passing argument 1 of 'assert_failed' differ in signedness [-Wpointer-sign]"
    assert_failed((uint8_t*) __FILE__, __LINE__);

	info(&log, "Done MCU errors test");
	while (1) {}

    return 0;
}
