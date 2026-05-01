#include "gyro.h"

static uint8_t gyroData[GYRO_DATA_SIZE]; // 用于存放接收到的陀螺仪数据
static int dataReady = 0; // 数据准备标志

void Gyro_Init(void) {
    // 启动 USART 接收中断
    HAL_UART_Receive_IT(&huart1, gyroData, GYRO_DATA_SIZE);
}

void Read_Gyro_Data(float *pitch, float *roll, float *yaw) {
    if (dataReady) {
        int16_t gyroX = (gyroData[0] << 8) | gyroData[1]; // 解析角速度 X
        int16_t gyroY = (gyroData[2] << 8) | gyroData[3]; // 解析角速度 Y
        int16_t gyroZ = (gyroData[4] << 8) | gyroData[5]; // 解析角速度 Z

        // 根据需要的比例转换为角度
        *pitch = (float)gyroX / 32768.0f * 2000.0f;
        *roll = (float)gyroY / 32768.0f * 2000.0f;
        *yaw = (float)gyroZ / 32768.0f * 2000.0f;

        dataReady = 0; // 重置标志
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) { // 检查是否为目标 USART
        dataReady = 1; // 数据准备好，可以读取
        HAL_UART_Receive_IT(&huart1, gyroData, GYRO_DATA_SIZE); // 再次启动接收
    }
}
