/*
 * temp_sensor.c - MAX31855 Driver
 *
 *  Created on: Aug 25, 2025
 *      Author: sksgk
 */

#include <Heating Module Hed/temp_sensor.h>
#include "uart.h"

#define Temp_buf_max 128

uint16_t Temp_buf[Temp_buf_max];
uint8_t Temp_index = 0;

// MAX31855 오류 비트 정의
#define MAX31855_FAULT_OPEN         0x01  // 열전대 개방
#define MAX31855_FAULT_SHORT_GND    0x02  // GND 단락
#define MAX31855_FAULT_SHORT_VCC    0x04  // VCC 단락

static MAX31855_Data_t last_reading;

bool Temp_sensor_Init(void)
{
    if(HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        uartPrintf(_DEF_CH1, "MAX31855: SPI Init Failed\r\n");
        return false;
    }

    // 초기 상태 초기화
    last_reading.thermocouple_temp = 0.0f;
    last_reading.internal_temp = 0.0f;
    last_reading.fault_flags = 0;
    last_reading.valid = false;

    uartPrintf(_DEF_CH1, "MAX31855: Init Complete\r\n");
    return true;
}

/**
 * @brief MAX31855에서 32비트 데이터 읽기 및 파싱
 * @param ch 채널 번호
 * @return 온도값 (x100, 예: 2550 = 25.50°C), 오류 시 0xFFFFFFFF
 */
uint32_t Temp_READ(uint8_t ch)
{
    uint8_t rx_data[4] = {0, 0, 0, 0};
    uint32_t raw_data = 0;

    // CS 핀을 HIGH로 설정하여 변환 시작 대기
    HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_SET);
    HAL_Delay(250);  // 변환 시간 대기 (220ms + 여유)

    // CS 핀을 LOW로 설정하여 데이터 읽기 시작
    HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);    // 안정화 시간

    // 32비트 데이터 읽기
    switch(ch)
    {
        case _DEF_CH1:
            if (HAL_SPI_Receive(&hspi1, rx_data, 4, 100) != HAL_OK) {
                HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_SET);
                uartPrintf(_DEF_CH1, "MAX31855: SPI Read Error\r\n");
                return 0xFFFFFFFF;
            }
            break;
        default:
            HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_SET);
            return 0xFFFFFFFF;
    }

    // CS 핀을 다시 HIGH로 설정
    HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_SET);

    // 32비트 데이터 조합 (MSB first)
    raw_data = ((uint32_t)rx_data[0] << 24) |
               ((uint32_t)rx_data[1] << 16) |
               ((uint32_t)rx_data[2] << 8)  |
               ((uint32_t)rx_data[3]);

    // 데이터 파싱 및 검증
    if (MAX31855_ParseData(raw_data, &last_reading)) {
        // 정상 데이터인 경우 온도값 반환 (x100)
        return (uint32_t)(last_reading.thermocouple_temp * 100.0f + 0.5f);
    } else {
        // 오류 발생 시 에러 메시지 출력
        uartPrintf(_DEF_CH1, "MAX31855: Fault detected - 0x%02X\r\n", last_reading.fault_flags);
        return 0xFFFFFFFF;
    }
}

/**
 * @brief MAX31855 원시 데이터 파싱
 * @param raw_data 32비트 원시 데이터
 * @param parsed_data 파싱된 결과를 저장할 구조체 포인터
 * @return true: 성공, false: 오류 발생
 */
bool MAX31855_ParseData(uint32_t raw_data, MAX31855_Data_t* parsed_data)
{

		if (raw_data == 0xFFFFFFFF || raw_data == 0x00000000)
		{
		        parsed_data->valid = false;
		        parsed_data->fault_flags = 0x07;
		        return false;
		}
	    // 유효하지 않은 데이터 체크
	    if (raw_data == 0xFFFFFFFF || raw_data == 0x00000000)
	    {
	        parsed_data->valid = false;
	        parsed_data->fault_flags = 0x07;
	        return false;
	    }

    // 비트 16 (Fault bit) 확인
    if (raw_data & 0x00010000)
    {
        // 오류 발생 - 하위 3비트에서 오류 타입 확인
        parsed_data->fault_flags = raw_data & 0x07;
        parsed_data->valid = false;
        return false;
    }

    // 열전대 온도 추출 (비트 31-18, 14비트, 부호 포함)
    int16_t thermocouple_raw = (int16_t)((raw_data >> 18) & 0x3FFF);

    // 부호 확장 (14비트 → 16비트)
    if (thermocouple_raw & 0x2000) {
        thermocouple_raw |= 0xC000;  // 음수인 경우 부호 확장
    }

    // 0.25°C 단위로 변환
    parsed_data->thermocouple_temp = (float)thermocouple_raw * 0.25f;

    // 내부 온도 추출 (비트 15-4, 12비트, 부호 포함)
    int16_t internal_raw = (int16_t)((raw_data >> 4) & 0x0FFF);

    // 부호 확장 (12비트 → 16비트)
    if (internal_raw & 0x0800) {
        internal_raw |= 0xF000;  // 음수인 경우 부호 확장
    }

    // 0.0625°C 단위로 변환
    parsed_data->internal_temp = (float)internal_raw * 0.0625f;

    parsed_data->fault_flags = 0;
    parsed_data->valid = true;
    return true;
}

