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


    // Polling
//    while (1) {
//        uint8_t rx_buf[1];
//        HAL_StatusTypeDef result = HAL_UART_Receive(&uart.handle, rx_buf, 1, 1);
//
////        info(&log, "loop %u", result);
//        if (result == HAL_OK) {
//            info(&log, "char %c (%u)", rx_buf[0], rx_buf[0]);
//            info(&log, "char %c", rx_buf[0]);
//            info(&log, "int %u", rx_buf[0]);
//        }
////        HAL_Delay(1000);
//    }


    // Interrupt

//    HAL_UART_Receive_IT(&uart.handle, (uint8_t*) uart.rx_buf, 3);
//    info(&logger, "waiting...");
//    while (1) {}


    // DMA

    // HAL_UART_DMAStop
    // HAL_UART_ErrorCallback??

    // The DMA has a CNDTR register (be aware that it counts down)
    // uint32_t bytesRx DMA1_Stream1->NDTR;
    // UART_HandleTypeDef huart3;
    // uint32_t bytesRx = huart3.hdmarx->Instance->NDTR;



//    HAL_UART_Transmit(&uart.handle, "test\r\n", 6, 0xFFFF);
//    HAL_Delay(1000);

    uart_write_dma(&uart, "testing dma 1\r\n", 15);
//    HAL_Delay(1000);
    uart_write_dma(&uart, "testing dma 2\r\n", 15);
//    HAL_Delay(1000);
    uart_write_dma(&uart, "testing dma 3\r\n", 15);
//    HAL_Delay(1000);
    uart_write_dma(&uart, "testing dma 4\r\n", 15);
//    HAL_Delay(1000);
    uart_write_dma(&uart, "testing dma 5\r\n", 15);
    HAL_Delay(1000);

    uint8_t rx_buf[10] = {0};
    HAL_UART_Receive_DMA(&uart.handle, rx_buf, 4);

    while (1)
    {

    }




    return 0;
}
