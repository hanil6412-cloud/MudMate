/*
 * fan.c
 *
 *  Created on: Aug 26, 2025
 *      Author: sksgk
 */


#include <Heating Module Hed/fan.h>
#include "uart.h"

bool fanInit(void)
{
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
  htim3.Instance->PSC = 2159;
  htim3.Instance->ARR = 999;
  __HAL_TIM_SET_AUTORELOAD(&htim3, 999);
  bool ret = (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) == HAL_OK);
  uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
  uartPrintf(_DEF_CH1, "Fan Init: %d, ARR: %d, PCLK1: %d\r\n", ret, __HAL_TIM_GET_AUTORELOAD(&htim3), pclk1);
  return ret;
}

void fanSetDuty(uint16_t duty1000)
{
  if (duty1000 > (PWM_DUTY_STEPS - 1)) duty1000 = (PWM_DUTY_STEPS - 1);
  uint16_t min_cut = (uint16_t)(PWM_DUTY_STEPS * PWM_DUTY_MIN_FRAC + 0.5f);
  if (duty1000 < min_cut) duty1000 = 0;
  uint32_t arr = 999; // 고정값 사용
  uint32_t ccr = ((uint32_t)duty1000 * arr + (PWM_DUTY_STEPS / 2)) / PWM_DUTY_STEPS;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ccr);
  uartPrintf(_DEF_CH1, "Duty Set: %d, CCR: %d, ARR: %d\r\n", duty1000, ccr, arr);
}
