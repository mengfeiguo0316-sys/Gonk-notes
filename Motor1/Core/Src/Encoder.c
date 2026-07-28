#include "Encoder.h"
#include "tim.h"

Encoder encoder = {0};

/**
 * @brief  编码器测速度函数 (针对 11线, 4倍频, 1:18.8减速比, 10ms采样周期)
 * @brief   alpha 越小，滤波越平滑（抗心跳能力越强），但延迟越大,滤波系数范围 0.0 ~ 1.0
 * @note   该函数优化方向为加一个滤波算法，减少抖动
 * @param  无
 */
float Get_Speed(void)
{
    // 1. 计算脉冲差值 (完美处理了 16位 溢出)
    encoder.count_now = __HAL_TIM_GET_COUNTER(&htim2);
    encoder.delta = (int16_t)(encoder.count_now - encoder.count_last);
    //这里溢出后，可能为-65530这种情况，强制转换为int16_t后，变为6
    encoder.count_last = encoder.count_now;

    // 2. 计算当前周期的原始转速
    float raw_rpm = ((float)encoder.delta / 827.2f) * 6000.0f;

    // 3. 突发噪声过滤 (限幅滤波)
    static float last_raw_rpm = 0.0f;
    float diff = raw_rpm - last_raw_rpm;
    if (diff < 0.0f) diff = -diff;
    if (diff > 200.0f)
    {
        raw_rpm = last_raw_rpm; // 如果突变超过 200 RPM，丢弃当前值，沿用上次真实的原始值
    }
    last_raw_rpm = raw_rpm; 

    // 4. 一阶低通滤波 (抹平“心跳”锯齿)
    // 滤波公式：新值 = a * 原始值 + (1 - a) * 上次滤波值
    static float filtered_rpm = 0.0f;
    filtered_rpm = SPEED_FILTER_ALPHA * raw_rpm + (1.0f - SPEED_FILTER_ALPHA) * filtered_rpm;
    
    encoder.motor_rpm = filtered_rpm;
    return encoder.motor_rpm;
}

