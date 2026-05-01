#include "state_machine.h"
#include "motor.h" // 包含电机控制函数的头文件

State currentState;

void State_Init(void) {
    currentState = IDLE; // 初始化为待命状态
}

void State_Update(float pitch, float roll, float yaw) {
    switch (currentState) {
        case IDLE:
            // 检测条件，决定是否移动
            // 例如：如果检测到前方有障碍物，则转向
            if (/* 条件 */) {
                currentState = TURNING_LEFT; // 转向左
            }
            break;

        case MOVING:
            // 控制小车前进
            Motor_Control(FORWARD);
            break;

        case TURNING_LEFT:
            // 控制小车左转
            Motor_Control(LEFT);
            // 设置时间或角度，转动特定角度后转变状态
            break;

        case TURNING_RIGHT:
            // 控制小车右转
            Motor_Control(RIGHT);
            // 设置时间或角度，转动特定角度后转变状态
            break;

        default:
            break;
    }
}
