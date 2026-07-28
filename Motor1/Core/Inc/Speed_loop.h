#ifndef __SPEED_LOOP_H
#define __SPEED_LOOP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"  // ⭐最关键（HAL工程核心入口）


void Speed_loop_PID_Position(float target_speed);
void Speed_loop_Init(void);
#ifdef __cplusplus
}
#endif

#endif