#include "motor_car.h"
#include "tim.h" 
#include <math.h>
#include <stdlib.h>

/**
 * @brief  初始化电机、PWM和编码器
 */
void Motor_Init(void)
{
    // 1. 启动PWM输出 
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // 左前
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2); // 左后
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1); // 右前
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2); // 右后
    
    // 2. 启动新的四个编码器定时器 (避开TIM1, TIM4)
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // 左前 (PA0, PA1)
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 左后 (PA6, PA7)
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 右前 (PA15, PB3)
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL); // 右后 (PC6, PC7)
    
    // 3. 开启待机模块(PD8, PD9 拉高)
    HAL_GPIO_WritePin(TB1_STBY_PORT, TB1_STBY_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TB2_STBY_PORT, TB2_STBY_PIN, GPIO_PIN_SET);
    
    Motor_Stop();
}

/**
 * @brief  控制小车两侧的速度 (-1000 到 1000)
 */
void Motor_SetSideSpeed(int16_t left_speed, int16_t right_speed)
{
    // 限幅
    if(left_speed > 1000) left_speed = 1000;
    if(left_speed < -1000) left_speed = -1000;
    if(right_speed > 1000) right_speed = 1000;
    if(right_speed < -1000) right_speed = -1000;

    // ----- 左侧 -----
    GPIO_PinState l_in1 = (left_speed > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState l_in2 = (left_speed < 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if(left_speed == 0) { l_in1 = GPIO_PIN_RESET; l_in2 = GPIO_PIN_RESET; }
    
    HAL_GPIO_WritePin(L_F_IN1_PORT, L_F_IN1_PIN, l_in1);
    HAL_GPIO_WritePin(L_F_IN2_PORT, L_F_IN2_PIN, l_in2);
    HAL_GPIO_WritePin(L_B_IN1_PORT, L_B_IN1_PIN, l_in1);
    HAL_GPIO_WritePin(L_B_IN2_PORT, L_B_IN2_PIN, l_in2);
    __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, abs(left_speed));
    __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, abs(left_speed));

    // ----- 右侧 -----
    GPIO_PinState r_in1 = (right_speed > 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState r_in2 = (right_speed < 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if(right_speed == 0) { r_in1 = GPIO_PIN_RESET; r_in2 = GPIO_PIN_RESET; }
    
    HAL_GPIO_WritePin(R_F_IN1_PORT, R_F_IN1_PIN, r_in1);
    HAL_GPIO_WritePin(R_F_IN2_PORT, R_F_IN2_PIN, r_in2);
    HAL_GPIO_WritePin(R_B_IN1_PORT, R_B_IN1_PIN, r_in1);
    HAL_GPIO_WritePin(R_B_IN2_PORT, R_B_IN2_PIN, r_in2);
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, abs(right_speed));
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, abs(right_speed));
}

/**
 * @brief  急停
 */
void Motor_Stop(void)
{
    Motor_SetSideSpeed(0, 0);
}

/**
 * @brief  读取4个轮子的脉冲数并清零
 */
EncoderData_t Motor_ReadEncoders(void)
{
    EncoderData_t enc;
    
    enc.LeftFront  = (int16_t)__HAL_TIM_GET_COUNTER(&htim5); // PA0, PA1
    enc.LeftBack   = (int16_t)__HAL_TIM_GET_COUNTER(&htim3); // PA6, PA7
    enc.RightFront = (int16_t)__HAL_TIM_GET_COUNTER(&htim2); // PA15, PB3
    enc.RightBack  = (int16_t)__HAL_TIM_GET_COUNTER(&htim8); // PC6, PC7
    
    __HAL_TIM_SET_COUNTER(&htim5, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim8, 0);
    
    return enc;
}
