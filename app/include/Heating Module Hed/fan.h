/*
 * fan.h
 *
 *  Created on: Aug 26, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_FAN_H_
#define INCLUDE_FAN_H_



#include "def.h"


extern TIM_HandleTypeDef htim3;


bool fanInit(void);
void fanStop(void);
void fanSetDuty(uint16_t duty1000);



#endif /* INCLUDE_FAN_H_ */
