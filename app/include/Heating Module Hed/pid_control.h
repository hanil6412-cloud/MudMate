/*
 * control.h
 *
 *  Created on: Aug 17, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_CONTROL_H_
#define INCLUDE_CONTROL_H_


#include "def.h"

void  controlInit(void);            // define.H에서 전부 읽어 초기화
void  controlSetTarget(float T_set);
void  controlSetTunings(float Kp, float Ki, float Kd);
void  controlStep(float T_meas);
float controlGetLastU(void);



#endif /* INCLUDE_CONTROL_H_ */
