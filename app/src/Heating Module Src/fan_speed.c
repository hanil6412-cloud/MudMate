/*
 * fan_speed.c
 *
 *  Created on: Aug 26, 2025
 *      Author: sksgk
 */

#include <Heating Module Hed/fan_speed.h>

extern TIM_HandleTypeDef htim9;

/* ===== 설정 상수 ===== */
// TIM9: APB2 타이머 216MHz, PSC=21599 → tick=10kHz(0.1ms), ARR=65535
#define TICK_HZ            10000u      // 10 kHz
#define TACH_PPR           2u          // 팬 펄스/회전 (필요시 4로 변경)
#define ZERO_TIMEOUT_MS     700u       // 이 시간 새 펄스 없으면 0으로 강제
#define MIN_DIFF_TICKS        5u       // <0.5ms 글리치 컷
#define MAX_DIFF_TICKS    65530u       // 이상치 컷
#define AVG_WINDOW             8u       // 이동평균 창
#define WARMUP_DISCARD         3u       // 재가동 초기 N펄스 무시

/* ===== 내부 상태 ===== */
static volatile uint16_t last_cc = 0;
static volatile uint8_t  primed = 0;
static volatile uint16_t diff_buf[AVG_WINDOW];
static volatile uint8_t  diff_idx = 0;
static volatile uint8_t  warmup_left = 0;

static volatile uint32_t rpm_x100 = 0;
static volatile uint32_t last_update_ms = 0;

/* ===== 유틸 ===== */
static inline uint32_t avg_diff_ticks(void)
{
  uint32_t sum = 0; uint8_t n = 0;
  for (uint8_t i=0; i<AVG_WINDOW; i++){
    uint16_t d = diff_buf[i];
    if (d){ sum += d; n++; }
  }
  return n ? (sum / n) : 0;
}

/* ===== API ===== */
void fan_rpm_Init(void)
{
  // Cube가 MX_TIM9_Init 생성했다면 여기서 호출해도 되고, 비워둬도 됩니다.
  // MX_TIM9_Init();
}

void fan_Start(void)
{
  TACH_Reset();
  HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_1);
}

void TACH_Reset(void)
{
  __disable_irq();
  primed = 0;
  last_cc = 0;
  diff_idx = 0;
  for (uint8_t i=0;i<AVG_WINDOW;i++) diff_buf[i]=0;
  warmup_left = WARMUP_DISCARD;
  rpm_x100 = 0;
  last_update_ms = 0;
  __enable_irq();
}

uint32_t fan_GetRPMx100(void)
{
  uint32_t now = HAL_GetTick();

  // 타임아웃 or 아직 측정 전이라면 무조건 0
  if (last_update_ms == 0 || (now - last_update_ms) > ZERO_TIMEOUT_MS)
  {
    rpm_x100 = 0;   // ★ 내부 상태도 0으로 덮기
    return 0;
  }

  return rpm_x100;
}

/* IRQ 핸들러는 stm32f7xx_it.c 에서:
   extern TIM_HandleTypeDef htim9;
   void TIM1_BRK_TIM9_IRQHandler(void){ HAL_TIM_IRQHandler(&htim9); }
*/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM9 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) return;

  uint16_t now = (uint16_t)HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

  if (!primed){ primed = 1; last_cc = now; return; }  // 첫 샘플 버림

  uint16_t diff = (now >= last_cc) ? (now - last_cc)
                                   : (uint16_t)(0x10000u - last_cc + now); // 16bit wrap
  last_cc = now;

  if (diff < MIN_DIFF_TICKS || diff > MAX_DIFF_TICKS) return; // 이상치 컷
  if (warmup_left){ warmup_left--; return; }                  // 초기 N펄스 무시

  diff_buf[diff_idx++] = diff;
  if (diff_idx >= AVG_WINDOW) diff_idx = 0;

  uint32_t d = avg_diff_ticks();          // tick(0.1ms) 단위
  if (d){
    // RPM*100 = (60*100*TICK_HZ) / (PPR * d)
    uint64_t num = (uint64_t)60u * 100u * (uint64_t)TICK_HZ;  // 6,000,000,000
    rpm_x100 = (uint32_t)(num / ((uint64_t)TACH_PPR * (uint64_t)d));
    last_update_ms = HAL_GetTick();
  }
}


