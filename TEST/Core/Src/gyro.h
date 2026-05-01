#ifndef GYRO_H
#define GYRO_H

#include "stm32f1xx_hal.h" // 根据你的具体 STM32 系列进行调整

// JY901P 陀螺仪 USART 句柄
extern UART_HandleTypeDef huart1; // 根据在 CubeMX 中配置的 USART

// JY901P 数据大小
#define GYRO_DATA_SIZE 8

// 函数声明
void Gyro_Init(void);
void Read_Gyro_Data(float *pitch, float *roll, float *yaw);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart); // UART 接收回调

#endif // GYRO_H
