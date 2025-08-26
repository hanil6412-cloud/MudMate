/*
 * fan_speed.h
 *
 *  Created on: Aug 26, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_FAN_SPEED_H_
#define INCLUDE_FAN_SPEED_H_

#include "def.h"



void fan_rpm_Init(void);
void fan_Start(void);
uint32_t fan_GetRPMx100(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void TACH_Reset(void);


#endif /* INCLUDE_FAN_SPEED_H_ */
