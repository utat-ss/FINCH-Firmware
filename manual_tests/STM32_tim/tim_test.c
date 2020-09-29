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

static TimFunc timer;
volatile uint8_t time_unit = 0;
volatile uint8_t led_toggle[] = {1,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,0,0,
						  1,0,0,1,1,1,0,0,
						  1,0,0,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0}; // Each line is a letter and the trailing space or end of word.

// Interrupt handler, advances at the end of each period
void TIM2_IRQHandler(){
	if(tim_get_IT_flag(&(timer.handle), TIM_IT_UPDATE)){
		tim_clear_IT_flag(&(timer.handle), TIM_IT_UPDATE);
		if(led_toggle[time_unit]){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}

		time_unit++;
		if(time_unit==54){ //Reset the cycle
			time_unit=0;
		}
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

	timer = tim_config_base(TIM2, mode, 62499, 1631);
}

/* Flashes 'FINCH' on the evaluation kit LD2 LED.
 *
 * In this context, a time unit is 150 ms (20/6 Hz), so the entire message (54 time units) is transmitted in 8.1 seconds.
 * Update events occur at each other time unit (300 ms or 20/3 Hz).
 */
int main(){
	setup_tim();
	setup_gpio();
	tim_start(&timer);

	return 0;
}
