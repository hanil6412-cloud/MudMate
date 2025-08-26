/*
 * touch.h
 *
 *  Created on: Aug 21, 2025
 *      Author: sksgk
 */

#ifndef INCLUDE_TOUCH_H_
#define INCLUDE_TOUCH_H_

#include "def.h"

/* ===== 터치 모듈 상수 정의 ===== */
#define TOUCH_MAX_CH    4           // 터치 채널 개수 (SW1~SW4)

/**
 * @brief 터치 상태 열거형
 * @note LTK-04 모듈은 터치되지 않았을 때 HIGH, 터치될 때 LOW 신호 출력
 */
typedef enum {
    TOUCH_RELEASED = 0,             // 터치가 해제된 상태 (버튼이 눌리지 않음)
    TOUCH_PRESSED = 1               // 터치가 눌린 상태 (버튼이 눌림)
} TouchState_t;

/* ===== 터치 모듈 함수 선언 ===== */

/**
 * @brief 터치 모듈 초기화 함수
 * @return true: 성공, false: 실패
 * @note 터치 채널 상태 초기화 및 모든 LED 끄기
 *       apInit()에서 한 번만 호출하면 됨
 */
bool touchInit(void);

/**
 * @brief 터치 상태 업데이트 함수 (메인 루프에서 주기적 호출 필요)
 * @note 각 터치 버튼의 상태를 확인하고 이벤트 처리
 *       apMain()의 while(1) 루프 안에서 계속 호출해야 함
 *       - 터치 감지 시: touchOnPress() 콜백 실행
 *       - 터치 해제 시: touchOnRelease() 콜백 실행
 *       - 롱프레스 시: touchOnLongPress() 콜백 실행
 */
void touchUpdate(void);

/**
 * @brief 특정 채널의 현재 터치 상태 반환
 * @param ch 터치 채널 번호 (0~3: SW1~SW4)
 * @return TouchState_t 현재 터치 상태
 * @note 현재 버튼이 눌려있는지 확인할 때 사용
 */
TouchState_t touchGetState(uint8_t ch);

/**
 * @brief 터치가 새로 눌렸는지 확인 (Rising Edge 감지)
 * @param ch 터치 채널 번호 (0~3: SW1~SW4)
 * @return true: 방금 눌림, false: 눌리지 않음
 * @note 버튼을 누르는 순간만 true 반환 (한 번만 감지)
 *       메뉴 선택, 기능 실행 등에 사용
 */
bool touchIsPressed(uint8_t ch);

/**
 * @brief 터치가 해제되었는지 확인 (Falling Edge 감지)
 * @param ch 터치 채널 번호 (0~3: SW1~SW4)
 * @return true: 방금 해제됨, false: 해제되지 않음
 * @note 버튼을 놓는 순간만 true 반환 (한 번만 감지)
 */
bool touchIsReleased(uint8_t ch);

/**
 * @brief 롱프레스가 발생했는지 확인
 * @param ch 터치 채널 번호 (0~3: SW1~SW4)
 * @return true: 롱프레스 발생, false: 발생하지 않음
 * @note 1초 이상 눌렀을 때 true 반환 (한 번만 감지)
 *       설정 모드 진입, 리셋 기능 등에 사용
 */
bool touchIsLongPressed(uint8_t ch);

/**
 * @brief 특정 채널의 LED 제어
 * @param ch 터치 채널 번호 (0~3: SW1~SW4)
 * @param state true: LED 켜기, false: LED 끄기
 * @note LTK-04는 Cathode 컨트롤이므로 LOW일 때 LED가 켜짐
 */
void touchSetLED(uint8_t ch, bool state);

/**
 * @brief 모든 터치 LED 일괄 제어
 * @param state true: 모든 LED 켜기, false: 모든 LED 끄기
 * @note 시스템 시작/종료 시 LED 상태 초기화에 사용
 */
void touchSetAllLEDs(bool state);

/* ===== 사용자 정의 콜백 함수 (터치 이벤트 발생 시 자동 호출) ===== */

/**
 * @brief 터치 눌림 이벤트 콜백 함수
 * @param ch 눌린 터치 채널 번호 (0~3: SW1~SW4)
 * @note 터치가 눌리는 순간 자동으로 호출됨
 *       이 함수 안에 원하는 동작을 구현하면 됨
 */
void touchOnPress(uint8_t ch);

/**
 * @brief 터치 해제 이벤트 콜백 함수
 * @param ch 해제된 터치 채널 번호 (0~3: SW1~SW4)
 * @note 터치가 해제되는 순간 자동으로 호출됨
 */
void touchOnRelease(uint8_t ch);

/**
 * @brief 롱프레스 이벤트 콜백 함수
 * @param ch 롱프레스된 터치 채널 번호 (0~3: SW1~SW4)
 * @note 1초 이상 눌렀을 때 자동으로 호출됨
 */
void touchOnLongPress(uint8_t ch);

#endif /* INCLUDE_TOUCH_H_ */
