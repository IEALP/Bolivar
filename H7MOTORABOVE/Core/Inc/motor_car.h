#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

// 方向与使能引脚宏定义
#define L_F_IN1_PORT GPIOD   // 左前
#define L_F_IN1_PIN  GPIO_PIN_0
#define L_F_IN2_PORT GPIOD   
#define L_F_IN2_PIN  GPIO_PIN_1

#define L_B_IN1_PORT GPIOD   // 左后
#define L_B_IN1_PIN  GPIO_PIN_2
#define L_B_IN2_PORT GPIOD   
#define L_B_IN2_PIN  GPIO_PIN_3

#define R_F_IN1_PORT GPIOD   // 右前
#define R_F_IN1_PIN  GPIO_PIN_4
#define R_F_IN2_PORT GPIOD   
#define R_F_IN2_PIN  GPIO_PIN_5

#define R_B_IN1_PORT GPIOD   // 右后
#define R_B_IN1_PIN  GPIO_PIN_6
#define R_B_IN2_PORT GPIOD   
#define R_B_IN2_PIN  GPIO_PIN_7

#define TB1_STBY_PORT GPIOD  // 使能
#define TB1_STBY_PIN  GPIO_PIN_8
#define TB2_STBY_PORT GPIOD
#define TB2_STBY_PIN  GPIO_PIN_9

// 编码器数据结构体
typedef struct {
    int16_t LeftFront;
    int16_t LeftBack;
    int16_t RightFront;
    int16_t RightBack;
} EncoderData_t;

// 外部函数声明
void Motor_Init(void);
void Motor_SetSideSpeed(int16_t left_speed, int16_t right_speed);
void Motor_Stop(void);
EncoderData_t Motor_ReadEncoders(void);

#endif /* __MOTOR_H */