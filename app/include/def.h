/*
 * def.h
 *
 *  Created on: Aug 14, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_DEF_H_
#define INCLUDE_DEF_H_

#include "stm32f7xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ===== 채널 정의 ===== */
#define _DEF_CH1                     0
#define _DEF_CH2                     1
#define _DEF_CH3                     2
#define _DEF_CH4                     3
#define _DEF_CH5                     4

/* ===== 온도 센서 모듈 (SPI) ===== */
#define SPI1_OUTPUT_Pin              GPIO_PIN_4
#define SPI1_OUTPUT_GPIO_Port        GPIOA

// 온도 센서 설정
#define TEMP_BUF_MAX                 (16u)      // 이동평균 창 크기
#define TEMP_LSB_C                   (0.25f)    // raw 1 LSB → 0.25°C (센서 스펙)
#define TEMP_RANGE_MIN_C             (0.0f)     // 안전 범위(선택)
#define TEMP_RANGE_MAX_C             (100.0f)

/* ===== PWM 제어 모듈 ===== */
// PWM 방향 제어 핀
#define DIR_GPIO_Port                GPIOE
#define DIR_Pin                      GPIO_PIN_8

// PWM 설정
#define PWM_DUTY_STEPS               (1000u)    // 0..999 스케일
#define PWM_DUTY_MIN_FRAC            (0.02f)    // 2% 미만은 0%로 강제(채터 방지)

// PWM 단독 테스트용
#define TEST_PWM_DUTY                300        // 0~999 (처음엔 30% 이하 권장)
#define TEST_DIR_SECS                3          // 방향 유지 시간 (초)

/* ===== 터치 모듈 (LTK-04) ===== */
// 터치 입력 핀 (main.h 기반)
#define TOUCH_CH1_GPIO_Port          GPIOF      // TOUCH_1CH
#define TOUCH_CH1_Pin                GPIO_PIN_1
#define TOUCH_CH2_GPIO_Port          GPIOF      // TOUCH_CH2
#define TOUCH_CH2_Pin                GPIO_PIN_0
#define TOUCH_CH3_GPIO_Port          GPIOD      // TOUCH_CH3
#define TOUCH_CH3_Pin                GPIO_PIN_1
#define TOUCH_CH4_GPIO_Port          GPIOD      // TOUCH_CH4
#define TOUCH_CH4_Pin                GPIO_PIN_0

// 터치 LED 출력 핀 (main.h 기반)
#define TOUCH_CH1_LED_GPIO_Port      GPIOG      // TOUCH_CH1_LED
#define TOUCH_CH1_LED_Pin            GPIO_PIN_0
#define TOUCH_CH2_LED_GPIO_Port      GPIOE      // TOUCH_CH2_LED
#define TOUCH_CH2_LED_Pin            GPIO_PIN_1
#define TOUCH_CH3_LED_GPIO_Port      GPIOG      // TOUCH_CH3_LED
#define TOUCH_CH3_LED_Pin            GPIO_PIN_9
#define TOUCH_CH4_LED_GPIO_Port      GPIOG      // TOUCH_CH4_LED
#define TOUCH_CH4_LED_Pin            GPIO_PIN_12

// 터치 모듈 설정
#define TOUCH_MAX_CH                 4
#define TOUCH_DEBOUNCE_TIME_MS       50         // 디바운싱 시간
#define TOUCH_LONG_PRESS_TIME_MS     1000       // 롱프레스 판정 시간

/* ===== PID 제어 모듈 ===== */
// 제어 루프 설정
#define CTRL_LOOP_HZ                 (10.0f)    // 제어 루프 주파수(Hz)
#define TSET_DEFAULT_C               (25.0f)    // 기본 목표 온도

// PID 파라미터 (출력 스케일: -1000..+1000)
#define PID_KP                       (60.0f)    // 비례 게인
#define PID_KI                       (3.0f)     // 적분 게인
#define PID_KD                       (0.0f)     // 미분 게인
#define PID_OUT_MIN                  (-1000.0f) // 최소 출력
#define PID_OUT_MAX                  ( 1000.0f) // 최대 출력
#define PID_DEADBAND_C               (0.5f)     // ±0.5°C 안은 0으로 취급
#define PID_D_CUTOFF_HZ              (2.0f)     // D항 LPF 컷오프(0이면 끔)
#define PID_SLEW_PER_SEC             (0.0f)     // 출력 변화 제한(0이면 없음)

/* ===== 보드 LED 모듈 ===== */
#define LED1_GPIO_Port               LD1_GPIO_Port    // Green LED
#define LED1_Pin                     LD1_Pin
#define LED2_GPIO_Port               LD2_GPIO_Port    // Blue LED
#define LED2_Pin                     LD2_Pin
#define LED3_GPIO_Port               LD3_GPIO_Port    // Red LED
#define LED3_Pin                     LD3_Pin

/* ===== LCD 모듈 (GC9A01A) ===== */
// SPI2 제어 핀 (CubeMX 기반)
#define LCD_CS_GPIO_Port             GPIOD
#define LCD_CS_Pin                   GPIO_PIN_12
#define LCD_DC_GPIO_Port             GPIOD      // Data/Command 선택
#define LCD_DC_Pin                   GPIO_PIN_11
#define LCD_RES_GPIO_Port            GPIOD      // Reset
#define LCD_RES_Pin                  GPIO_PIN_13

// LCD 설정
#define LCD_WIDTH                    240
#define LCD_HEIGHT                   240
#define LCD_SPI_TIMEOUT              100        // SPI 타임아웃 (ms)

// LCD 제어 매크로
#define LCD_RES_Clr()   HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET)
#define LCD_RES_Set()   HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)
#define LCD_DC_Clr()    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET)
#define LCD_DC_Set()    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET)
#define LCD_CS_Clr()    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_Set()    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)

/* ===== 시스템 설정 ===== */
// bring-up stage: 0=센서만, 1=PWM만, 2=PID(정식), 3=터치+PID
#define TEST_STAGE                   0

/* ===== 함수 선언 ===== */
void delay(uint32_t time_ms);
uint32_t millis(void);

#endif /* INCLUDE_DEF_H_ */
