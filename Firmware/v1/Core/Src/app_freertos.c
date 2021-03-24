/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "macro.h"
#include "Int_Sensors.h"
#include "BC95_Def.h"
#include "Display.h"

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
/* USER CODE BEGIN Variables */

osThreadId IntSensors_TaskHandle;
osThreadId BC95_TaskHandle;
osThreadId Display_TaskHandle;

static int32_t en_deepsleep = 0;
#define DEEP_SLEEP_OK()			(en_deepsleep == 0)

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN VPORT_SUPPORT_TICKS_AND_SLEEP */

#pragma GCC push_options
#pragma GCC optimize ("O3")

/**
 * @brief Switch the system clock on HSI
 * @param none
 * @retval none
 */
static inline void Switch_On_HSI( void )
{
	LL_RCC_HSI_Enable( );
	while(!LL_RCC_HSI_IsReady( ));
	LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_HSI );
	LL_RCC_SetSMPSClockSource(LL_RCC_SMPS_CLKSOURCE_HSI);
	while (LL_RCC_GetSysClkSource( ) != LL_RCC_SYS_CLKSOURCE_STATUS_HSI);
}

/**
 * @brief Switch the system clock on HSI
 * @param none
 * @retval none
 */
static inline void Exit_Stop( void )
{
	/* Enable clock source back */
	if(LL_RCC_GetSysClkSource( ) == LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
	{
		LL_RCC_HSI_Enable( );
		LL_RCC_HSE_Enable( );
		LL_RCC_PLL_Enable( );
		while(!LL_RCC_HSI_IsReady( ));
		while(!LL_RCC_HSE_IsReady( ));
		while(!LL_RCC_PLL_IsReady( ));
		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
		while (LL_RCC_GetSysClkSource( ) != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
	}
}

#pragma GCC pop_options


__weak void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
  // Generated when configUSE_TICKLESS_IDLE == 2.
  // Function called in tasks.c (in portTASK_FUNCTION).
  // TO BE COMPLETED or TO BE REPLACED by a user one, overriding that weak one.

#if CHECK_HEAP_RUNTIME

	volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	    FreeRTOSConfig.h.

	    This function is called on each cycle of the idle task.  In this case it
	    does nothing useful, other than report the amount of FreeRTOS heap that
	    remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace < 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
	        if there is a lot of heap remaining unallocated then
	        the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
	        reduced accordingly. */
		while(1);
	}

#endif

#if (RTOS_SLEEP_EN) && (!DEEP_SLEEP_EN)

	DISABLE_IRQ();
	/* sleep MCU */
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	ENABLE_IRQ();

#endif

#if DEEP_SLEEP_EN
	if(DEEP_SLEEP_OK()){

		CRITICAL_SECTION_BEGIN();

		HAL_SuspendTick();

		/*clear wake up flag*/
		LL_PWR_ClearFlag_WU( );

		/* clear EXTI flags */
		//LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_ALL_0_31);
		//LL_EXTI_ClearFlag_32_63(LL_EXTI_LINE_ALL_32_63);

		/*        2.2.9 Incomplete Stop 2 mode entry after a wakeup from debug upon EXTI line 48 event
		 *        "With the JTAG debugger enabled on GPIO pins and after a wakeup from debug triggered by an event on EXTI
		 *        line 48 (CDBGPWRUPREQ), the device may enter in a state in which attempts to enter Stop 2 mode are not fully
		 *        effective ..."
		 *        Workaround implementation example using LL driver:
		 */
		LL_EXTI_DisableIT_32_63(LL_EXTI_LINE_48);
		LL_C2_EXTI_DisableIT_32_63(LL_EXTI_LINE_48);

		LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_HSI);
		Switch_On_HSI();

#ifdef STM32WB55xx
		/* Set Stop 2 mode of CPU2 */
		LL_C2_PWR_SetPowerMode(LL_PWR_MODE_STOP2);
#endif
		/* Request to enter Stop 2 mode */
		HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

		Exit_Stop();

		HAL_ResumeTick();

		CRITICAL_SECTION_END( );

		/* Clear Wake Up Flag */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	}else{
		DISABLE_IRQ();
		/* sleep MCU */
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		ENABLE_IRQ();
	}
#endif

}
/* USER CODE END VPORT_SUPPORT_TICKS_AND_SLEEP */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(InSensorsTask, IntSensors_Task, osPriorityAboveNormal, 0, 256);
  IntSensors_TaskHandle = osThreadCreate(osThread(InSensorsTask), TRUE);			// TRUE

//  osThreadDef(InBC95, BC95_op, osPriorityNormal, 0, 512);
//  BC95_TaskHandle = osThreadCreate(osThread(InBC95), TRUE);

  osThreadDef(Dis_Task, Display_Task, osPriorityLow, 0, 512);
  Display_TaskHandle = osThreadCreate(osThread(Dis_Task), NULL);					// TRUE

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void Mngm_DeepSleep_en (uint8_t d){
	if (d){
		en_deepsleep--;
	}else{
		en_deepsleep++;
	}
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
