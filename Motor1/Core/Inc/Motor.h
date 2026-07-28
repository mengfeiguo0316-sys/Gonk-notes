#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   // ⭐最关键（HAL工程核心入口）

typedef enum {
    KEY_IDLE = 0,    // 空闲状态
    KEY_PRESSED_RAW, // 捕获到按下信号，等待消抖
    KEY_PRESSED_OK      // 确认按下，等待执行
} KeyState_t;

extern volatile uint16_t count;

// 你的函数声明
void Motor_Init();
void Motor_DEInit();
void Motor_Mode();

#ifdef __cplusplus
}
#endif

#endif