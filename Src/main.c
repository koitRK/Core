/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "estop.h"
#include "brake_hall_sensor.h"
#include "iBus.h"
#include "control_signal.h"
#include "utils.h"
#include "steering_encoder.h"
#include "kelly.h"
#include "brake_actuator.h"
#include "ATV_lights.h"
#include "steering_stepper.h"
#include "speedometer.h"
#include "imu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
TIM_HandleTypeDef htim4;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3; // XAVIER
DMA_HandleTypeDef hdma_usart3_rx; // XAVIER
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//static void MX_DMA_Init(void); // XAVIER
//static void MX_USART7_UART_Init(void); // XAVIER

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int32_t CH1_DC = 0;
  /* USER CODE END 1 */

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
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_UART4_Init();
  MX_TIM2_Init();
  MX_UART7_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	// initiate the reception of the first byte.
	HAL_UART_Receive_IT(&huart4, ibus_rx_buffer, 1);
	ReceiverState_t RC_data = {
		  .left_horizontal=1500,
		  .right_horizontal=1500,
		  .left_vertical=1500,
		  .right_vertical=1500,
		  .switch_a=ReceiverSwitchUp,
		  .switch_b=ReceiverSwitchUp,
		  .switch_c=ReceiverSwitchUp,
		  .switch_d=ReceiverSwitchUp};

	HAL_UART_Receive_IT (&huart3, ibus_rx_xavier_buffer, 1);

	uint8_t counter = 0;
	uint32_t current_serial_time = HAL_GetTick();
	uint32_t previous_serial_time = HAL_GetTick();
	uint8_t button_released = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

		if (ibus_rx_flags == IBUS_HAS_MESSAGE)
		{
			RC_data = HandleReceiverMessage();
		}
		else
		{
			// hacky solution to ISR errors.
			CheckUartInterruptStatus();
		}

		ControlSignal_t u = {0};

		const uint16_t target_steering_value = Map_To_Range(RC_data.right_horizontal, 1000, 2000, STEERING_ENCODER_LEFT_VALUE, STEERING_ENCODER_RIGHT_VALUE);
		const uint16_t current_steering_value = Get_Steering_Encoder_Value();
		const uint16_t brake_sensor_value = Get_Brake_Sensor_Value();
		const uint8_t estop_sensor_value = Read_Estop();
		const imu_t imu_data = {.acc={11, 22, 33}, .gyro={44, 55, 66}};

		const double current_braking_strength = Map_To_Range(brake_sensor_value, BRAKE_SENSOR_OFF_VALUE, BRAKE_SENSOR_ON_VALUE, 0, 1); // Value will go below 0, if piston contracted too much, or above 1, if piston pushed too much


		u.throttle = Clamp(Map_To_Range(RC_data.left_vertical, 1500, 2000, 0, 255), 0, 255);
		u.brake = RC_data.left_vertical < RC_BRAKE_ACTIVATION_VALUE ? 1 : 0;
		u.steering = current_steering_value < target_steering_value ? STEER_RIGHT : current_steering_value == target_steering_value ? STEER_OFF : STEER_LEFT;
		u.gear = RC_data.switch_a == ReceiverSwitchUp ? GEAR_FORWARD : GEAR_REVERSE;
		u.handbrake = RC_data.switch_c == ReceiverSwitchUp ? 0 : 1;
		u.steering_lock = RC_data.switch_d == ReceiverSwitchUp ? 0 : 1;
		u.xavier_override = RC_data.switch_b == ReceiverSwitchDown ? 1 : 0;

		if (u.xavier_override == 1)
		{
			// Controls come from Xavier
			u.throttle = xavier_command.throttle;
			u.brake = xavier_command.brake;
			u.steering = xavier_command.steering;
			u.gear = xavier_command.gear;
			u.h_light = xavier_command.h_light;
			u.r_light = xavier_command.r_light;
			u.l_light = xavier_command.l_light;
			// handbrake and steering_lock can still be controlled with RC controller
		}

		if (u.brake == 1 || u.handbrake == 1 || estop_sensor_value == ESTOP_ACTIVATED) // Brake
		{
			Set_Throttle(THROTTLE_OFF);   // Stopping ATV motor
			Set_Brake(BRAKE_ENGAGE);      // Engaging brakes
			Set_Brake_Light(LIGHT_ON);    // Brake light ON
		}
		else // Release brakes
		{
			Set_Brake(BRAKE_DISENGAGE);   // Disengage brakes
			Set_Brake_Light(LIGHT_OFF);   // Brake light off

			if (current_braking_strength <= 0) // Problematic??? If brake sensor has noise, throttle might be unstable TODO
			{
				Set_Gear(u.gear);		  // Change gear
				Set_Throttle(u.throttle); // Power ATV motor
			}
		}

		if (u.steering_lock == 0) // Steering not locked
			Steering_Stepper_Move(u.steering, STEER_ENABLED);
		else {
			Steering_Stepper_Move(u.steering, STEER_DISABLED);
		}

		Set_Headlights(u.h_light);
		Set_Left_Blinker(u.l_light);
		Set_Right_Blinker(u.r_light);


		MessageToXavier_t message_to_xavier = {.velocity=xavier_command.throttle, .steering_encoder=321, .imu=imu_data, .estop=5};
		Transmit_To_Xavier(message_to_xavier);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4
                              |RCC_PERIPHCLK_UART7;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == SPEEDOMETER_EXTI_IN_Pin)
  {
	  previous_speed_encoder_signal = current_speed_encoder_signal;
	  current_speed_encoder_signal = HAL_GetTick();
  }
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
