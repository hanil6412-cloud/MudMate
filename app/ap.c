/*
 * ap.c
 *
 *  Created on: Aug 14, 2025
 *      Author: sksgk
 */

#include "ap.h"

void apInit(void)
{
	uartInit();
	fanInit();
	fan_rpm_Init();
	fan_Start();
	lv_init();
	lv_port_disp_init();
	lv_example_anim_2();
}

void apMain(void)
{

	lv_timer_handler();
	delay(5);
}
