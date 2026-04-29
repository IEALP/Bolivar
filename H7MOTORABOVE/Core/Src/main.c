/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stepper_motor.h"
#include "stepper_motor2.h"
#include "motor_car.h"  
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern volatile uint8_t is_motor_running;     // 电机1运行标志
extern volatile uint8_t motor2_is_running;    // 电机2运行标志
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  EncoderData_t my_encoder_data;
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM12_Init();
  MX_TIM15_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init();  // 初始化小车电机和编码器
  HAL_Delay(2000); // 等待系统稳定
  // ===================== 动作调试序列 (开环延时控制) =====================

  // 动作1：直线前进
  // 左右侧给出相同的正向速度
  Motor_SetSideSpeed(500, 500);  
  HAL_Delay(2000);     // 假设前进2秒
  Motor_Stop();        // 刹车
  HAL_Delay(1000);     // 停顿1秒

  // 动作2：原地右转 90度
  // 左侧正转，右侧反转产生差速
  Motor_SetSideSpeed(400, -400); 
  HAL_Delay(800);      // 【微调点】根据地面的摩擦力，调整这个延时让它刚好转90度
  Motor_Stop();
  HAL_Delay(1000);

  // 动作3：直线前进
  Motor_SetSideSpeed(500, 500);  
  HAL_Delay(1500);     // 前进1.5秒
  Motor_Stop();
  HAL_Delay(1000);

  // 动作4：原地左转掉头 180度
  // 左侧反转，右侧正转
  Motor_SetSideSpeed(-400, 400); 
  HAL_Delay(1600);     // 【微调点】大约是转90度延时的两倍
  Motor_Stop();
  HAL_Delay(1000);

  // 动作5：原路返回
  Motor_SetSideSpeed(500, 500);  
  HAL_Delay(1500);     // 对应动作3的距离
  Motor_Stop();
  HAL_Delay(1000);
  
  // 动作6：原地左转 90度，方向朝向最初始状态
  Motor_SetSideSpeed(-400, 400); 
  HAL_Delay(800);      
  Motor_Stop();
  HAL_Delay(1000);

  // 动作7：退回到绝对起点位置
  Motor_SetSideSpeed(500, 500);  
  HAL_Delay(2000);     // 对应动作1的距离
  Motor_Stop();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // // --- 电机1：一次性转20圈 (20 * 360 = 7200度) ---
    // // 速度设为360度/秒，20圈需要20秒
    // Motor_Speed_Dir_Angle(360.0f, 1, 7200.0f);  // 顺时针
    // // --- 电机2：连续转20次，每次3.6度 ---
    // for (int i = 0; i < 20; i++)
    // {
    //   Motor2_Speed_Dir_Angle(36.0f, 0, 3.6f);   // 逆时针，速度36度/秒
    //   // 等待电机2完成本次3.6度转动
    //   while (motor2_is_running)
    //   {
    //     HAL_Delay(5);
    //   }
    //   HAL_Delay(100);   // 每两次之间短暂停顿，方便观察
    // }
    // // --- 电机2的20次已经全部做完，现在等待电机1完成20圈 ---
    // while (is_motor_running)
    // {
    //   HAL_Delay(10);
    // }
    // // 全部完成后，停留5秒再重新开始（可根据需求删除此延时）
    // HAL_Delay(5000);
    
    // 在死循环中，您可以不断读取编码器的值，用于后续开发闭环控制（PID）
      // 这个函数内部会自动清零计数器，所以读到的是这一小段时间内的脉冲数(速度)
      my_encoder_data = Motor_ReadEncoders();
      
      // 如果您配置了串口重定向 printf，可以打印出来观察打滑情况：
      // printf("LF:%d, LB:%d, RF:%d, RB:%d\r\n", 
      //         my_encoder_data.LeftFront, my_encoder_data.LeftBack,
      //         my_encoder_data.RightFront, my_encoder_data.RightBack);

      HAL_Delay(50); // 每50ms读取一次（通常作为PID的控制周期）
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 32;
  RCC_OscInitStruct.PLL.PLLN = 480;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
