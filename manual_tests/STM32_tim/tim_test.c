/*
 * tim_test.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: A timer that uses LD2 to flash 'FINCH' in Morse code on the NUCLEO-G474RE. Based on https://visualgdb.com/tutorials/arm/stm32/timers/hal/
 */

#include "../../src/drivers/STM32_tim/tim.h"

#include "../../Generated_Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_gpio.h"

static TIM_HandleTypeDef timer;
volatile uint8_t time_unit = 0;
volatile uint8_t led_toggle[] = {1,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,0,0,
						  1,0,0,1,1,1,0,0,
						  1,0,0,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0}; // Each line is a letter and the trailing space or end of word.
// Interrupt handler
void TIM2_IRQHandler(){
	HAL_TIM_IRQHandler(&timer);
}

void TIM3_IRQHandler(){
	HAL_TIM_IRQHandler(&timer);
}

// Advance to the next signal on every second time unit
void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim){
	if(led_toggle[time_unit]){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	time_unit++;
	if(time_unit==54){ //Reset the cycle
		time_unit=0;
	}
}

// Advance to the next signal on every first time unit
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(led_toggle[time_unit]){
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	time_unit++;
	if(time_unit==54){ //Reset the cycle
		time_unit=0;
	}
}

// Setup the GPIO pins needed
void setup_gpio(){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef handle;
	handle.Pin=GPIO_PIN_5;
	handle.Mode=GPIO_MODE_OUTPUT_PP;
	handle.Pull=GPIO_PULLDOWN;
	handle.Speed=GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(GPIOA, &handle);
}

// Setup the timer and interrupt functions
void setup_tim(){
	tim_populate_handle(&timer, TIM2, 62499, TIM_COUNTERMODE_UP, 815, TIM_CLOCKDIVISION_DIV1, 0, TIM_AUTORELOAD_PRELOAD_ENABLE); // Update event = clock frequency / ((prescaler+1) * (period+1))
	__HAL_RCC_TIM2_CLK_ENABLE();
	tim_init(&timer, TIM_Base, 0, NULL);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/* Flashes 'FINCH' on the evaluation kit LD2 LED.
 *
 * In this context, a time unit is 150 ms (20/6 Hz), so the entire message (54 time units) is transmitted in 8.1 seconds.
 * Update events occur at each other time unit (300 ms or 20/3 Hz).
 */
void tim_test_morse(){
	setup_tim();
	setup_gpio();
	tim_start_IT(&timer, TIM_Base, 0);
}
