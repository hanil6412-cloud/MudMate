/*
 * pwm.c
 *
 *  Created on: Aug 17, 2025
 *      Author: sksgk
 */

#include <Heating Module Hed/pwm.h>

extern TIM_HandleTypeDef htim1;

bool pwmInit(void)
{
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
  return (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) == HAL_OK);
}

void pwmSetDuty(uint16_t duty1000)
{
  if (duty1000 > (PWM_DUTY_STEPS-1)) duty1000 = (PWM_DUTY_STEPS-1);

  // 아주 작은 듀티는 0%로 강제(스위칭 낭비/발열 방지)
  uint16_t min_cut = (uint16_t)(PWM_DUTY_STEPS * PWM_DUTY_MIN_FRAC + 0.5f);
  if (duty1000 < min_cut) duty1000 = 0;

  uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim1);
  uint32_t ccr = ( (uint32_t)duty1000 * arr + (PWM_DUTY_STEPS/2) ) / PWM_DUTY_STEPS;
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
}

void pwmStop(void)
{
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void peltier_set_dir(bool heat)
{
  HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, heat ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

