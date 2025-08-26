/*
 * ap.h
 *
 *  Created on: Aug 14, 2025
 *      Author: sksgk
 */

#ifndef AP_H_
#define AP_H_

#include <Heating Module Hed/fan.h>
#include <Heating Module Hed/fan_speed.h>
#include <Heating Module Hed/pid.h>
#include <Heating Module Hed/pid_control.h>
#include <Heating Module Hed/pwm.h>
#include <Heating Module Hed/temp_sensor.h>
#include "def.h"
#include "led.h"
#include "utils.h"
#include "uart.h"
#include "touch.h"

void apInit();
void apMain();



#endif /* AP_H_ */
