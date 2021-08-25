/*
 * init.c
 *
 *  Created on: Jul. 24, 2021
 *      Author: bruno
 */

#include <common/stm32/mcu/HAL.h>
#include <common/stm32/mcu/Init.h>

/**
 * Initializes the Global MSP.
 * This function is called by HAL_Init().
 * This function definition overrides the weak function defined in the HAL.
 */
void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
 * @brief GPIO Clock Configuration (similar to MX_GPIO_Init())
 * @retval None
 */
void GPIOClock_Config(void) {
    // Enable clocks for all GPIO ports
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // Should only be available on the H7 series, not the G4 series
#if defined(GPIOH)
    __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
#if defined(GPIOI)
    __HAL_RCC_GPIOI_CLK_ENABLE();
#endif
#if defined(GPIOJ)
    __HAL_RCC_GPIOJ_CLK_ENABLE();
#endif
#if defined(GPIOK)
    __HAL_RCC_GPIOK_CLK_ENABLE();
#endif
}
