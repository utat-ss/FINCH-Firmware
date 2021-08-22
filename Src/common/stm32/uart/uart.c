/*
 * uart.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Bruno
 */

// Don't include uart.h here or else it will produce a circular include error

// log.h includes uart.h
#include <common/stm32/uart/log.h>
#include <common/stm32/util/util.h>
#include <nucleo_g474re/g474re_config.h>
#include <nucleo_h743zi2/h743zi2_config.h>


// Pointer to each specific UART peripheral (if used) - needed for use in ISRs
UART *g_uart_usart1 = NULL;
UART *g_uart_usart2 = NULL;
UART *g_uart_usart3 = NULL;
UART *g_uart_uart4 = NULL;
UART *g_uart_uart5 = NULL;
UART *g_uart_usart6 = NULL;
UART *g_uart_uart7 = NULL;
UART *g_uart_uart8 = NULL;
UART *g_uart_lpuart1 = NULL;

// Default UART - can be used globally
UART *g_uart_def = NULL;



void uart_init_dma(UART* uart, USART_TypeDef *instance) {
	// DMA priorities: TX low, RX medium
	// UART should be relatively low priority compared to other peripherals that
	// are more time-critical (e.g. DCMI, SD card) since UART is generally only
	// used for logging and debugging
	// RX should have a higher priority than TX because we don't want to delay
	// bytes arriving so they aren't lost, whereas TX will not lose bytes if it
	// is delayed

	/* DMA controller clock enable */
	// This MUST come BEFORE initializing the DMA handles and calling
	// HAL_DMA_Init() or else the DMAs will not work
    __HAL_RCC_DMA1_CLK_ENABLE();


    // TODO - Confluence page with DMA channel/stream allocations?
    /* DMA interrupt init */
#if defined(STM32G4)
    const IRQn_Type tx_dma_irq = DMA1_Channel1_IRQn;
    const IRQn_Type rx_dma_irq = DMA1_Channel2_IRQn;
#elif defined(STM32H7)
    const IRQn_Type tx_dma_irq = DMA1_Stream0_IRQn;
    const IRQn_Type rx_dma_irq = DMA1_Stream1_IRQn;
#endif

    // Configure and enable the DMA interrupts
    // Set the preempt priority to 8, around the middle of the 0-15 range
    // Give stream 1 (RX) a more important subpriority than stream 0 (TX)
    HAL_NVIC_SetPriority(tx_dma_irq, 8, 1);
    HAL_NVIC_EnableIRQ(tx_dma_irq);
    HAL_NVIC_SetPriority(rx_dma_irq, 8, 0);
    HAL_NVIC_EnableIRQ(rx_dma_irq);


	// Select DMA parameters for the specific UxART peripheral
	uint32_t tx_dma_request;
	uint32_t rx_dma_request;

	if (instance == USART1) {
		tx_dma_request = DMA_REQUEST_USART1_TX;
        rx_dma_request = DMA_REQUEST_USART1_RX;
	}
	if (instance == USART2) {
		tx_dma_request = DMA_REQUEST_USART2_TX;
        rx_dma_request = DMA_REQUEST_USART2_RX;
	}
	if (instance == USART3) {
		tx_dma_request = DMA_REQUEST_USART3_TX;
		rx_dma_request = DMA_REQUEST_USART3_RX;
	}
	if (instance == UART4) {
		tx_dma_request = DMA_REQUEST_UART4_TX;
        rx_dma_request = DMA_REQUEST_UART4_RX;
	}
	if (instance == UART5) {
		tx_dma_request = DMA_REQUEST_UART5_TX;
        rx_dma_request = DMA_REQUEST_UART5_RX;
	}
#if defined(STM32H7)
	if (instance == USART6) {
		tx_dma_request = DMA_REQUEST_USART6_TX;
        rx_dma_request = DMA_REQUEST_USART6_RX;
	}
	if (instance == UART7) {
		tx_dma_request = DMA_REQUEST_UART7_TX;
        rx_dma_request = DMA_REQUEST_UART7_RX;
	}
	if (instance == UART8) {
		tx_dma_request = DMA_REQUEST_UART8_TX;
        rx_dma_request = DMA_REQUEST_UART8_RX;
	}
	if (instance == LPUART1) {
		// The H7 series does not support the standard DMA with LPUART1
		Error_Handler();
	}
#else
	if (instance == LPUART1) {
		tx_dma_request = DMA_REQUEST_LPUART1_TX;
		rx_dma_request = DMA_REQUEST_LPUART1_RX;
	}
#endif

    /* TX DMA Init */
    // The G4 HAL has `Instance` with type `DMA_Channel_TypeDef*`,
	// while the H7 HAL has `Instance` with type `DMA_Stream_TypeDef*`
	// G4 uses the term "channel" and starts numbering at 1
    // H7 uses the term "stream" and starts numbering at 0
#if defined(STM32G4)
    uart->tx_dma_handle.Instance = DMA1_Channel1;
#elif defined(STM32H7)
    uart->tx_dma_handle.Instance = DMA1_Stream0;
#endif
    uart->tx_dma_handle.Init.Request = tx_dma_request;
    uart->tx_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    uart->tx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    uart->tx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    uart->tx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    uart->tx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    uart->tx_dma_handle.Init.Mode = DMA_NORMAL;
    uart->tx_dma_handle.Init.Priority = DMA_PRIORITY_LOW;
#if defined(STM32H7)
    // Only H7 supports FIFO mode and burst
    uart->tx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    uart->tx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    uart->tx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    uart->tx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#endif
    if (HAL_DMA_Init(&uart->tx_dma_handle) != HAL_OK) {
        Error_Handler();
    }
    __HAL_LINKDMA(&uart->handle, hdmatx, uart->tx_dma_handle);

    /* RX DMA Init */
#if defined(STM32G4)
    uart->rx_dma_handle.Instance = DMA1_Channel2;
#elif defined(STM32H7)
    uart->rx_dma_handle.Instance = DMA1_Stream1;
#endif
    uart->rx_dma_handle.Init.Request = rx_dma_request;
    uart->rx_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
    uart->rx_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;
    uart->rx_dma_handle.Init.MemInc = DMA_MINC_ENABLE;
    uart->rx_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    uart->rx_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    uart->rx_dma_handle.Init.Mode = DMA_NORMAL;
    uart->rx_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;
#if defined(STM32H7)
    uart->rx_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    uart->rx_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
    uart->rx_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;
    uart->rx_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
#endif
    if (HAL_DMA_Init(&uart->rx_dma_handle) != HAL_OK) {
        Error_Handler();
    }
    __HAL_LINKDMA(&uart->handle, hdmarx, uart->rx_dma_handle);
}

