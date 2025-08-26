/*
 * uart.h
 *
 *  Created on: Aug 18, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include "def.h"

#define UART_MAX_CH			1


bool uartInit();
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
uint32_t uartAvaliable(uint8_t ch);
uint8_t uartRead(uint8_t ch);
uint32_t uartPrintf(uint8_t ch, const char *fmt, ...);


#endif /* INCLUDE_UART_H_ */
