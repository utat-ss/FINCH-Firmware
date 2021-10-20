/*
Timer.c

Date created: 2021-10-19
Last modified: 2021-10-19
Author: Cameron Rodriguez

Wrapper for the HAL timer (TIM) drivers. Timers to be implemeneted
  * Basic timer with interrupt
    * Autoreload toggle
    * Update toggle (raises flag after number of loops completed)
    * Countup toggle
  * Timer with callback function

Not currently supported
  * output compare (OC) - modify signal when counter == catch/capture register
  * pulse width modulation (PWM)
  * OnePulse mode (variant of Oc)
  * input capture (IC) - latch counter value after a signal is recieved
  * DMA burst mode

Still need to determine which timers will be enabled
 */

#include <common/stm32/timer/Timer.h>



