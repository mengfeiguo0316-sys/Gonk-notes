#include "Speed_loop.h"
#include "Encoder.h"
#include "PID.h"
#include "tim.h"

/**
 * @brief  速度环PID初始化函数
 * @param  无
 * @return  无
 */
void Speed_loop_Init(void)
{
  HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
  HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 1);
  PID_Init(&PID[0], 0.5f, 0.3f, 0.0f);
}
/**
 * @brief  速度环PID控制函数
 * @param  target_speed: 目标速度 (单位: RPM)
 * @return  无
 */
void Speed_loop_PID_Position(float target_speed)
{
  PID[0].target = target_speed;
  PID[0].feedback = Get_Speed(); // 获取编码器测速值
  float pwm = PID_Position(&PID[0]);

  if (pwm < 0)
  {
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 1);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
    pwm = -pwm; // 取绝对值
  }
  else
  {
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 1);
  }
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm); // 将PID输出值写入PWM寄存器
}
