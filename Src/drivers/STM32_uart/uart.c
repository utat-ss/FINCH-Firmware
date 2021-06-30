/*
 * uart.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#include <drivers/STM32_uart/uart.h>

#include <drivers/STM32_uart/log.h>

// TODO - add others
UART *g_uart_usart3 = NULL;

// alt - e.g. GPIO_AF7_USART3 for an instance of USART3
void uart_init_base(UART* uart,
        USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

    // TODO - move to HAL_MspInit()??
	__HAL_RCC_SYSCFG_CLK_ENABLE();




	// TODO - common header file / Confluence page with DMA/stream allocations/constants?

	  /* DMA controller clock enable */
	  __HAL_RCC_DMA1_CLK_ENABLE();

	  /* DMA interrupt init */
	  /* DMA1_Stream0_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	  /* DMA1_Stream1_IRQn interrupt configuration */
	  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);



	  DMA_HandleTypeDef hdma_usart3_rx;
	  DMA_HandleTypeDef hdma_usart3_tx;



	uart->handle.Instance = instance;
	uart->handle.Init.BaudRate = baud_rate;
	uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart->handle.Init.StopBits = UART_STOPBITS_1;
	uart->handle.Init.Parity = UART_PARITY_NONE;
	uart->handle.Init.Mode = UART_MODE_TX_RX;
	uart->handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart->handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart->handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	uart->handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	// TODO - should these TX/RX FIFO thresholds be changed?
	// TODO - look into FIFO modes
	if (HAL_UARTEx_SetTxFifoThreshold(&uart->handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&uart->handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&uart->handle) != HAL_OK) {
		Error_Handler();
	}
	// TODO - test enabling FIFO mode
	//  if (HAL_UARTEx_EnableFifoMode(&huart3) != HAL_OK)
	//  {
	//    Error_Handler();
	//  }

	// TODO - move to after usart clk enable?
	//tx pin init
	uart->tx = gpio_init_alt(tx_port, tx_pin, alt);
	//rx pin init
	uart->rx = gpio_init_alt(rx_port, rx_pin, alt);

	// Peripheral clock configuration
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
	if (uart->handle.Instance==USART1 || uart->handle.Instance==USART6) {
	    // UART peripheral 1 or 6
	    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART16;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	} else {
	    // UART peripheral 2, 3, 4, 5, 7, or 8
	    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	}
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

	// Peripheral clock enable for the appropriate UxART peripheral
	// TODO finish others
	if(uart->handle.Instance==USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();
	}
	if(uart->handle.Instance==USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
	}
	if(uart->handle.Instance==USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
		g_uart_usart3 = uart;
	}
	if(uart->handle.Instance==UART4) {
		__HAL_RCC_UART4_CLK_ENABLE();
	}
	if(uart->handle.Instance==UART5) {
		__HAL_RCC_UART5_CLK_ENABLE();
	}
	if(uart->handle.Instance==USART6) {
		__HAL_RCC_USART6_CLK_ENABLE();
	}
	if(uart->handle.Instance==UART7) {
		__HAL_RCC_UART7_CLK_ENABLE();
	}
	if(uart->handle.Instance==UART8) {
		__HAL_RCC_UART8_CLK_ENABLE();
	}





	/* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Stream0;
    hdma_usart3_rx.Init.Request = DMA_REQUEST_USART3_RX;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    // Bruno TODO high?
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    //
    // Bruno disabled fifo mode
//    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    //
    hdma_usart3_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hdma_usart3_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_usart3_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);






    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Stream1;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_USART3_TX;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart3_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    hdma_usart3_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_usart3_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);








	/* USART3 interrupt Init */
    // TODO - should preempty priority be 1 instead of 0?
    // Configure the NVIC and enable the RX interrupt
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    // Don't need to call __enable_irq() - interrupts are already enabled

    // TODO - could try implementing receiver timeout for RX in the future
}

// Initialize normal UART
void uart_init(UART* uart,
        USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

	uart_init_base(uart, instance, baud_rate, alt, tx_port, tx_pin, rx_port, rx_pin);

	if (HAL_UART_Init(&uart->handle) != HAL_OK) {
		Error_Handler();
	}

	uint8_t buf[] = "Initialized UART\r\n";
	uart_write(uart, buf, sizeof(buf));

	if (!g_log_def_initialized) {
	    log_init(&g_log_def, uart);
        info(&g_log_def, "Initialized default log");
        g_log_def_initialized = true;
	}

	// TODO - change to 9600 baud, print warning message, change back
}

// Initialize UART + RS-485
void uart_init_with_rs485(UART* uart,
        USART_TypeDef *instance, uint32_t baud_rate, uint8_t alt,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin,
		GPIO_TypeDef *de_port, uint16_t de_pin) {

	uart_init_base(uart, instance, baud_rate, alt, tx_port, tx_pin, rx_port, rx_pin);

	// RS-485 DE pin init
	uart->de = gpio_init_alt(de_port, de_pin, alt);

	if (HAL_RS485Ex_Init(&uart->handle, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK) {
		Error_Handler();
	}

	if (g_log_def_initialized) {
        info(&g_log_def, "Initialized UART + RS-485");
    }
}

// Writes data in blocking mode
void uart_write(UART *uart, uint8_t *buf, uint32_t count) {
	// Just use the max possible timeout
	HAL_UART_Transmit(&uart->handle, buf, count, 0xFFFF);
}

// Writes data in DMA mode
void uart_write_dma(UART *uart, uint8_t *buf, uint32_t count) {
	// TODO - might need to check if there is a DMA write already going on, and
	// wait if necessary?
	HAL_UART_Transmit_DMA(&uart->handle, buf, count);
}




/*
 * This function is called in the UART IRQ handler when all possible bytes have
 * been received, i.e. number of bytes received is equal to the `Size` parameter
 * passed to HAL_UART_Receive_IT().
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (!g_log_def_initialized) {
        return;
    }
    warning(&g_log_def, "UART RX buffer is full");
}

/*
 * From https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
 * To define an interrupt handler, "you just declare a void function with no arguments and the same
 * name as the corresponding entry in the vector table".
 * The vector table with the available peripheral interrupt handler names is in
 * the startup file (of assembly code), e.g. startup_stm32h743zitx.s.
 */

/**
  * @brief This function handles USART3 global interrupt.
  */
// TODO - add others
void USART3_IRQHandler(void)
{
    if (g_uart_usart3 == NULL) {
        return;
    }

    // Need to call this here so that it calls the default RX ISR function,
    // which saves the received byte into the RX buffer
    HAL_UART_IRQHandler(&g_uart_usart3->handle);
}





/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}




