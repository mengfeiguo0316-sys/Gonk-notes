#include "Motor.h"
#include "tim.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

volatile KeyState_t KeyState = KEY_IDLE; // 按键状态标志
volatile uint32_t key_pretime=0;
volatile uint16_t count;//占空比

/**
* @brief  电机初始化函数
  * @param  无
  */
void Motor_Init()
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,1);//使能驱动芯片引脚
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,0);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,0);
}

/**
* @brief  电机刹车函数
  * @param  无
  */
void Motor_DEInit()
{
	HAL_GPIO_WritePin(STBY_GPIO_Port,STBY_Pin,0);//使能驱动芯片引脚
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,0);
	HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,0);
}

/**
* @brief  电机转向切换函数
  * @param  无
  */
static void Motor_Mode_Change(void)
{
	char msg[20] = {0};
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0); // PWM=0
	// 先进入刹车状态（让电流快速衰减）
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);   // 刹车
    
    HAL_Delay(140);   // 根据电机实际情况调整 520电机差不多140ms左右反电动势和机械惯性消失！！！
	
    // 切换到目标方向（这里示例切换方向）
    static uint8_t dir = 0;
    dir = !dir;
    sprintf(msg, "NOW count is:%d\r\n", count);
    if(dir)
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 0);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 1);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }
    else
    {
        HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, 1);
        HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, 0);
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }
	__HAL_TIM_SetCounter(&htim3,count);
}


/**
* @brief  按键状态机处理函数
  * @param  无
  */
void Motor_Mode()
{
	switch(KeyState)
	{
		case KEY_IDLE:
			if(Key_pressed)
			{
			Key_pressed=0;// 收到信号立马清零，防止重复触发
			KeyState=KEY_PRESSED_RAW;
			key_pretime=HAL_GetTick();
			}
			break;
		
		case KEY_PRESSED_RAW:
			if((HAL_GetTick()-key_pretime)>20)
			{
				if(HAL_GPIO_ReadPin(KEY_Mode_GPIO_Port,KEY_Mode_Pin)==1)
				{
					KeyState=KEY_PRESSED_OK;
				}
				else
				{
					KeyState = KEY_IDLE;       // 发现是抖动，打回原形
				}
			}
			break;
		
		case KEY_PRESSED_OK:
			//调用驱动API，高内聚低耦合。
			Motor_Mode_Change(); 
			KeyState=KEY_IDLE;
			break;
		
		default: 
			KeyState=KEY_IDLE;
			break;
	}
}

