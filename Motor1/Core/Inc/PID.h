#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   // ⭐最关键（HAL工程核心入口）

typedef struct 
{
    // --- 1. 基础控制参数 ---
    float Kp;
    float Ki;
    float Kd;
    
    // --- 2. 实时运行数据 ---
    float target;      // 目标速度 (设定值)
    float feedback;    // 反馈速度 (编码器测得值)
    float err;         // 当前误差 e(k)

    // --- 3. 历史记忆 (离散化必须) ---
    float last_err;    // 上次误差 e(k-1) 
    float prev_err;    // 上上次误差 e(k-2) （用增量式必须加）
    float err_sum;     //  误差积分 ∑e(k),历史误差累计 （用位置式必须加）
    
    // --- 4. 安全限幅保护 ---
    float integral_max; // 积分上限 (防积分饱和)
    float integral_min; // 积分下限
    float output_max;   // 最终输出上限 (例如限制PWM最大为100)
    float output_min;   // 最终输出下限 (如果电机允许反转，这里可以是-100)
    
    // --- 5. 计算结果 ---
    float output;      // 算出的最终控制量，准备交给PWM寄存器
    
} PID_TypeDef;

float PID_Position(PID_TypeDef *pid);
void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd);

extern PID_TypeDef PID[3]; // PID结构体数组，0:PID速度环, 1:PID位置环, 2:PID电流环

#ifdef __cplusplus
}
#endif

#endif