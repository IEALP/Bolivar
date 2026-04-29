#include "stepper_motor2.h"
#include "main.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim4;       // TIM4 句柄，由 CubeMX 生成

#define MICRO_STEP 64                  // 细分
#define STEP_ANGLE 1.8f                // 步进角
#define TIM4_FREQ  64000000.0f         // 定时器计数频率 (64MHz，与 TIM1 相同)

// 计步相关的全局变量（用于定角度转动）
volatile uint32_t motor2_target_pulses = 0;
volatile uint32_t motor2_current_pulses = 0;
volatile uint8_t  motor2_is_running = 0;

/* ==========================================================
 * 1. 速度方向函数（连续旋转）
 * ========================================================== */
void Motor2_Speed_Dir(float v, uint8_t dir) {
    if (v <= 0.0f) {
        Motor2_Stop();
        return;
    }
    // 使能电机
    HAL_GPIO_WritePin(GPIOB, MOTOR2_EN_Pin, GPIO_PIN_SET);
    // 设置方向 (PB5)
    HAL_GPIO_WritePin(GPIOB, MOTOR2_DIR_Pin, (dir ? GPIO_PIN_SET : GPIO_PIN_RESET));

    // 计算 ARR 值
    uint32_t arr_val = (uint32_t)(TIM4_FREQ * STEP_ANGLE / (v * MICRO_STEP));
    if (arr_val < 2) arr_val = 2;

    // 配置 TIM4
    TIM4->ARR = arr_val - 1;
    TIM4->CCR1 = arr_val / 2;          // 50% 占空比

    motor2_target_pulses = 0;          // 0 表示无限转动
    motor2_is_running = 1;
    
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

/* ==========================================================
 * 2. 速度、方向、角度函数（定角度转动）
 * ========================================================== */
void Motor2_Speed_Dir_Angle(float v, uint8_t dir, float angle) {
    if (v <= 0.0f || angle <= 0.0f) return;
    HAL_GPIO_WritePin(GPIOB, MOTOR2_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, MOTOR2_DIR_Pin, (dir ? GPIO_PIN_SET : GPIO_PIN_RESET));

    motor2_target_pulses = (uint32_t)((angle / STEP_ANGLE) * MICRO_STEP);
    motor2_current_pulses = 0;

    uint32_t arr_val = (uint32_t)(TIM4_FREQ * STEP_ANGLE / (v * MICRO_STEP));
    if (arr_val < 2) arr_val = 2;
    TIM4->ARR = arr_val - 1;
    TIM4->CCR1 = arr_val / 2;

    motor2_is_running = 1;
    HAL_TIM_Base_Start_IT(&htim4);             // 开启定时器中断
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  // 开启 PWM
}

/* ==========================================================
 * 3. 紧急制动/停止函数
 * ========================================================== */
void Motor2_Stop(void) {
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
    HAL_TIM_Base_Stop_IT(&htim4);
    motor2_is_running = 0;
}