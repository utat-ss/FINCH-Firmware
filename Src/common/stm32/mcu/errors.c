/*
 * errors.c
 */

#include <common/stm32/mcu/errors.h>
#include <common/stm32/uart/log.h>

void Error_Handler(void) {
	// It is very useful to place a breakpoint at the beginning of this function
	// to catch all errors that call this function, especially when UART has not
	// been initialized yet so it can't print anything

	// If UART has already been initialized, a breakpoint is still useful here
	// because no file/line information on where the error occurred is passed to
	// this function, so pausing the program here allows you to check the stack
	// trace for what called this function

    if (g_log_def == NULL) {
        return;
    }
    error(g_log_def, "Generic error occurred, see %s() in %s", __FUNCTION__, __FILE__);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
	if (g_log_def == NULL) {
		return;
	}
	error(g_log_def, "HAL assertion failed: file %s, line %lu", file, line);
}
#endif /* USE_FULL_ASSERT */