/**
 * @brief 현재 온도 정보 가져오기
 * @return 마지막 읽은 온도 데이터 구조체 포인터
 */
const MAX31855_Data_t* MAX31855_GetLastReading(void)
{
    return &last_reading;
}

/**
 * @brief 온도 센서 상태 확인
 * @return true: 정상, false: 오류
 */
bool MAX31855_IsHealthy(void)
{
    return last_reading.valid && (last_reading.fault_flags == 0);
}

/**
 * @brief 오류 상태를 문자열로 변환
 * @param fault_flags 오류 플래그
 * @return 오류 설명 문자열
 */
const char* MAX31855_GetFaultString(uint8_t fault_flags)
{
    if (fault_flags & MAX31855_FAULT_OPEN) {
        return "Thermocouple Open Circuit";
    }
    if (fault_flags & MAX31855_FAULT_SHORT_GND) {
        return "Short to Ground";
    }
    if (fault_flags & MAX31855_FAULT_SHORT_VCC) {
        return "Short to VCC";
    }
    return "No Fault";
}

/**
 * @brief 상세 온도 정보를 UART로 출력
 */
void MAX31855_PrintStatus(void)
{
    const MAX31855_Data_t* data = MAX31855_GetLastReading();

    if (data->valid) {
        uartPrintf(_DEF_CH1, "=== MAX31855 Status ===\r\n");
        uartPrintf(_DEF_CH1, "Thermocouple: %.2f C\r\n", data->thermocouple_temp);
        uartPrintf(_DEF_CH1, "Internal Temp: %.2f C\r\n", data->internal_temp);
        uartPrintf(_DEF_CH1, "Status: %s\r\n", MAX31855_GetFaultString(data->fault_flags));
    } else {
        uartPrintf(_DEF_CH1, "MAX31855: Invalid Data - %s\r\n",
                  MAX31855_GetFaultString(data->fault_flags));
    }
}

void Temp_buf_push(uint16_t temp_value)
{
    Temp_buf[Temp_index++] = temp_value;
    if(Temp_index >= Temp_buf_max)
    {
        Temp_index = 0;
    }
}

/**
 * @brief 이동평균 계산 (최근 N개 샘플)
 * @param samples 평균할 샘플 수 (최대 Temp_buf_max)
 * @return 평균 온도값 (x100)
 */
uint32_t Temp_GetMovingAverage(uint8_t samples)
{
    if (samples > Temp_buf_max) samples = Temp_buf_max;
    if (samples == 0) return 0;

    uint32_t sum = 0;
    uint8_t start_index = (Temp_index >= samples) ? (Temp_index - samples) : 0;

    for (uint8_t i = 0; i < samples; i++) {
        uint8_t idx = (start_index + i) % Temp_buf_max;
        sum += Temp_buf[idx];
    }

    return sum / samples;
}

void MAX31855_RawDataTest(void)
{
    uint8_t rx_data[4] = {0, 0, 0, 0};

    uartPrintf(_DEF_CH1, "=== MAX31855 Raw Data Test ===\r\n");

    for(int test = 0; test < 5; test++) {
        // CS 활성화
        HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_RESET);
        HAL_Delay(1);

        // 4바이트 개별 읽기
        for(int i = 0; i < 4; i++) {
            HAL_SPI_Receive(&hspi1, &rx_data[i], 1, 100);
        }

        // CS 비활성화
        HAL_GPIO_WritePin(SPI1_OUTPUT_GPIO_Port, SPI1_OUTPUT_Pin, GPIO_PIN_SET);

        // 원시 데이터 출력
        uint32_t raw = ((uint32_t)rx_data[0] << 24) |
                      ((uint32_t)rx_data[1] << 16) |
                      ((uint32_t)rx_data[2] << 8)  |
                      ((uint32_t)rx_data[3]);

        uartPrintf(_DEF_CH1, "Test %d: %02X %02X %02X %02X = 0x%08lX\r\n",
                  test+1, rx_data[0], rx_data[1], rx_data[2], rx_data[3], raw);

        HAL_Delay(500);
    }
}