void uart_init_clk_and_nvic(UART* uart, USART_TypeDef *instance) {
	// Peripheral clock configuration
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

#if defined(STM32G4)
	if (instance == USART1) {
        // UART peripheral 1
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    }
	else if (instance == USART2) {
        // UART peripheral 2
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    }
	else if (instance == USART3) {
        // UART peripheral 3
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    }
	else if (instance == UART4) {
        // UART peripheral 4
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
        PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
    }
	else if (instance == UART5) {
        // UART peripheral 5
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5;
        PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
    }
	else if (instance == LPUART1) {
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
		PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
	}
#elif defined(STM32H7)
	if (	instance == USART1 ||
			instance == USART6) {
	    // UART peripheral 1 or 6
	    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART16;
        PeriphClkInitStruct.Usart16ClockSelection =
        		RCC_USART16CLKSOURCE_D2PCLK2;
	}
	else if (
			instance == USART2 ||
			instance == USART3 ||
			instance == UART4 ||
			instance == UART5 ||
			instance == UART7 ||
			instance == UART8) {
	    // UART peripheral 2, 3, 4, 5, 7, or 8
	    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
        PeriphClkInitStruct.Usart234578ClockSelection =
        		RCC_USART234578CLKSOURCE_D2PCLK1;
	}
	else if (instance == LPUART1) {
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPUART1;
		PeriphClkInitStruct.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK4;
	}
#endif

	// Now actually configure the peripheral clock
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
	    Error_Handler();
    }


	// Enable peripheral clock for the appropriate UxART peripheral
	// Also select IRQ value for the specific UxART peripheral
	IRQn_Type irq;

	if (instance == USART1) {
		__HAL_RCC_USART1_CLK_ENABLE();
        irq = USART1_IRQn;
	}
	if (instance == USART2) {
		__HAL_RCC_USART2_CLK_ENABLE();
        irq = USART2_IRQn;
	}
	if (instance == USART3) {
		__HAL_RCC_USART3_CLK_ENABLE();
		irq = USART3_IRQn;
	}
	if (instance == UART4) {
		__HAL_RCC_UART4_CLK_ENABLE();
        irq = UART4_IRQn;
	}
	if (instance == UART5) {
		__HAL_RCC_UART5_CLK_ENABLE();
        irq = UART5_IRQn;
	}
