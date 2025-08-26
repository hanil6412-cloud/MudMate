/*
 * touch.c
 *
 *  Created on: Aug 21, 2025
 *      Author: sksgk
 */

#ifndef SRC_TOUCH_C_
#define SRC_TOUCH_C_


#include "touch.h"
#include "uart.h"

/* ===== 내부 구조체 정의 ===== */

/**
 * @brief 터치 채널 정보를 저장하는 구조체
 * @note 각 터치 버튼(SW1~SW4)의 상태와 타이밍 정보 관리
 */
typedef struct {
    GPIO_TypeDef* port;             // GPIO 포트 (예: GPIOF, GPIOD)
    uint16_t pin;                   // GPIO 핀 번호 (예: GPIO_PIN_0)
    TouchState_t current_state;     // 현재 터치 상태
    TouchState_t previous_state;    // 이전 터치 상태 (상태 변화 감지용)
    uint32_t press_time;            // 터치가 시작된 시간 (HAL_GetTick() 값)
    bool long_press_detected;       // 롱프레스 이미 감지되었는지 플래그
} TouchChannel_t;

/**
 * @brief LED 채널 정보를 저장하는 구조체
 * @note 각 터치 버튼에 대응하는 LED 제어용
 */
typedef struct {
    GPIO_TypeDef* port;             // LED가 연결된 GPIO 포트
    uint16_t pin;                   // LED가 연결된 GPIO 핀 번호
} LEDChannel_t;

/* ===== 터치 채널 설정 배열 ===== */
/**
 * @note main.h에서 정의된 실제 핀에 맞춰 설정됨
 *       순서: SW1(PF1), SW2(PF0), SW3(PD1), SW4(PD0)
 */
static TouchChannel_t touch_channels[TOUCH_MAX_CH] = {
    {TOUCH_CH1_GPIO_Port, TOUCH_CH1_Pin, TOUCH_RELEASED, TOUCH_RELEASED, 0, false},  // SW1: PF1
    {TOUCH_CH2_GPIO_Port, TOUCH_CH2_Pin, TOUCH_RELEASED, TOUCH_RELEASED, 0, false},  // SW2: PF0
    {TOUCH_CH3_GPIO_Port, TOUCH_CH3_Pin, TOUCH_RELEASED, TOUCH_RELEASED, 0, false},  // SW3: PD1
    {TOUCH_CH4_GPIO_Port, TOUCH_CH4_Pin, TOUCH_RELEASED, TOUCH_RELEASED, 0, false}   // SW4: PD0
};

/* ===== LED 채널 설정 배열 ===== */
/**
 * @note main.h에서 정의된 실제 LED 핀에 맞춰 설정됨
 *       순서: LED1(PG0), LED2(PE1), LED3(PG9), LED4(PG12)
 */
static LEDChannel_t led_channels[TOUCH_MAX_CH] = {
    {TOUCH_CH1_LED_GPIO_Port, TOUCH_CH1_LED_Pin},  // LED1: PG0
    {TOUCH_CH2_LED_GPIO_Port, TOUCH_CH2_LED_Pin},  // LED2: PE1
    {TOUCH_CH3_LED_GPIO_Port, TOUCH_CH3_LED_Pin},  // LED3: PG9
    {TOUCH_CH4_LED_GPIO_Port, TOUCH_CH4_LED_Pin}   // LED4: PG12
};

/* ===== 상수 정의 ===== */
#define TOUCH_DEBOUNCE_TIME_MS      50      // 디바운싱 시간 (채터링 방지)
#define TOUCH_LONG_PRESS_TIME_MS    1000    // 롱프레스 판정 시간 (1초)

/* ===== 함수 구현 ===== */

/**
 * @brief 터치 모듈 초기화 함수
 * @return true 항상 성공 반환
 * @note 시스템 시작 시 한 번만 호출됨 (apInit에서)
 */
bool touchInit(void)
{
    // 모든 터치 채널 상태를 초기값으로 설정
    for(int i = 0; i < TOUCH_MAX_CH; i++) {
        touch_channels[i].current_state = TOUCH_RELEASED;      // 터치 안됨 상태
        touch_channels[i].previous_state = TOUCH_RELEASED;     // 이전 상태도 안됨
        touch_channels[i].press_time = 0;                      // 시간 초기화
        touch_channels[i].long_press_detected = false;         // 롱프레스 플래그 초기화
    }

    // 모든 LED를 꺼진 상태로 초기화
    touchSetAllLEDs(false);

    return true;
}

/**
 * @brief 터치 상태 업데이트 및 이벤트 처리 함수
 * @note 메인 루프에서 지속적으로 호출되어야 함 (apMain의 while(1) 안에서)
 *       각 터치 버튼의 상태 변화를 감지하고 적절한 콜백 함수 호출
 */
