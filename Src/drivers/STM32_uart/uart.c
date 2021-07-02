/*
 * uart.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */

#include <drivers/STM32_uart/uart.h>

#include <drivers/STM32_uart/log.h>

// TODO - add others
// TODO - add default
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




    /* TX DMA Init */
    uart->tx_dma_handle.Instance = DMA1_Stream0;
    uart->tx_dma_handle.Init.Request = DMA_REQUEST_USART3_TX;
    uart->tx_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    uart->tx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    uart->tx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    uart->tx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    uart->tx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    uart->tx_dma_handle.Init.Mode = DMA_NORMAL;
    uart->tx_dma_handle.Init.Priority = DMA_PRIORITY_LOW;
    uart->tx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    uart->tx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    uart->tx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    uart->tx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&uart->tx_dma_handle) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&uart->handle,hdmatx,uart->tx_dma_handle);




    /* RX DMA Init */
    uart->rx_dma_handle.Instance = DMA1_Stream1;
    uart->rx_dma_handle.Init.Request = DMA_REQUEST_USART3_RX;
    uart->rx_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    uart->rx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    uart->rx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    uart->rx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    uart->rx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    uart->rx_dma_handle.Init.Mode = DMA_NORMAL;
    // Bruno TODO high?
    uart->rx_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    //
    // Bruno disabled fifo mode
//    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    uart->rx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    //
    uart->rx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    uart->rx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    uart->rx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&uart->rx_dma_handle) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(&uart->handle,hdmarx,uart->rx_dma_handle);




	/* USART3 interrupt Init */
    // Configure the NVIC and enable the interrupt
    // This is necessary for TX (even though it's in DMA mode), enabling
    // interrupts when a transmission is done so it
    // can call the IRQ handler, which sets the UART handle back to a ready
    // state to be able to do the next DMA transmission
    // TODO - should preempt priority be 1 instead of 0?
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    // Don't need to call __enable_irq() - global interrupts are already enabled
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

	// Start receiving data through RX DMA
	uart_restart_rx_dma(uart);

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
    // If a TX process is already ongoing, wait for it to finish
    // If a previous DMA TX is in progress, when the TX is done the UART ISR
    // will set gState to ready
    // This is necessary because if you call HAL_UART_Transmit_DMA() when
    // gState is not ready, it fails and returns busy (without printing anything)
    uint32_t start = HAL_GetTick();
    while (uart->handle.gState != HAL_UART_STATE_READY) {
        // 100ms timeout (this should never happen)
        if (HAL_GetTick() > start + 100) {
            // TODO - hard fault or something?
            return;
        }
    }

	HAL_UART_Transmit_DMA(&uart->handle, buf, count);
}

/*
 * This function works for starting RX for the first time or for restarting it
 * after the first time.
 */
void uart_restart_rx_dma(UART *uart) {
    // Do NOT temporary disable interrupts in the function
    // The abort complete interrupt callback must be executed to set the RX
    // state to ready and start the next RX transfer (described below)

    // Get number of bytes already transferred
    // Do this before aborting the RX transfer just in case NDTR is changed
    uint32_t prev_count = uart_get_rx_count(uart);

    // Abort the ongoing UART RX DMA transfer
    HAL_UART_AbortReceive_IT(&uart->handle);
    // Shortly after the abort function is called (or maybe during the function),
    // the abort complete callback (UART_DMARxOnlyAbortCallback()) is called,
    // which sets RxState to ready
    // HAL_UART_Receive_DMA() requires the RxState to be ready or else it fails
    // to start a new RX DMA transfer

    // Wait until RxState changes to ready (set in the abort callback)
    uint32_t start = HAL_GetTick();
    while (uart->handle.RxState != HAL_UART_STATE_READY) {
        // 100ms timeout (this should never happen)
        if (HAL_GetTick() > start + 100) {
            // TODO - hard fault or something?
            return;
        }
    }

    // Set bytes in RX buffer to zero, but only the bytes that were filled by
    // the previous RX transfer
    // Technically this is not required, but do it to make it easier to view the
    // contents of the buffer in the debugger (only see the bytes filled by the
    // current RX transfer)
    memset(uart->rx_buf, 0, prev_count);

    // Start a new RX DMA transfer, starting back at the beginning of the RX buffer
    HAL_UART_Receive_DMA(&uart->handle, (uint8_t*) uart->rx_buf, UART_RX_BUF_SIZE);
}

/*
 * Returns the number of bytes received and stored in the buffer so far through RX DMA.
 */
uint32_t uart_get_rx_count(UART *uart) {
    // The DMA's NDTR (or CNDTR) register counts down from the number of RX
    // bytes expected to 0 (at which point the DMA RX transfer is done)

    // uart->rx_dma_handle.Instance is actually a void* type, so we need to cast
    // it to its struct type to access the NDTR field/register
    return UART_RX_BUF_SIZE - ((DMA_Stream_TypeDef *) uart->rx_dma_handle.Instance)->NDTR;
}

bool uart_is_newline_char(char c) {
    return (c == '\r') || (c == '\n');
}

/*
 * Reads an UNSIGNED integer from UART input.
 * It can be typed either in decimal form (no prefix) or in hexadecimal form
 * (with a "0x" prefix). For example, "35" and "0x23" would both return 35.
 * "0xff" and "0xFF" would both return 255.
 */
