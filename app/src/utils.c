/*
 * utils.c
 *
 *  Created on: Aug 14, 2025
 *      Author: sksgk
 */

#ifndef SRC_UTILS_C_
#define SRC_UTILS_C_


#include "utils.h"


void delay(uint32_t time_ms)
{
	HAL_Delay(time_ms);
}
uint32_t millis(void)
{
	return HAL_GetTick();
}

#endif /* SRC_UTILS_C_ */
