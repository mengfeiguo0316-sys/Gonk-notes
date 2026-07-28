#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h" // ⭐最关键（HAL工程核心入口）
#define SPEED_FILTER_ALPHA 0.3f 

    typedef struct Encoder
    {
        uint16_t count_now;  // 当前计数值
        uint16_t count_last; // 上一次的计数值
        int16_t delta;       // 脉冲变化量（有符号，正数代表正转，负数代表反转）
        float motor_rpm;  // 计算得到的实际输出轴转速 (RPM)
    }Encoder;

    extern Encoder encoder;
    float Get_Speed();
#ifdef __cplusplus
}
#endif

#endif