uint32_t uart_read_uint(UART *uart) {
    uint32_t start = HAL_GetTick();
    // 60,000ms timeout
    while (HAL_GetTick() < start + 60000) {
        uint32_t count = uart_get_rx_count(uart);
        if (count > 0 && uart_is_newline_char(uart->rx_buf[count - 1])) {
            // Must add a \0 character to the end of the string to terminate it
            // as a C string, which is required when passing it to sscanf()
            // Don't need the \r or \n newline character anyway, so just replace it with \0
            uart->rx_buf[count - 1] = '\0';

            uint32_t value = 0;
            // TODO - use log attached to this UART struct?
            // sscanf() returns the number of values successfully matched,
            // so should be 0 (failed) or 1 (succeeded)

            // Match in hexadecimal format
            if (sscanf(uart->rx_buf, "0x%lx", &value) > 0) {
                // Restarting RX DMA clears the RX buffer and starts a new DMA
                // transfer
                uart_restart_rx_dma(uart);
                return value;
            }
            // Match in decimal format
            else if (sscanf(uart->rx_buf, "%lu", &value) > 0) {
                uart_restart_rx_dma(uart);
                return value;
            }
            // No match
            else {
                error(&g_log_def, "Failed to read uint from UART");
                uart_restart_rx_dma(uart);
                return 0;
            }
        }
    }

    error(&g_log_def, "Timed out trying to read uint from UART");
    uart_restart_rx_dma(uart);
    return 0;
}

/*
 * Reads a SIGNED integer from UART input.
 */
int32_t uart_read_int(UART *uart) {
    uint32_t start = HAL_GetTick();
    // 60,000ms timeout
    while (HAL_GetTick() < start + 60000) {
        uint32_t count = uart_get_rx_count(uart);
        if (count > 0 && uart_is_newline_char(uart->rx_buf[count - 1])) {
            // Must add a \0 character to the end of the string to terminate it
            // as a C string, which is required when passing it to sscanf()
            // Don't need the \r or \n newline character anyway, so just replace it with \0
            uart->rx_buf[count - 1] = '\0';

            int32_t value = 0;
            // TODO - use log attached to this UART struct?
            // sscanf() returns the number of values successfully matched,
            // so should be 0 (failed) or 1 (succeeded)
            if (sscanf(uart->rx_buf, "%ld", &value) > 0) {
                // Restarting RX DMA clears the RX buffer and starts a new DMA
                // transfer
                uart_restart_rx_dma(uart);
                return value;
            } else {
                error(&g_log_def, "Failed to read int from UART");
                uart_restart_rx_dma(uart);
                return 0;
            }
        }
    }

    error(&g_log_def, "Timed out trying to read int from UART");
    uart_restart_rx_dma(uart);
    return 0;
}

/*
 * Reads one character from UART input.
 */
char uart_read_char(UART *uart) {
    uint32_t start = HAL_GetTick();
    // 60,000ms timeout
    while (HAL_GetTick() < start + 60000) {
        uint32_t count = uart_get_rx_count(uart);
        if (count > 0 && uart_is_newline_char(uart->rx_buf[count - 1])) {
            // Must add a \0 character to the end of the string to terminate it
            // as a C string, which is required when passing it to sscanf()
            // Don't need the \r or \n newline character anyway, so just replace it with \0
            uart->rx_buf[count - 1] = '\0';

            char value = 0;
            // TODO - use log attached to this UART struct?
            // sscanf() returns the number of values successfully matched,
            // so should be 0 (failed) or 1 (succeeded)
            if (sscanf(uart->rx_buf, "%c", &value) > 0) {
                // Restarting RX DMA clears the RX buffer and starts a new DMA
                // transfer
                uart_restart_rx_dma(uart);
                return value;
            } else {
                error(&g_log_def, "Failed to read char from UART");
                uart_restart_rx_dma(uart);
                return 0;
            }
        }
    }

    error(&g_log_def, "Timed out trying to read char from UART");
    uart_restart_rx_dma(uart);
    return 0;
}



/*
 * From https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
 * To define an interrupt handler, "you just declare a void function with no arguments and the same
 * name as the corresponding entry in the vector table".
 * The vector table with the available peripheral interrupt handler names is in
 * the startup file (of assembly code), e.g. startup_stm32h743zitx.s.
 */

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  * This function is automatically called for TX transfer complete
  */
void DMA1_Stream0_IRQHandler(void) {
    // TODO - default UART
    if (g_uart_usart3 == NULL) {
        return;
    }
    HAL_DMA_IRQHandler(&g_uart_usart3->tx_dma_handle);
}

/**
  * @brief This function handles USART3 global interrupt.
  * This function is only called by the HAL for TX DMA complete, not RX DMA complete.
  */
// TODO - add others
void USART3_IRQHandler(void) {
    if (g_uart_usart3 == NULL) {
        return;
    }

    // Must call this here so that after a DMA transmission is complete, it
    // changes the UART handle's gState from busy to ready, allowing it to do
    // the next transmission
    HAL_UART_IRQHandler(&g_uart_usart3->handle);

    // Note that HAL_UART_TxCpltCallback() never gets called after a DMA
    // transmission is complete because our DMA does not use circular mode
}




/**
  * @brief This function handles DMA1 stream1 global interrupt.
  * This function is automatically called for RX transfer complete
  */
void DMA1_Stream1_IRQHandler(void) {
    // TODO - default UART
    if (g_uart_usart3 == NULL) {
        return;
    }
    HAL_DMA_IRQHandler(&g_uart_usart3->rx_dma_handle);
}

/*
 * This function is called in the UART IRQ handler when all possible bytes have
 * been received, i.e. number of bytes received is equal to the `Size` parameter
 * passed to HAL_UART_Receive_DMA().
 * This function is called from DMA1_Stream1_IRQHandler() -> HAL_DMA_IRQHandler() -> UART_DMAReceiveCplt() if all RX bytes have been received.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (!g_log_def_initialized) {
        return;
    }
    warning(&g_log_def, "UART RX buffer is full");
}
