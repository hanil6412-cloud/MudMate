/*
 * temp_sensor.h - MAX31855 Driver Header
 *
 *  Created on: Aug 25, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_TEMP_SENSOR_H_
#define INCLUDE_TEMP_SENSOR_H_

#include "def.h"

extern SPI_HandleTypeDef hspi1;

// MAX31855 데이터 구조체 정의
typedef struct {
    float thermocouple_temp;    // 열전대 온도 (°C)
    float internal_temp;        // 내부 온도 (°C) - cold-junction
    uint8_t fault_flags;        // 오류 플래그
    bool valid;                 // 데이터 유효성
} MAX31855_Data_t;

/* ===== 기본 함수 ===== */
bool Temp_sensor_Init(void);
uint32_t Temp_READ(uint8_t ch);
void Temp_buf_push(uint16_t value);

/* ===== MAX31855 전용 함수 ===== */
bool MAX31855_ParseData(uint32_t raw_data, MAX31855_Data_t* parsed_data);
const MAX31855_Data_t* MAX31855_GetLastReading(void);
bool MAX31855_IsHealthy(void);
const char* MAX31855_GetFaultString(uint8_t fault_flags);
void MAX31855_PrintStatus(void);

/* ===== 고급 기능 ===== */
uint32_t Temp_GetMovingAverage(uint8_t samples);

/* ===== 온도 범위 및 상수 정의 ===== */
#define MAX31855_MIN_TEMP_C         (-270.0f)   // 최소 측정 온도
#define MAX31855_MAX_TEMP_C         (1800.0f)   // 최대 측정 온도
#define MAX31855_TEMP_RESOLUTION    (0.25f)     // 온도 분해능 (°C)
#define MAX31855_INTERNAL_RESOLUTION (0.0625f)  // 내부 온도 분해능 (°C)

/* ===== 오류 플래그 정의 ===== */
#define MAX31855_FAULT_OPEN         0x01        // 열전대 개방
#define MAX31855_FAULT_SHORT_GND    0x02        // GND 단락
#define MAX31855_FAULT_SHORT_VCC    0x04        // VCC 단락

/* ===== 매크로 함수 ===== */
#define MAX31855_TEMP_TO_INT100(temp)   ((int32_t)((temp) * 100.0f + 0.5f))  // 온도를 x100 정수로 변환
#define MAX31855_INT100_TO_TEMP(temp)   ((float)(temp) / 100.0f)             // x100 정수를 온도로 변환

void MAX31855_RawDataTest(void);

#endif /* INCLUDE_TEMP_SENSOR_H_ */
