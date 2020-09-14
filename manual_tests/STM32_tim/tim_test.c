/*
 * tim_test.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: A timer that uses LD2 to flash 'FINCH' in Morse code on the NUCLEO-G474RE. Based on https://visualgdb.com/tutorials/arm/stm32/timers/hal/
 */

#include <STM32_tim/tim.h>

#ifdef STM32G4xx
#include <stm32g4xx_hal_gpio.h>
#endif

static TimFunc *timer;
volatile uint8_t time_unit = 0;
volatile uint8_t led_toggle[] = {1,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,0,0,
						  1,0,0,1,1,1,0,0,
						  1,0,0,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0}; // Each line is a letter and the trailing space or end of word.
// Interrupt handler
void TIM2_IRQHandler(){
	HAL_TIM_IRQHandler(timer->handle);
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
	HAL_Init();
	clock_init();
	TimMode mode = {.mode = TIM_Mode_IT};

	timer = tim_config_base(TIM2, &mode, 62499, 815);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/* Flashes 'FINCH' on the evaluation kit LD2 LED.
 *
 * In this context, a time unit is 150 ms (20/6 Hz), so the entire message (54 time units) is transmitted in 8.1 seconds.
 * Update events occur at each other time unit (300 ms or 20/3 Hz).
 */
int main(){
	setup_tim();
	setup_gpio();
	tim_start(timer);

	return 0;
}
