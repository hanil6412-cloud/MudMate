/*
 * uart.c
 *
 *  Created on: Aug 18, 2025
 *      Author: sksgk
 */



#include "uart.h"


#define UART_Q_BUF_MAX			256

extern UART_HandleTypeDef huart3;

static uint16_t q_in = 0;
static uint16_t q_out = 0;
static uint8_t q_buf[UART_Q_BUF_MAX];
//static uint8_t q_data; [인터럽트 시 사용]


bool uartInit()
{
//	HAL_UART_Receive_IT(&huart3, &q_data, 1);		// size를 1로 해야 1byte마다 한번씩 인터럽트가 걸림.
	HAL_UART_Receive_DMA(&huart3, &q_buf[0], UART_Q_BUF_MAX);
	return true;
}

// 2. uart 송출용 코드
uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;
	HAL_StatusTypeDef hal_ret;
	switch(ch)
	{
		case _DEF_CH1:
		hal_ret = HAL_UART_Transmit(&huart3, p_data, length, 100);
		if (hal_ret == HAL_OK)
		{
			ret = length;
		}
		break;
	}
	return ret;
}

// 3. 버퍼에 몇개의 데이터가 있는지를 확인
uint32_t uartAvaliable(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
	{
	case _DEF_CH1:
//		ret = (UART_Q_BUF_MAX + q_in -q_out) % UART_Q_BUF_MAX; 인터럽트 모드 시 사용
		q_in =  (UART_Q_BUF_MAX - huart3.hdmarx->Instance->NDTR) % UART_Q_BUF_MAX;
		break;
	}
	return ret;
}

uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;

	switch(ch)
	{
		case _DEF_CH1:
			if (q_out != q_in)
			{
				ret = q_buf[q_out];
				q_out = (q_out+1) % UART_Q_BUF_MAX;
			}
			break;
	}
	return ret;
}


uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
	uint32_t ret =0;
	va_list arg;
	char print_buf[256];

	va_start(arg, fmt);

	int len;
	len = vsnprintf(print_buf, 256, fmt, arg);	// 버퍼 이름, 갯수, 포맷 지정자, 값
	va_end(arg);

	if (len > 0)	// 데이터가 있다면~
	{
		ret =  uartWrite(ch,(uint8_t*)print_buf, len);
	}
	return ret;
}

////인터럽트 모드의 Receive에서 수신을 입력받으면 RxCpltCallback이 걸림 [인터럽트 모드일때 사용]
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(&huart3 == huart)
//	{
//		uint16_t q_in_next;
//
//		// q_in을 buf의 크기에 맞춰 q_in_next로 주소를 옮김.
//		q_in_next = (q_in + 1) % UART_Q_BUF_MAX;
//		if(q_in_next != q_out)	// q_out보다 q_in이 더 들어왔다면
//		{
//			q_buf[q_in] = q_data;
//			q_in = q_in_next;
//		}
//		HAL_UART_Receive_IT(&huart3, &q_data, 1);	// q_data 주소의 값을 역참조하여 데이터 수신
//	}
//}
