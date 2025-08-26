/*
 * control.c
 *
 *  Created on: Aug 17, 2025
 *      Author: sksgk
 */


#include <Heating Module Hed/pid.h>
#include <Heating Module Hed/pid_control.h>
#include <Heating Module Hed/pwm.h>


static PID_t pid;
static float g_Tset   = TSET_DEFAULT_C;
static float g_last_u = 0.0f;
static bool  g_last_dir = false;

void controlInit(void)
{
  // PID 기본 세팅
  PID_Init(&pid, PID_KP, PID_KI, PID_KD, CTRL_LOOP_HZ);
  PID_SetOutputLimits(&pid, PID_OUT_MIN, PID_OUT_MAX);
  PID_SetDeadband(&pid, PID_DEADBAND_C);
  PID_SetDerivativeCutoff(&pid, PID_D_CUTOFF_HZ);
  PID_SetSlewRate(&pid, PID_SLEW_PER_SEC);

  pwmInit();
  peltier_set_dir(false);
  pwmSetDuty(0);
}

void controlSetTarget(float T_set)           { g_Tset = T_set; }
void controlSetTunings(float kp, float ki, float kd) { PID_SetGains(&pid, kp, ki, kd); }
float controlGetLastU(void)                  { return g_last_u; }

void controlStep(float T_meas)
{
  // (선택) 센서 안전 범위 체크
  if (T_meas < TEMP_RANGE_MIN_C || T_meas > TEMP_RANGE_MAX_C) {
    pwmSetDuty(0);
    return;
  }

  float u = PID_Step(&pid, g_Tset, T_meas);      // -1000..+1000
  bool dir = (u >= 0.0f);
  uint16_t duty = (u >= 0.0f) ? (uint16_t)(u + 0.5f) : (uint16_t)((-u) + 0.5f);
  if (duty > (PWM_DUTY_STEPS-1)) duty = (PWM_DUTY_STEPS-1);

  // 방향 전환 시 보호: 듀티 0 → 방향 갱신 (필요 시 수 ms 지연)
  if (dir != g_last_dir) {
    pwmSetDuty(0);
    // HAL_Delay(2);
    peltier_set_dir(dir);
    g_last_dir = dir;
  } else {
    peltier_set_dir(dir);
  }

  g_last_u = u;
  pwmSetDuty(duty);
}
