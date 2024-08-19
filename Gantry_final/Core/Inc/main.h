/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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
#define c1_Pin GPIO_PIN_2
#define c1_GPIO_Port GPIOE
#define e1_Pin GPIO_PIN_3
#define e1_GPIO_Port GPIOE
#define cylinder_02_Pin GPIO_PIN_4
#define cylinder_02_GPIO_Port GPIOE
#define electromagnet_02_Pin GPIO_PIN_5
#define electromagnet_02_GPIO_Port GPIOE
#define cylinder_03_Pin GPIO_PIN_0
#define cylinder_03_GPIO_Port GPIOC
#define electromagnet_03_Pin GPIO_PIN_1
#define electromagnet_03_GPIO_Port GPIOC
#define cylinder_05_Pin GPIO_PIN_2
#define cylinder_05_GPIO_Port GPIOC
#define electromagnet_05_Pin GPIO_PIN_3
#define electromagnet_05_GPIO_Port GPIOC
#define PUL_Pin GPIO_PIN_6
#define PUL_GPIO_Port GPIOA
#define DIR_Pin GPIO_PIN_7
#define DIR_GPIO_Port GPIOA
#define ENA_Pin GPIO_PIN_4
#define ENA_GPIO_Port GPIOC
#define electromagnet_04_Pin GPIO_PIN_7
#define electromagnet_04_GPIO_Port GPIOE
#define electromagnet_01_Pin GPIO_PIN_9
#define electromagnet_01_GPIO_Port GPIOE
#define cylinder_04_Pin GPIO_PIN_14
#define cylinder_04_GPIO_Port GPIOD
#define cylinder_01_Pin GPIO_PIN_5
#define cylinder_01_GPIO_Port GPIOB
#define Reset_02_Pin GPIO_PIN_8
#define Reset_02_GPIO_Port GPIOB
#define Reset_03_Pin GPIO_PIN_9
#define Reset_03_GPIO_Port GPIOB
#define Reset_01_Pin GPIO_PIN_0
#define Reset_01_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
