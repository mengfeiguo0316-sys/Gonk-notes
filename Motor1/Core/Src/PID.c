#include "PID.h"

PID_TypeDef PID[3];


/**
 * @brief  PID初始化函�?
 * @param  pid: PID结构体指�?
 * @param  Kp: 比例系数
 * @param  Ki: 积分系数
 * @param  Kd: 微分系数
 */
void PID_Init(PID_TypeDef *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->err=0;
    pid->last_err=0;
    pid->err_sum=0;

    pid->integral_max = 200.0f; // 积分上限，配合Ki参数允许更大I积分
    pid->integral_min = -200.0f; 

    pid->output_max = 49.0f; 
    pid->output_min = -49.0f; 
}

/**
 * @brief  PID位置式函数  进阶优化：“微分先行”
 * @param  pid: PID结构体指针
 * @retval 输出值
 */
float PID_Position(PID_TypeDef *pid)
{
    pid->err = pid->target - pid->feedback;

    // 输出P
    pid->output = pid->Kp * pid->err;

    // 累计误差积分
    pid->err_sum += pid->err * 0.01f; // 采样周期10ms

    //积分分离：当误差较大时，暂时不累加积分，避免积分饱和
    if(pid->err > 200.0f)
    {
        pid->err_sum = 0; // 清零积分
        pid->output += 0; // 不累加积分
    }
    else
    {
     //积分限幅保护
    if (pid->err_sum > pid->integral_max)
    {
        pid->err_sum = pid->integral_max; 
    }
    else if (pid->err_sum < pid->integral_min)
    {
        pid->err_sum = pid->integral_min; 
    }
    // 输出I
    pid->output += pid->Ki * pid->err_sum;
    }

    // 输出D
    pid->output += pid->Kd * (pid->err - pid->last_err);
    pid->last_err = pid->err;

    // 输出限幅保护
    if(pid->output > pid->output_max)
    {
        pid->output = pid->output_max;
    }
    else if(pid->output < pid->output_min)
    {
        pid->output = pid->output_min;
    }
    
    return pid->output;
}


/**
 * @brief  PID增量式函数  进阶优化：结合“积分分离”与“微分先行”?
 * @param  pid: PID结构体指针   
 * @retval 输出值
 */
float PID_Increment(PID_TypeDef *pid)
{
    // 输出限幅保护
    if(pid->output > pid->output_max)
    {
        pid->output = pid->output_max;
    }
    else if(pid->output < pid->output_min)
    {
        pid->output = pid->output_min;
    }
    
    return pid->output;
}