#if defined(STM32H7)
	if (instance == USART6) {
		__HAL_RCC_USART6_CLK_ENABLE();
        irq = USART6_IRQn;
	}
	if (instance == UART7) {
		__HAL_RCC_UART7_CLK_ENABLE();
        irq = UART7_IRQn;
	}
	if (instance == UART8) {
		__HAL_RCC_UART8_CLK_ENABLE();
        irq = UART8_IRQn;
	}
#endif
	if (instance == LPUART1) {
		__HAL_RCC_LPUART1_CLK_ENABLE();
		irq = LPUART1_IRQn;
	}


    // Configure and enable the UART interrupt
    // This is necessary for TX (even though it's in DMA mode), enabling
    // interrupts when a transmission is done so it can call the IRQ handler,
    // which sets the UART handle back to a ready state to be able to do the
    // next DMA transmission
    HAL_NVIC_SetPriority(irq, 8, 2);
    HAL_NVIC_EnableIRQ(irq);

    // Don't need to call __enable_irq() - global interrupts are already enabled
}

/*
 * alternate - e.g. GPIO_AF7_USART3 for an instance of USART3
 */
void uart_init_base(UART* uart, MCU *mcu,
        USART_TypeDef *instance, UARTBaud baud, uint8_t alternate,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

	// Initialize DMA
	uart_init_dma(uart, instance);

    // Initialize TX and RX GPIO pins
	// Low GPIO speed on the H743 MCU supports up to 12MHz but UART can operate
	// up to 12.5Mbit/s, so use medium GPIO speed to be safe
    // TX pin init
	gpio_alt_func_init(&uart->tx_gpio, mcu, tx_port, tx_pin, alternate,
    		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
    // RX pin init
	gpio_alt_func_init(&uart->rx_gpio, mcu, rx_port, rx_pin, alternate,
    		GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);

	// Initialize peripheral clock and NVIC
	uart_init_clk_and_nvic(uart, instance);

	// Set up UART struct and handle
	uart->mcu = mcu;
	uart->handle.Instance = instance;
	uart->handle.Init.BaudRate = baud;
	uart->handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart->handle.Init.StopBits = UART_STOPBITS_1;
	uart->handle.Init.Parity = UART_PARITY_NONE;
	uart->handle.Init.Mode = UART_MODE_TX_RX;
	uart->handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uart->handle.Init.OverSampling = UART_OVERSAMPLING_16;
	uart->handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	uart->handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	uart->handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	// The TX FIFO threshold is for the number of EMPTY slots in the FIFO, while
	// the RX FIFO threshold is for the number of FILLED slots in the FIFO
	// (RM0433 p.2046)
	// These thresholds honestly shouldn't matter since we are not using the
	// FIFO threshold interrupts
	if (HAL_UARTEx_SetTxFifoThreshold(&uart->handle, UART_TXFIFO_THRESHOLD_7_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&uart->handle, UART_RXFIFO_THRESHOLD_7_8)
			!= HAL_OK) {
		Error_Handler();
	}
    if (HAL_UARTEx_EnableFifoMode(&uart->handle) != HAL_OK) {
        Error_Handler();
    }
}

/*
 * Sets the necessary global variable(s) for this UART.
 */
void uart_set_globals(UART *uart) {
    // Save a pointer to the UART struct to the appropriate global variable
    if (uart->handle.Instance == USART1) {
        g_uart_usart1 = uart;
    }
    if (uart->handle.Instance == USART2) {
        g_uart_usart2 = uart;
    }
    if (uart->handle.Instance == USART3) {
        g_uart_usart3 = uart;
    }
    if (uart->handle.Instance == UART4) {
        g_uart_uart4 = uart;
    }
    if (uart->handle.Instance == UART5) {
        g_uart_uart5 = uart;
    }
#if defined(STM32H7)
    if (uart->handle.Instance == USART6) {
        g_uart_usart6 = uart;
    }
    if (uart->handle.Instance == UART7) {
        g_uart_uart7 = uart;
    }
    if (uart->handle.Instance == UART8) {
        g_uart_uart8 = uart;
    }
#endif
    if (uart->handle.Instance == LPUART1) {
		g_uart_lpuart1 = uart;
	}

    // Set default UART global variable
    if (g_uart_def == NULL) {
        g_uart_def = uart;
    }
}

// Initialize normal UART
void uart_init(UART* uart, MCU *mcu,
        USART_TypeDef *instance, UARTBaud baud, uint8_t alternate,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin) {

	uart_init_base(uart, mcu, instance, baud, alternate, tx_port, tx_pin,
			rx_port, rx_pin);

	if (HAL_UART_Init(&uart->handle) != HAL_OK) {
		Error_Handler();
	}

	// Common buffer for printing messages
	char buf[140];

	// Print message at original baud rate
    snprintf(buf, sizeof(buf), "Started UART at %lu baud\r\n", (uint32_t) baud);
    uart_write(uart, (uint8_t*) buf, strlen(buf));

    // Serial monitors often default to 9600 baud, so in case the user has
    // theirs set to 9600, we switch the MCU's UART to 9600, print a warning
    // message, then switch back to the original baud rate
    if (baud != UART_BAUD_9600) {
    	uart_set_baud(uart, UART_BAUD_9600);

        // Print warning message at 9600 baud
        // Note this takes over 100ms to write to UART at 9600 baud, so it can
        // be disabled if you need to speed up MCU initialization
        snprintf(buf, sizeof(buf),
                "WARNING: UART will be operating at %lu baud\r\n"
                "Your serial monitor is set to 9600 baud\r\n"
                "Change your serial monitor's baud rate!\r\n",
                (uint32_t) baud);
        // Must use uart_write here because the Log struct has not been
        // initialized yet
        uart_write(uart, (uint8_t*) buf, strlen(buf));

        // Restore the original baud rate and reinitialize UART
        uart_set_baud(uart, baud);
    }

	// Set global variables after HAL_UART_Init() to make sure they are
	// initialized before being used
	uart_set_globals(uart);

	// Start receiving data through RX DMA
	uart_restart_rx_dma(uart);

	snprintf(buf, sizeof(buf), "Initialized UART\r\n");
	uart_write(uart, (uint8_t*) buf, strlen(buf));

	log_init(&uart->log, uart);
}

/*
 * Initialize normal UART for the specified board. This function is provided as
 * a convenient initializer, given that UART is used on every MCU on every
 * board. This function is useful to reduce code duplication in the tests in
 * Manual_Tests/common that are supposed to work on all boards.
 */
void uart_init_for_board(UART* uart, MCU *mcu) {
	if (mcu->board == MCU_BOARD_NUCLEO_G474RE) {
		// 115,200 baud should work for LPUART1 with a higher frequency clock
		// than a 32.768kHz LSE
		// If this doesn't work, try 9,600 baud
		uart_init(uart, mcu, G474RE_UART_INST, UART_DEF_BAUD, G474RE_UART_AF,
				G474RE_UART_TX_PORT, G474RE_UART_TX_PIN,
				G474RE_UART_RX_PORT, G474RE_UART_RX_PIN);
	} else if (mcu->board == MCU_BOARD_NUCLEO_H743ZI2) {
		uart_init(uart, mcu, H743ZI2_UART_INST, UART_DEF_BAUD, H743ZI2_UART_AF,
				H743ZI2_UART_TX_PORT, H743ZI2_UART_TX_PIN,
				H743ZI2_UART_RX_PORT, H743ZI2_UART_RX_PIN);
	}
}

// Initialize UART + RS-485
void uart_init_with_rs485(UART* uart, MCU *mcu,
        USART_TypeDef *instance, UARTBaud baud, uint8_t alternate,
		GPIO_TypeDef *tx_port, uint16_t tx_pin,
		GPIO_TypeDef *rx_port, uint16_t rx_pin,
		GPIO_TypeDef *de_port, uint16_t de_pin) {

	uart_init_base(uart, mcu, instance, baud, alternate, tx_port, tx_pin,
			rx_port, rx_pin);

	// RS-485 DE pin init
	gpio_alt_func_init(&uart->de_gpio, mcu, de_port, de_pin, alternate,
			GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);

	// Initialize both UART and RS-485
	if (HAL_RS485Ex_Init(&uart->handle, UART_DE_POLARITY_HIGH, 0, 0)
			!= HAL_OK) {
		Error_Handler();
	}

	// Set global variables after HAL_RS485Ex_Init() to make sure they are
    // initialized before being used
	uart_set_globals(uart);

	if (g_log_def != NULL) {
        info(g_log_def, "Initialized UART + RS-485");
    }
}

/*
 * This should only be used to change the baud after initializing UART.
 */
void uart_set_baud(UART *uart, UARTBaud baud) {
	// Abort all ongoing transfers (this function executes in blocking mode)
	HAL_UART_Abort(&uart->handle);
	// Deinitialize the UART peripheral
	HAL_UART_DeInit(&uart->handle);
	// Change baud rate to desired
	uart->handle.Init.BaudRate = baud;
	// Reinitialize UART at new baud
	if (HAL_UART_Init(&uart->handle) != HAL_OK) {
		Error_Handler();
	}
}

void uart_wait_for_tx_ready(UART *uart) {
	// If a TX process is already ongoing, wait for it to finish
	// If a previous DMA TX is in progress, when the TX is done the UART ISR
	// will set gState to ready
	// This is necessary because if you call HAL_UART_Transmit() or
	// HAL_UART_Transmit_DMA() when gState is not ready, it fails and returns
	// busy (without transmitting anything)
	uint32_t start = HAL_GetTick();
	while (uart->handle.gState != HAL_UART_STATE_READY) {
		// Timeout (this should never happen)
		if (HAL_GetTick() > start + UART_TX_TIMEOUT_MS) {
			// Note that this could get stuck infinitely because Error_Handler()
			// logs a message over UART, which calls uart_wait_for_tx_ready()
			// first, which can call Error_Handler() again, and so on
			Error_Handler();
			return;
		}
	}
}

/*
 * Writes data in blocking mode
 */
void uart_write(UART *uart, uint8_t *buf, uint32_t count) {
	// Must wait until the previous TX transfer has completed (could have a TX
	// DMA transfer ongoing)
	uart_wait_for_tx_ready(uart);

	// Don't need to copy the data bytes to the UART struct's TX buffer
	// since this function just blocks until the transmission is complete
	HAL_UART_Transmit(&uart->handle, buf, count, UART_TX_TIMEOUT_MS);
}

/*
 * Writes data in DMA mode
 * Note when this function returns, the DMA transmission is probably not
 * finished
 * Note this may not work correctly if you call it from an ISR
 * Note that buf should be a SEPARATE BUFFER from uart->tx_buf
 */
void uart_write_dma(UART *uart, uint8_t *buf, uint32_t count) {
	// Must wait until the previous TX DMA transfer has completed
    uart_wait_for_tx_ready(uart);

    // Copy data from the buffer passed in as an argument to the UART struct's
    // TX buffer
    // This ensures the bytes to be written remain stable (unmodified) while the
    // DMA is transferring them to the UART output
    // This must occur AFTER waiting for UART TX to be ready (i.e. after waiting
    // for the previous DMA transfer to complete)
    // If we write to uart->tx_buf before the previous transfer has completed,
	// it overwrites the data of the previous transfer and corrupts the bytes
    // that have not been sent out yet by the previous transfer
    // Note the cast discards the `volatile` qualifier
    util_safe_memcpy((uint8_t *) uart->tx_buf, sizeof(uart->tx_buf),
    		buf, count);

    // Transmit the data from the UART struct's TX buffer
    // Note the cast discards the `volatile` qualifier
	HAL_UART_Transmit_DMA(&uart->handle, (uint8_t *) uart->tx_buf, count);

	// In the future, could modify this implementation so it doesn't have to
	// wait for the previous DMA transfer to finish before starting the next
	// one
	// Could make it pause the previous DMA transfer, discard bytes from the
	// buffer that have already been transmitted, append the bytes of the
	// new message to the buffer, then start a new DMA transfer
}

/*
 * This function works for starting RX for the first time or for restarting it
 * after the first time.
 */
void uart_restart_rx_dma(UART *uart) {
    // Get number of bytes already transferred
    // Do this before aborting the RX transfer just in case NDTR is changed
    uint32_t prev_count = uart_get_rx_count(uart);

    // Abort the ongoing UART RX DMA transfer
    // This function blocks until the abort is complete and RxState is set to
    // ready
    HAL_UART_AbortReceive(&uart->handle);

    // Set bytes in RX buffer to zero, but only the bytes that were filled by
    // the previous RX transfer
    // Technically this is not required, but do it to make it easier to view the
    // contents of the buffer in the debugger (only see the bytes filled by the
    // current RX transfer)
    // Note the cast discards the `volatile` qualifier
    memset((uint8_t *) uart->rx_buf, 0, prev_count);

    // Start a new RX DMA transfer, starting back at the beginning of the RX
    // buffer
    // Note the cast discards the `volatile` qualifier
    HAL_UART_Receive_DMA(&uart->handle, (uint8_t *) uart->rx_buf,
    		sizeof(uart->rx_buf));
}

/*
 * Returns the number of bytes received and stored in the buffer so far through
 * RX DMA.
 */
uint32_t uart_get_rx_count(UART *uart) {
    // The DMA's CNDTR (or NDTR) register counts down from the number of RX
    // bytes expected to 0 (at which point the DMA RX transfer is done)

    // uart->rx_dma_handle.Instance is actually a void* type, so we need to cast
    // it to its struct type to access the NDTR field/register

#if defined(STM32G4)
    uint32_t ndtr =
    		((DMA_Channel_TypeDef*) uart->rx_dma_handle.Instance)->CNDTR;
#elif defined(STM32H7)
    uint32_t ndtr =
    		((DMA_Stream_TypeDef*) uart->rx_dma_handle.Instance)->NDTR;
#endif

    return sizeof(uart->rx_buf) - ndtr;
}

bool uart_is_newline_char(char c) {
    return (c == '\r') || (c == '\n');
}

/*
 * Reads a single value (no more than 64 bits) from UART input.
 *
 * Value can be either a uint32_t (32 bits unsigned), int32_t (32 bits signed),
 * double (64 bits floating-point), or char (8 bits signed). This function
 * returns a uint64_t as a way of storing the raw bits because it is large
 * enough to store the value in all four cases, but the result will need to be
 * casted or reinterpreted.
 *
 * Try two different format strings to accommodate the case of reading a uint or
 * int, which can be in either hexadecimal or decimal form.
 *
 * To read floating-point values, must enable the -u_scanf_float flag in the
 * linker settings
 * See Project Properties > C/C++ Build > Settings > Tool Settings
 * > MCU Settings
 * Note this adds about 6,384 bytes to the `text` section in the compiled binary
 */
uint64_t uart_read_value(UART *uart, char *tx_format, va_list tx_args,
		char *format1, char *format2) {
	// Log an output message before receiving input
	log_log(&uart->log, LOG_LEVEL_INFO, tx_format, tx_args);

	// Restart RX DMA to clear any characters that are already in the buffer
	// (e.g. from a previous call to uart_wait_for_key_press()) and start a new
	// DMA transfer
	uart_restart_rx_dma(uart);

	uint64_t value = 0;

    while (true) {
        uint32_t count = uart_get_rx_count(uart);

        if (count > 0 && uart_is_newline_char(uart->rx_buf[count - 1])) {
            // Must add a \0 character to the end of the string to terminate it
            // as a C string, which is required when passing it to sscanf()
            // Don't need the \r or \n newline character anyway, so just replace
            // it with \0
            uart->rx_buf[count - 1] = '\0';

            // sscanf() returns the number of values successfully matched, so
            // should be 0 (failed) or 1 (succeeded)

            // sscanf() does not know the type of the memory location that the
            // pointer (&value) is pointing to, it just places the result in
            // that memory location assuming there is enough space there

            // Match in first format
            // Note the cast discards the `volatile` qualifier
            if (sscanf((char *) uart->rx_buf, format1, &value) > 0) {
            }
            // Match in second format
            // Note the cast discards the `volatile` qualifier
            else if (sscanf((char *) uart->rx_buf, format2, &value) > 0) {
            }
            // No match
            else {
                error(&uart->log, "Failed to read value from UART");
                value = 0;
            }

            // Break out of while loop
            break;
        }

        // If using an RTOS, should yield the thread right here
    }

    uart_restart_rx_dma(uart);
	return value;
}

/*
 * Reads an UNSIGNED integer from UART input.
 * It can be typed either in decimal form (no prefix) or in hexadecimal form
 * (with a "0x" prefix). For example, "35" and "0x23" would both return 35.
 * "0xff" and "0xFF" would both return 255.
 */
uint32_t uart_read_uint(UART *uart, char *tx_format, ...) {
	// %lu caps the saved value at 4294967295
	// %llu caps the saved value at 2147483647 (not sure why)
	// %llx fails completely
	va_list tx_args;
	va_start(tx_args, tx_format);
	uint64_t raw_value = uart_read_value(uart, tx_format, tx_args,
			"0x%lx", "%lu");
	va_end(tx_args);

	// Return value is a uint64_t, so need to cast it to uint32_t
	uint32_t uint_value = (uint32_t) raw_value;
	return uint_value;
}

/*
 * Reads a SIGNED integer from UART input.
 */
int32_t uart_read_int(UART *uart, char *tx_format, ...) {
	// %ld caps the saved value at 2147483647 and -2147483648
	// %lld also caps the saved value at 2147483647 and -2147483648
	// %llx fails completely
	va_list tx_args;
	va_start(tx_args, tx_format);
	uint64_t raw_value = uart_read_value(uart, tx_format, tx_args,
			"0x%lx", "%ld");
	va_end(tx_args);

	// Return value is a uint64_t, so need to cast it to int32_t
	int32_t int_value = (int32_t) raw_value;
	return int_value;
}

/*
 * Reads a floating-point number from UART input.
 */
double uart_read_double(UART *uart, char *tx_format, ...) {
	// %f - float
	// %lf - double
	// %Lf - long double (same as double, just don't use it)
	// %llf, %LLf - unsupported
	// Only need one format string, so just make the second format string blank
	va_list tx_args;
	va_start(tx_args, tx_format);
	uint64_t raw_value = uart_read_value(uart, tx_format, tx_args, "%lf", "");
	va_end(tx_args);

	// Return value is a uint64_t, so need to reinterpret the underlying bits as
	// a double
	// Note this is NOT the same thing as a standard cast from one data type to
	// another, instead this is like `reinterpret_cast` in C++
	// Get a pointer to the uint64_t value, cast the pointer to reinterpret it
	// as a double, then dereference the pointer to get the underlying value as
	// a double
	double double_value = * ((double *) (&raw_value));
	return double_value;
}

/*
 * Reads one character from UART input.
 */
char uart_read_char(UART *uart, char *tx_format, ...) {
	// Only need one format string, so just make the second format string blank
	va_list tx_args;
	va_start(tx_args, tx_format);
	uint64_t raw_value = uart_read_value(uart, tx_format, tx_args, "%c", "");
	va_end(tx_args);

	// Return value is a uint64_t, so need to cast it to char
	char char_value = (char) raw_value;
	return char_value;
}

/*
 * Waits for one character from UART input (any key pressed).
 * This should only be used for testing, NOT in flight.
 */
void uart_wait_for_key_press(UART *uart) {
	info(&uart->log, "Press any key to continue...");

	// Restart RX DMA to clear any characters that are already in the buffer
	// (e.g. from a previous call to uart_wait_for_key_press()) and start a new
	// DMA transfer
	// Make sure to do this AFTER logging the message above
	uart_restart_rx_dma(uart);

    while (true) {
        if (uart_get_rx_count(uart) > 0) {
        	verbose(&uart->log, "Received key press");
			uart_restart_rx_dma(uart);
			return;
        }

        // If using an RTOS, should yield the thread right here
    }
}




// -----------------------------------------------------------------------------
// Interrupt handlers (ISRs)

/*
 * From https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
 * To define an interrupt handler, "you just declare a void function with no
 * arguments and the same name as the corresponding entry in the vector table".
 * The vector table with the available peripheral interrupt handler names is in
 * the startup file (of assembly code), e.g. startup_stm32h743zitx.s.
 */

/**
  * @brief This function handles DMA1 channel1 or stream0 global interrupt.
  * This function is automatically called for TX transfer complete.
  */
#if defined(STM32G4)
void DMA1_Channel1_IRQHandler(void) {
#elif defined(STM32H7)
void DMA1_Stream0_IRQHandler(void) {
#endif

    if (g_uart_def == NULL) {
        return;
    }
    HAL_DMA_IRQHandler(&g_uart_def->tx_dma_handle);
}

/**
  * @brief This function handles USART3 global interrupt.
  * This function is only called by the HAL for TX DMA complete, not RX DMA
  * complete.
  */
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

// Same as USART3_IRQHandler(), but for other UART peripherals
void USART1_IRQHandler(void) {
    if (g_uart_usart1 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_usart1->handle);
}
void USART2_IRQHandler(void) {
    if (g_uart_usart2 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_usart2->handle);
}
void UART4_IRQHandler(void) {
    if (g_uart_uart4 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_uart4->handle);
}
void UART5_IRQHandler(void) {
    if (g_uart_uart5 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_uart5->handle);
}
void USART6_IRQHandler(void) {
    if (g_uart_usart6 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_usart6->handle);
}
void UART7_IRQHandler(void) {
    if (g_uart_uart7 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_uart7->handle);
}
void UART8_IRQHandler(void) {
    if (g_uart_uart8 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_uart8->handle);
}
void LPUART1_IRQHandler(void) {
    if (g_uart_lpuart1 == NULL) {
        return;
    }
    HAL_UART_IRQHandler(&g_uart_lpuart1->handle);
}





/**
  * @brief This function handles DMA1 channel2/stream1 global interrupt.
  * This function is automatically called for RX transfer complete.
  */
#if defined(STM32G4)
void DMA1_Channel2_IRQHandler(void) {
#elif defined(STM32H7)
void DMA1_Stream1_IRQHandler(void) {
#endif

    if (g_uart_def == NULL) {
        return;
    }
    HAL_DMA_IRQHandler(&g_uart_def->rx_dma_handle);
}

/*
 * This function is called in the DMA1 Channel2/Stream1 IRQ handler when all
 * possible bytes have been received, i.e. number of bytes received is equal to
 * the `Size` parameter passed to HAL_UART_Receive_DMA().
 * This function is called from
 * DMA1_Channel2_IRQHandler()/DMA1_Stream1_IRQHandler() -> HAL_DMA_IRQHandler()
 * -> UART_DMAReceiveCplt()
 * if all possible RX bytes have been received.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (g_log_def == NULL) {
        return;
    }
    warning(g_log_def, "UART RX buffer is full");
}
