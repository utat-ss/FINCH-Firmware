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

// Interrupt
uint8_t rx_buf[10];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    info(&logger, "irq");
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
    info(&logger, "irq");
}





/*
 * From https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
 * To define an interrupt handler, "you just declare a void function with no arguments and the same
 * name as the corresponding entry in the vector table"
 */

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
    info(&logger, "irq");


  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
//  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}





int main() {
    // Initialization
    HAL_Init();
    clock_init();

    uart_init(&uart, UART_INST, 115200, UART_ALT,
            UART_TX_PORT, UART_TX_PIN, UART_RX_PORT, UART_RX_PIN);




    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);


    // TODO

    // Setup the NVIC to enable interrupts.
    // Use 4 bits for 'priority' and 0 bits for 'subpriority'.
    NVIC_SetPriorityGrouping( 0 );
    // UART receive interrupts should be high priority.
    uint32_t uart_pri_encoding = NVIC_EncodePriority( 0, 1, 0 );
    NVIC_SetPriority( USART2_IRQn, uart_pri_encoding );
    NVIC_EnableIRQ( USART2_IRQn );





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

    // TODO - HAL_UART_ReceiverTimeout_Config?
    // TODO - HAL_UART_ERROR_RTO?
    // TODO - __HAL_UART_GET_FLAG?
    // TODO - __HAL_UART_ENABLE_IT()?
    // TODO - __HAL_UART_GET_IT?
    // TODO - UART_RECEIVER_TIMEOUT_ENABLE?

    // TODO - may need to uncomment define(s) in ...hal_conf.h?
    // TODO - uncomment USE_FULL_ASSERT in ...hal_conf.h
    // TODO - configure NVIC?

    // TODO - UART_RxISR_8BIT_FIFOEN is automatically set to be the RX ISR when in FIFO mode?
    // TODO - UART_RxISR_8BIT when not in FIFO mode?

    // Is this necessary?
    __enable_irq();

    HAL_UART_Receive_IT(&uart.handle, rx_buf, 1);
    info(&logger, "waiting...");
    while (1) {}

    return 0;
}
