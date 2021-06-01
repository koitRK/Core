/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define USER_Btn_EXTI_IRQn EXTI15_10_IRQn
#define LEFT_BLINKER_ON_OUT_Pin GPIO_PIN_3
#define LEFT_BLINKER_ON_OUT_GPIO_Port GPIOF
#define RIGHT_BLINKER_ON_OUT_Pin GPIO_PIN_5
#define RIGHT_BLINKER_ON_OUT_GPIO_Port GPIOF
#define SERIAL_JETSON_RX_Pin GPIO_PIN_6
#define SERIAL_JETSON_RX_GPIO_Port GPIOF
#define SERIAL_JETSON_TX_Pin GPIO_PIN_7
#define SERIAL_JETSON_TX_GPIO_Port GPIOF
#define ESTOP_IN_Pin GPIO_PIN_10
#define ESTOP_IN_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define BRAKE_LIGHT_ON_OUT_Pin GPIO_PIN_0
#define BRAKE_LIGHT_ON_OUT_GPIO_Port GPIOC
#define RMII_MDC_Pin GPIO_PIN_1
#define RMII_MDC_GPIO_Port GPIOC
#define HEAD_LIGHTS_ON_OUT_Pin GPIO_PIN_3
#define HEAD_LIGHTS_ON_OUT_GPIO_Port GPIOC
#define RMII_REF_CLK_Pin GPIO_PIN_1
#define RMII_REF_CLK_GPIO_Port GPIOA
#define RMII_MDIO_Pin GPIO_PIN_2
#define RMII_MDIO_GPIO_Port GPIOA
#define BRAKE_MAGNET_ADC1_IN3_Pin GPIO_PIN_3
#define BRAKE_MAGNET_ADC1_IN3_GPIO_Port GPIOA
#define STEERING_STEPPER_PWM_OUT_Pin GPIO_PIN_5
#define STEERING_STEPPER_PWM_OUT_GPIO_Port GPIOA
#define STEERING_STEPPER_DIRECTION_OUT_Pin GPIO_PIN_6
#define STEERING_STEPPER_DIRECTION_OUT_GPIO_Port GPIOA
#define RMII_CRS_DV_Pin GPIO_PIN_7
#define RMII_CRS_DV_GPIO_Port GPIOA
#define RMII_RXD0_Pin GPIO_PIN_4
#define RMII_RXD0_GPIO_Port GPIOC
#define RMII_RXD1_Pin GPIO_PIN_5
#define RMII_RXD1_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define SPEEDOMETER_EXTI_IN_Pin GPIO_PIN_12
#define SPEEDOMETER_EXTI_IN_GPIO_Port GPIOF
#define SPEEDOMETER_EXTI_IN_EXTI_IRQn EXTI15_10_IRQn
#define STEERING_ENCODER_DO_IN_Pin GPIO_PIN_14
#define STEERING_ENCODER_DO_IN_GPIO_Port GPIOF
#define STEERING_ENCODER_CLK_OUT_Pin GPIO_PIN_11
#define STEERING_ENCODER_CLK_OUT_GPIO_Port GPIOE
#define STEERING_ENCODER_CS_OUT_Pin GPIO_PIN_13
#define STEERING_ENCODER_CS_OUT_GPIO_Port GPIOE
#define THROTTLE_PWM_OUT_Pin GPIO_PIN_14
#define THROTTLE_PWM_OUT_GPIO_Port GPIOE
#define ATV_GEAR_OUT_Pin GPIO_PIN_15
#define ATV_GEAR_OUT_GPIO_Port GPIOE
#define RMII_TXD1_Pin GPIO_PIN_13
#define RMII_TXD1_GPIO_Port GPIOB
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define STEERING_STEPPER_ENABLE_OUT_Pin GPIO_PIN_14
#define STEERING_STEPPER_ENABLE_OUT_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define USB_OverCurrent_Pin GPIO_PIN_7
#define USB_OverCurrent_GPIO_Port GPIOG
#define USB_SOF_Pin GPIO_PIN_8
#define USB_SOF_GPIO_Port GPIOA
#define USB_VBUS_Pin GPIO_PIN_9
#define USB_VBUS_GPIO_Port GPIOA
#define USB_ID_Pin GPIO_PIN_10
#define USB_ID_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IBUS_UART_RX_Pin GPIO_PIN_0
#define IBUS_UART_RX_GPIO_Port GPIOD
#define IBUS_UART_TX_Pin GPIO_PIN_1
#define IBUS_UART_TX_GPIO_Port GPIOD
#define BRAKE_ACTUATOR_DIRECTION_OUT_Pin GPIO_PIN_9
#define BRAKE_ACTUATOR_DIRECTION_OUT_GPIO_Port GPIOG
#define RMII_TX_EN_Pin GPIO_PIN_11
#define RMII_TX_EN_GPIO_Port GPIOG
#define RMII_TXD0_Pin GPIO_PIN_13
#define RMII_TXD0_GPIO_Port GPIOG
#define BRAKE_ACTUATOR_ENABLE_OUT_Pin GPIO_PIN_14
#define BRAKE_ACTUATOR_ENABLE_OUT_GPIO_Port GPIOG
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define RC_BRAKE_ACTIVATION_VALUE 1490	// RC value below which vehicle will activate brakes
#define THROTTLE_OFF 0		 // Throttle value so motor won't turn
#define STEER_OFF 0
#define STEER_LEFT 1
#define STEER_RIGHT 2

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