void touchUpdate(void)
{
    uint32_t current_time = HAL_GetTick();  // 현재 시스템 시간 (ms 단위)

    // 4개 터치 채널 모두 확인
    for(int i = 0; i < TOUCH_MAX_CH; i++) {
        // 이전 상태를 저장 (상태 변화 감지를 위해)
        touch_channels[i].previous_state = touch_channels[i].current_state;

        // 현재 터치 핀의 상태 읽기
        // LTK-04: 터치 안됨=HIGH, 터치됨=LOW
        GPIO_PinState pin_state = HAL_GPIO_ReadPin(touch_channels[i].port, touch_channels[i].pin);

        if(pin_state == GPIO_PIN_RESET) {  // 터치됨 (LOW 신호)
            if(touch_channels[i].current_state == TOUCH_RELEASED) {
                // 터치가 새로 시작됨 (RELEASED → PRESSED)
                touch_channels[i].current_state = TOUCH_PRESSED;
                touch_channels[i].press_time = current_time;           // 시작 시간 기록
                touch_channels[i].long_press_detected = false;        // 롱프레스 플래그 초기화

                // 터치 시작 이벤트 처리
                touchOnPress(i);        // 사용자 정의 콜백 함수 호출
                touchSetLED(i, true);   // 해당 LED 켜기
            } else {
                // 터치가 계속 지속됨 - 롱프레스 체크
                if(!touch_channels[i].long_press_detected &&
                   (current_time - touch_channels[i].press_time) >= TOUCH_LONG_PRESS_TIME_MS) {
                    // 1초 이상 눌렸고, 아직 롱프레스 이벤트가 발생하지 않았음
                    touch_channels[i].long_press_detected = true;      // 중복 방지 플래그
                    touchOnLongPress(i);    // 롱프레스 콜백 함수 호출
                }
            }
        } else {  // 터치 안됨 (HIGH 신호)
            if(touch_channels[i].current_state == TOUCH_PRESSED) {
                // 터치가 해제됨 (PRESSED → RELEASED)
                touch_channels[i].current_state = TOUCH_RELEASED;

                // 터치 해제 이벤트 처리
                touchOnRelease(i);      // 사용자 정의 콜백 함수 호출
                touchSetLED(i, false);  // 해당 LED 끄기
            }
        }
    }
}

/**
 * @brief 특정 채널의 현재 터치 상태 반환
 * @param ch 터치 채널 번호 (0~3)
 * @return 현재 터치 상태 (TOUCH_PRESSED 또는 TOUCH_RELEASED)
 */
TouchState_t touchGetState(uint8_t ch)
{
    // 채널 번호 유효성 검사
    if(ch >= TOUCH_MAX_CH) {
        return TOUCH_RELEASED;  // 잘못된 채널은 안전하게 RELEASED 반환
    }
    return touch_channels[ch].current_state;
}

/**
 * @brief 터치가 새로 눌렸는지 확인 (Rising Edge)
 * @param ch 터치 채널 번호 (0~3)
 * @return true: 방금 눌림, false: 눌리지 않음
 * @note 버튼을 누르는 순간만 true, 계속 누르고 있으면 false
 */
bool touchIsPressed(uint8_t ch)
{
    if(ch >= TOUCH_MAX_CH) return false;

    // 이전=RELEASED, 현재=PRESSED인 경우만 true
    return (touch_channels[ch].current_state == TOUCH_PRESSED && touch_channels[ch].previous_state == TOUCH_RELEASED);
}

/**
 * @brief 터치가 해제되었는지 확인 (Falling Edge)
 * @param ch 터치 채널 번호 (0~3)
 * @return true: 방금 해제됨, false: 해제되지 않음
 */
bool touchIsReleased(uint8_t ch)
{
    if(ch >= TOUCH_MAX_CH) return false;

    // 이전=PRESSED, 현재=RELEASED인 경우만 true
    return (touch_channels[ch].current_state == TOUCH_RELEASED &&
            touch_channels[ch].previous_state == TOUCH_PRESSED);
}

/**
 * @brief 롱프레스가 발생했는지 확인
 * @param ch 터치 채널 번호 (0~3)
 * @return true: 롱프레스 발생함, false: 발생하지 않음
 */
bool touchIsLongPressed(uint8_t ch)
{
    if(ch >= TOUCH_MAX_CH) return false;
    return touch_channels[ch].long_press_detected;
}

/**
 * @brief 특정 채널의 LED 제어
 * @param ch 터치 채널 번호 (0~3)
 * @param state true: LED 켜기, false: LED 끄기
 * @note LTK-04는 Cathode 컨트롤이므로 GPIO LOW일 때 LED 켜짐
 */
