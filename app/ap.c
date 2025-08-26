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
}
static void setDutyAndReset(uint16_t duty)
{
	fanSetDuty(duty);   // 0~999 스케일 가정
	TACH_Reset();       // 듀티 급변/정지 후 반드시 리셋
}

void apMain(void)
{
  uint16_t duty[] = {0, 250, 500, 750, 999};
  uint8_t i = 0;

  while (1)
  {
    setDutyAndReset(duty[i]);
    HAL_Delay(300); // 워밍업

    uint32_t rpm100 = fan_GetRPMx100();
    uartPrintf(_DEF_CH1, "Duty:%u%%  RPM:%lu.%02lu\r\n",
               (duty[i]*100)/999, rpm100/100, rpm100%100);

    HAL_Delay(5000);
    if (++i >= sizeof(duty)/sizeof(duty[0])) i = 0;
  }
}
