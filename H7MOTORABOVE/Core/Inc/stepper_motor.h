#ifndef INC_STEPPER_MOTOR_H_
#define INC_STEPPER_MOTOR_H_
#include <stdint.h>
//三个函数控制步进电机

//1.速度方向函数

void Motor_Speed_Dir(float v,uint8_t dir);    

//2.速度方向角度函数
void Motor_Speed_Dir_Angle(float v, uint8_t dir, float angle);

//3.紧急制动函数
void Motor_Stop();


#endif /* INC_STEPPER_MOTOR_H_ */ 