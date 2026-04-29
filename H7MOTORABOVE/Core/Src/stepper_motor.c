#include "stepper_motor.h"
#include "main.h"                     // 包含 H7 HAL 及引脚标签定义
#include <stdint.h>
#include "stepper_motor2.h"

extern TIM_HandleTypeDef htim1;       // TIM1 句柄，由 CubeMX 生成

#define MICRO_STEP 64                  // 细分
#define STEP_ANGLE 1.8f                // 步进角
#define TIM_FREQ   64000000.0f         // 定时器计数频率 (64MHz)

// 计步相关的全局变量（用于定角度转动）
volatile uint32_t target_pulses = 0;   // 目标脉冲数
volatile uint32_t current_pulses = 0;  // 当前已发脉冲数
volatile uint8_t  is_motor_running = 0; // 电机运行状态标志

/* ==========================================================
 * 1. 速度方向函数（连续旋转）
 * ========================================================== */
void Motor_Speed_Dir(float v, uint8_t dir) {
    if (v <= 0.0f) {
        Motor_Stop();
        return;
    }
    HAL_GPIO_WritePin(GPIOE, MOTOR1_EN_Pin, GPIO_PIN_SET);

    // 1. 设置方向引脚 (PE8)
    HAL_GPIO_WritePin(GPIOE, MOTOR1_DIR_Pin, (dir ? GPIO_PIN_SET : GPIO_PIN_RESET));

    // 2. 计算 ARR 值
    // 公式: ARR = 定时器频率 * 步进角 / (速度 * 细分)
    uint32_t arr_val = (uint32_t)(TIM_FREQ * STEP_ANGLE / (v * MICRO_STEP));
    
    if (arr_val < 2) arr_val = 2; // 防止ARR过小导致定时器崩溃

    // 3. 配置定时器寄存器
    TIM1->ARR = arr_val - 1;           // 频率设定
    TIM1->CCR1 = arr_val / 2;          // 占空比 50%

    // 4. 清除计步标志并启动PWM
    target_pulses = 0;                 // 0 表示无限转动
    is_motor_running = 1;
    
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 开启PWM输出
}

/* ==========================================================
 * 2. 速度、方向、角度函数（定角度转动）
 * ========================================================== */
void Motor_Speed_Dir_Angle(float v, uint8_t dir, float angle) {
    if (v <= 0.0f || angle <= 0.0f) return;
    HAL_GPIO_WritePin(GPIOE, MOTOR1_EN_Pin, GPIO_PIN_SET);

    // 1. 设置方向
    HAL_GPIO_WritePin(GPIOE, MOTOR1_DIR_Pin, (dir ? GPIO_PIN_SET : GPIO_PIN_RESET));

    // 2. 将角度转换为需要发送的脉冲总数
    target_pulses = (uint32_t)((angle / STEP_ANGLE) * MICRO_STEP);
    current_pulses = 0;

    // 3. 计算并设置 ARR 和 CCR
    uint32_t arr_val = (uint32_t)(TIM_FREQ * STEP_ANGLE / (v * MICRO_STEP));
    if (arr_val < 2) arr_val = 2;
    TIM1->ARR = arr_val - 1;
    TIM1->CCR1 = arr_val / 2;     // 50% 占空比

    // 4. 启动定时器中断和PWM
    is_motor_running = 1;
    HAL_TIM_Base_Start_IT(&htim1);            // 开启定时器中断(用于计步)
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 开启PWM输出脉冲
}

/* ==========================================================
 * 3. 紧急制动/停止函数
 * ========================================================== */
void Motor_Stop(void) {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);  // 停止PWM脉冲输出
    HAL_TIM_Base_Stop_IT(&htim1);             // 停止定时器中断
    is_motor_running = 0;
}

/* ==========================================================
 * 4. 定时器中断回调函数 (用于电机1和电机2的计步)
 * ========================================================== */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // 电机1 (TIM1)
    if (htim->Instance == TIM1) {
        if (is_motor_running && target_pulses > 0) {
            current_pulses++;
            if (current_pulses >= target_pulses) {
                Motor_Stop();
            }
        }
    }
    // 电机2 (TIM4)
    else if (htim->Instance == TIM4) {
        if (motor2_is_running && motor2_target_pulses > 0) {
            motor2_current_pulses++;
            if (motor2_current_pulses >= motor2_target_pulses) {
                Motor2_Stop();
            }
        }
    }
}