/*
 * pwm.h
 *
 *  Created on: Aug 17, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

#include "def.h"



extern TIM_HandleTypeDef htim1;


bool pwmInit(void);
void pwmStop(void);
void peltier_set_dir(bool heat);
void pwmSetDuty(uint16_t duty1000);

#endif /* INCLUDE_PWM_H_ */
