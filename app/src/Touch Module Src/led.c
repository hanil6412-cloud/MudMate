/*
 * led.c
 *
 *  Created on: Aug 14, 2025
 *      Author: sksgk
 */



#include "led.h"
#include "main.h"


bool ledInit(void)
{
	return true;
}
void ledOn	(uint8_t ch)
{
	switch(ch)
	{
	case _DEF_CH1:
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	break;
	}
}

void ledOff	(uint8_t ch)
{
	switch(ch)
	{
	case _DEF_CH1:
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	break;
	}
}
void ledToggle	(uint8_t ch)
{
	switch(ch)
	{
	case _DEF_CH1:
	HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	break;
	}
}
