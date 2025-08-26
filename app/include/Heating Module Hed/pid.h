/*
 * pid.h
 *
 *  Created on: Aug 16, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_PID_H_
#define INCLUDE_PID_H_

#include "def.h"

// PID 출력 범위 기본: -1000 ~ +1000  (Sign-Magnitude: 부호=DIR, 크기=PWM 듀티)
typedef struct {
  // Gains
  float Kp;
  float Ki;
  float Kd;

  // Timing
  float hz;            // 제어 루프 주파수(Hz), 예: 10.0f -> 100ms 주기

  // Limits
  float out_min;       // 최소 출력 (기본 -1000)
  float out_max;       // 최대 출력 (기본 +1000)

  // Behavior
  float deadband;      // |e| < deadband 면 오차=0 취급 (예: 0.5°C)
  float d_cutoff_hz;   // D항 LPF 컷오프(Hz), 0 -> 필터 끔
  float slew_rate;     // 출력 변화 제한(절대값, 단위: 출력/초). 0 -> 제한 없음

  // Internal states
  float integ;         // 적분 누적
  float d_state;       // D항 LPF 내부 상태
  float prev_meas;     // 이전 측정값 (D on measurement용)
  float prev_out;      // 직전 출력(슬루레이트 제한용)
  bool  initialized;   // 첫 스텝 초기화 여부
} PID_t;

/* 초기화/설정 */
void  PID_Init(PID_t* pid, float kp, float ki, float kd, float hz);
void  PID_Reset(PID_t* pid);
void  PID_SetOutputLimits(PID_t* pid, float out_min, float out_max);
void  PID_SetDeadband(PID_t* pid, float deadband);
void  PID_SetDerivativeCutoff(PID_t* pid, float cutoff_hz);   // 0: off
void  PID_SetSlewRate(PID_t* pid, float slew_per_sec);         // 0: off
void  PID_SetGains(PID_t* pid, float kp, float ki, float kd);

/* 한 스텝 계산: setpoint, measurement -> 출력(-1000..1000) */
float PID_Step(PID_t* pid, float setpoint, float measurement);

/* Sign-Magnitude 헬퍼 */
static inline bool PID_DirFromSign(float u)       { return (u >= 0.0f); }
static inline uint16_t PID_AbsDuty01000(float u)  {
  if (u < 0.0f) u = -u;
  if (u > 1000.0f) u = 1000.0f;
  return (uint16_t)(u + 0.5f);
}

#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_PID_H_ */
