/*
 * tim_test.c
 *
 *  Created on: Jul 22, 2020
 *      Author: Cameron Rodriguez
 *
 * Overview: A timer that uses LD2 to flash 'FINCH' in Morse code on the NUCLEO-G474RE.
 */

#include "../../src/drivers/STM32G4_tim/tim.h"
#include "../../Generated_Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_gpio.h"

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

/* Flashes 'FINCH' on the evaluation kit LD2 LED.
 *
 * In this context, a time unit is 150 ms (20/6 Hz), so the entire message (54 time units) is transmitted in 8.1 seconds.
 * Update events occur at each other time unit (300 ms or 20/3 Hz).
 */
void tim_test_morse(){
	TIM_HandleTypeDef timer;
	uint8_t time_unit = 0;
	uint8_t old_counter = 0, new_counter;
	uint8_t led_toggle[] = {1,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,0,0,
						  1,0,0,1,1,1,0,0,
						  1,0,0,1,1,1,1,0,0,1,1,1,0,0,
						  1,1,1,1,1,1,1,1,0,0,0,0,0,0}; // Each line is a letter and the trailing space or end of word.

	tim_populate_handle(&timer, TIM2, 62499, TIM_COUNTERMODE_UP, 815, TIM_CLOCKDIVISION_DIV1, 0, TIM_AUTORELOAD_PRELOAD_ENABLE); // Update event = clock frequency / ((prescaler+1) * (period+1))
	__HAL_RCC_TIM2_CLK_ENABLE();
	setup_gpio();
	tim_init(&timer, TIM_Base, 0, NULL);
	tim_start(&timer, TIM_Base, 0);

	// Flash the message
	for(;;){
			if(time_unit==54){
				time_unit=0;
			}

			if(__HAL_TIM_GET_COUNTER(&timer)<408){ // First time unit
				new_counter=0;
			}else{ // Second time unit
				new_counter=1;
			}

			if(new_counter!=old_counter){ // Toggle light as needed
				old_counter=new_counter;
				if(led_toggle[time_unit]){
					HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				}
				time_unit++;
			}
		}
}
