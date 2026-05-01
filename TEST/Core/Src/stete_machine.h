#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "stm32f1xx_hal.h" // 适用于 STM32F103 的 HAL 库

// 状态定义
typedef enum {
    IDLE,
    MOVING,
    TURNING_LEFT,
    TURNING_RIGHT
} State;

// 函数声明
void State_Init(void);
void State_Update(float pitch, float roll, float yaw);

#endif // STATE_MACHINE_H