void touchSetLED(uint8_t ch, bool state)
{
    if(ch >= TOUCH_MAX_CH) return;  // 잘못된 채널 번호는 무시

    // LTK-04 LED는 Cathode 컨트롤 (LOW=켜짐, HIGH=꺼짐)
    if(state) {
        // LED 켜기 = GPIO LOW
        HAL_GPIO_WritePin(led_channels[ch].port, led_channels[ch].pin, GPIO_PIN_RESET);
    } else {
        // LED 끄기 = GPIO HIGH
        HAL_GPIO_WritePin(led_channels[ch].port, led_channels[ch].pin, GPIO_PIN_SET);
    }
}

/**
 * @brief 모든 터치 LED 일괄 제어
 * @param state true: 모든 LED 켜기, false: 모든 LED 끄기
 */
void touchSetAllLEDs(bool state)
{
    for(int i = 0; i < TOUCH_MAX_CH; i++) {
        touchSetLED(i, state);
    }
}

/* ===== 사용자 정의 콜백 함수 구현 ===== */
/**
 * @note 이 함수들은 터치 이벤트 발생 시 자동으로 호출됩니다.
 *       원하는 동작을 이 함수들 안에 구현하세요.
 */

/**
 * @brief 터치 눌림 이벤트 콜백 함수
 * @param ch 눌린 터치 채널 번호 (0~3)
 * @note 각 버튼별로 다른 기능을 구현할 수 있음
 */
void touchOnPress(uint8_t ch)
{
    // 기존 프로젝트의 채널 정의 활용
    switch(ch) {
        case _DEF_CH1:  // SW1 (채널 0)
            uartPrintf(_DEF_CH1, "Touch SW1 Pressed\r\n");
            // 여기에 SW1 눌림 시 실행할 코드 추가
            // 예: 메뉴 위로 이동, 온도 증가 등
            break;

        case _DEF_CH2:  // SW2 (채널 1)
            uartPrintf(_DEF_CH1, "Touch SW2 Pressed\r\n");
            // 여기에 SW2 눌림 시 실행할 코드 추가
            // 예: 메뉴 아래로 이동, 온도 감소 등
            break;

        case _DEF_CH3:  // SW3 (채널 2)
            uartPrintf(_DEF_CH1, "Touch SW3 Pressed\r\n");
            // 여기에 SW3 눌림 시 실행할 코드 추가
            // 예: 선택/확인, 모드 변경 등
            break;

        case _DEF_CH4:  // SW4 (채널 3)
            uartPrintf(_DEF_CH1, "Touch SW4 Pressed\r\n");
            // 여기에 SW4 눌림 시 실행할 코드 추가
            // 예: 취소/뒤로, 시스템 정지 등
            break;
    }
}

/**
 * @brief 터치 해제 이벤트 콜백 함수
 * @param ch 해제된 터치 채널 번호 (0~3)
 * @note 필요시 해제 시점에 특별한 동작 구현 가능
 */
void touchOnRelease(uint8_t ch)
{
    switch(ch) {
        case _DEF_CH1:
            uartPrintf(_DEF_CH1, "Touch SW1 Released\r\n");
            // SW1 해제 시 동작 (일반적으로 비워둠)
            break;
        case _DEF_CH2:
            uartPrintf(_DEF_CH1, "Touch SW2 Released\r\n");
            break;
        case _DEF_CH3:
            uartPrintf(_DEF_CH1, "Touch SW3 Released\r\n");
            break;
        case _DEF_CH4:
            uartPrintf(_DEF_CH1, "Touch SW4 Released\r\n");
            break;
    }
}

/**
 * @brief 롱프레스 이벤트 콜백 함수
 * @param ch 롱프레스된 터치 채널 번호 (0~3)
 * @note 특별한 기능(설정 모드, 공장 초기화 등)에 사용
 */
void touchOnLongPress(uint8_t ch)
{
    switch(ch) {
        case _DEF_CH1:
            uartPrintf(_DEF_CH1, "Touch SW1 Long Pressed\r\n");
            // SW1 롱프레스 시 LED 깜빡임 예제
            for(int i = 0; i < 3; i++) {
                touchSetLED(ch, false);     // LED 끄기
                delay(100);                 // 100ms 대기
                touchSetLED(ch, true);      // LED 켜기
                delay(100);                 // 100ms 대기
            }
            // 여기에 SW1 롱프레스 기능 추가 (예: 설정 모드 진입)
            break;

        case _DEF_CH2:
            uartPrintf(_DEF_CH1, "Touch SW2 Long Pressed\r\n");
            // SW2 롱프레스 기능 (예: 자동 모드 토글)
            break;

        case _DEF_CH3:
            uartPrintf(_DEF_CH1, "Touch SW3 Long Pressed\r\n");
            // SW3 롱프레스 기능 (예: 캘리브레이션)
            break;

        case _DEF_CH4:
            uartPrintf(_DEF_CH1, "Touch SW4 Long Pressed\r\n");
            // SW4 롱프레스 기능 (예: 시스템 리셋)
            break;
    }
}

#endif /* SRC_TOUCH_C_ */
