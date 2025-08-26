/*
 * pid.c
 *
 *  Created on: Aug 16, 2025
 *      Author: sksgk
 */


#include <Heating Module Hed/pid.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static inline float clampf(float v, float lo, float hi)
{
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static inline float lpf_alpha_from_cutoff(float cutoff_hz, float sample_hz)
{
  // 1차 LPF: y += alpha*(x - y), alpha = dt / (tau + dt), tau = 1/(2*pi*fc)
  if (cutoff_hz <= 0.0f || sample_hz <= 0.0f) return 1.0f; // 필터 끔(즉시 추종)
  const float dt  = 1.0f / sample_hz;
  const float tau = 1.0f / (2.0f * (float)M_PI * cutoff_hz);
  return dt / (tau + dt);
}

void PID_Init(PID_t* pid, float kp, float ki, float kd, float hz)
{
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;
  pid->hz = (hz > 0.0f) ? hz : 10.0f;

  pid->out_min = -1000.0f;
  pid->out_max =  1000.0f;

  pid->deadband    = 0.0f;
  pid->d_cutoff_hz = 0.0f;     // 기본: D 필터 끔
  pid->slew_rate   = 0.0f;     // 기본: 제한 없음

  PID_Reset(pid);
}

void PID_Reset(PID_t* pid)
{
  pid->integ       = 0.0f;
  pid->d_state     = 0.0f;
  pid->prev_meas   = 0.0f;
  pid->prev_out    = 0.0f;
  pid->initialized = false;
}

void PID_SetOutputLimits(PID_t* pid, float out_min, float out_max)
{
  if (out_max < out_min) { float t = out_min; out_min = out_max; out_max = t; }
  pid->out_min = out_min;
  pid->out_max = out_max;
}

void PID_SetDeadband(PID_t* pid, float deadband)
{
  pid->deadband = (deadband >= 0.0f) ? deadband : 0.0f;
}

void PID_SetDerivativeCutoff(PID_t* pid, float cutoff_hz)
{
  pid->d_cutoff_hz = (cutoff_hz >= 0.0f) ? cutoff_hz : 0.0f;
}

void PID_SetSlewRate(PID_t* pid, float slew_per_sec)
{
  pid->slew_rate = (slew_per_sec >= 0.0f) ? slew_per_sec : 0.0f;
}

void PID_SetGains(PID_t* pid, float kp, float ki, float kd)
{
  pid->Kp = kp; pid->Ki = ki; pid->Kd = kd;
}

float PID_Step(PID_t* pid, float setpoint, float measurement)
{
  const float fs   = pid->hz;
  const float dt   = 1.0f / fs;

  // 첫 스텝 초기화
  if (!pid->initialized) {
    pid->prev_meas   = measurement;
    pid->d_state     = 0.0f;
    pid->prev_out    = 0.0f;
    pid->initialized = true;
  }

  // 오차 계산 + 데드밴드
  float e = setpoint - measurement;
  if (pid->deadband > 0.0f) {
    if (fabsf(e) < pid->deadband) e = 0.0f;
  }

  // P
  const float P = pid->Kp * e;

  // I (조건부 적분: 포화 방향으로 더 밀어붙이는 경우 적분 정지)
  float I_old = pid->integ;
  pid->integ += (pid->Ki * e) * dt;

  // D — measurement 기반(노이즈에 강함): d/dt(meas) -> 부호 반전
  float d_meas = (measurement - pid->prev_meas) * fs; // [units/sec]
  float D_raw  = -pid->Kd * d_meas;

  // D LPF
  const float alpha = lpf_alpha_from_cutoff(pid->d_cutoff_hz, fs);
  pid->d_state += alpha * (D_raw - pid->d_state);
  const float D = pid->d_state;

  // 미포화 출력
  float u = P + pid->integ + D;

  // 포화 + 앤티윈드업(조건부 적분: 포화되고, e가 같은 방향이면 I 롤백)
  float u_sat = clampf(u, pid->out_min, pid->out_max);
  if (u != u_sat) {
    // 포화 상태에서 에러가 같은 방향(더 포화시키는 방향)이면 적분 롤백
    if ((u > pid->out_max && e > 0.0f) ||
        (u < pid->out_min && e < 0.0f)) {
      pid->integ = I_old;
      u_sat = clampf(P + pid->integ + D, pid->out_min, pid->out_max);
    }
  }

  // 슬루레이트 제한(옵션)
  if (pid->slew_rate > 0.0f) {
    float max_step = pid->slew_rate * dt;        // 한 스텝에 허용되는 변화량
    float delta    = u_sat - pid->prev_out;
    if      (delta >  max_step) u_sat = pid->prev_out + max_step;
    else if (delta < -max_step) u_sat = pid->prev_out - max_step;
  }

  // 상태 업데이트
  pid->prev_out  = u_sat;
  pid->prev_meas = measurement;

  return u_sat; // -1000 .. +1000 (권장 범위)
}
