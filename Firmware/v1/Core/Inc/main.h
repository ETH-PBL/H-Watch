/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32wbxx_hal.h"

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

/* BACKUP_PRIMASK MUST be implemented at the begining of the funtion
   that implement a critical section
   PRIMASK is saved on STACK and recovered at the end of the funtion
   That way RESTORE_PRIMASK ensures critical sections are maintained even in nested calls...*/
#define BACKUP_PRIMASK()  uint32_t primask_bit1 = __get_PRIMASK()
#define DISABLE_IRQ() __disable_irq()
#define ENABLE_IRQ() __enable_irq()
#define RESTORE_PRIMASK() __set_PRIMASK(primask_bit1)


#define CRITICAL_SECTION_BEGIN( )     uint32_t primask_bit= __get_PRIMASK();\
                                    __disable_irq()
#define CRITICAL_SECTION_END( )   __set_PRIMASK(primask_bit)

/*!
 * \brief Returns the minimum value between a and b
 *
 * \param [IN] a 1st value
 * \param [IN] b 2nd value
 * \retval minValue Minimum value
 */
#define MIN( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

/*!
 * \brief Returns the maximum value between a and b
 *
 * \param [IN] a 1st value
 * \param [IN] b 2nd value
 * \retval maxValue Maximum value
 */
#define MAX( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )

/*!
 * \brief Returns 2 raised to the power of n
 *
 * \param [IN] n power value
 * \retval result of raising 2 to the power n
 */
#define POW2( n ) ( 1 << n )

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

extern void GLOB_SET_EVT( void* xEventGroup, uint32_t x );
extern void Mngm_DeepSleep_en (uint8_t d);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LSM303_INT_MAG_Pin GPIO_PIN_0
#define LSM303_INT_MAG_GPIO_Port GPIOA
#define LSM303_INT_MAG_EXTI_IRQn EXTI0_IRQn
#define LSM303_INT2_Pin GPIO_PIN_2
#define LSM303_INT2_GPIO_Port GPIOA
#define LSM303_INT2_EXTI_IRQn EXTI2_IRQn
#define LSM303_INT1_Pin GPIO_PIN_3
#define LSM303_INT1_GPIO_Port GPIOA
#define LSM303_INT1_EXTI_IRQn EXTI3_IRQn
#define RI_INT_Pin GPIO_PIN_4
#define RI_INT_GPIO_Port GPIOA
#define BC95_Reset_Pin GPIO_PIN_5
#define BC95_Reset_GPIO_Port GPIOA
#define LPS22HB_INT_Pin GPIO_PIN_8
#define LPS22HB_INT_GPIO_Port GPIOA
#define LPS22HB_INT_EXTI_IRQn EXTI9_5_IRQn
#define LPS22HB_CS_Pin GPIO_PIN_2
#define LPS22HB_CS_GPIO_Port GPIOB
#define LSM6DSM_INT2_Pin GPIO_PIN_10
#define LSM6DSM_INT2_GPIO_Port GPIOB
#define LSM6DSM_INT2_EXTI_IRQn EXTI15_10_IRQn
#define LSM6DSM_INT1_Pin GPIO_PIN_11
#define LSM6DSM_INT1_GPIO_Port GPIOB
#define LSM6DSM_INT1_EXTI_IRQn EXTI15_10_IRQn
#define LSM6DSM_CS_Pin GPIO_PIN_1
#define LSM6DSM_CS_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOD
#define PPG_INT_Pin GPIO_PIN_6
#define PPG_INT_GPIO_Port GPIOB
#define PPG_INT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
