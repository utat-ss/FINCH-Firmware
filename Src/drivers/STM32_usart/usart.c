/*
 * usart.c
 *
 *  Created on: Oct 10, 2020
 *      Author: Ketan
 */


USART USART_Init(USART_TypeDef Instance, uint32_t BaudRate, uint32_t HwFlowCtl,
		GPIO_TypeDef *tx_port, uint16_t tx_pin, GPIO_TypeDef *rx_port,
		uint16_t rx_pin, GPIO_TypeDef *hw_port, uint16_t hw_pin, uint8_t alt)
{

	USART usart;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	usart.handle.Instance = Instance;
	usart.handle.Init.BaudRate = BaudRate;
	usart.handle.WordLength = UART_WORDLENGTH_8B;
	usart.handle.Init.StopBits = UART_STOPBITS_1;
	usart.handle.Init.Parity = UART_PARITY_NONE;
	usart.handle.Init.Mode = UART_MODE_TX_RX;
	usart.handle.Init.HwFlowCtl = HwFlowCtl;
	usart.handle.Init.OverSampling = UART_OVERSAMPLING_16;
	usart.handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	usart.handle.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	usart.handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&usart.handle, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&usart.handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&usart.handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&usart.handle) != HAL_OK)
  {
    Error_Handler();
  }

  //tx pin init
  usart.tx = gpio_init_alt(tx_port, tx_pin, alt);

  //rx pin init
  usart.rx = gpio_init_alt(rx_port, rx_pin, alt);


  //check for rs485 hardware flow
  if(HwFlowCtl != UART_HWCONTROL_NONE) {
	  usart.hw_enable = gpio_init_alt(hw_port, hw_pin, alt);
  }

  if(usart.handle->Instance==USART1)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART2)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART3)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART4)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART4_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART5)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART5_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART6)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART7)
  {

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART7_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART8)
  {

    __HAL_RCC_USART8_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART9)
  {

    __HAL_RCC_USART9_CLK_ENABLE();

  }

  if(usart.handle->Instance==USART10)
  {

    __HAL_RCC_USART10_CLK_ENABLE();

  }

  return usart;

}

void USART_DeInit(USART* huart)
{
  if(huart->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOD, STLINK_RX_Pin|STLINK_TX_Pin|GPIO_PIN_12);

  }

}
