/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f0xx_it.h"
#include "include.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

uint8_t Finger_Inter_IT = 0;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

	printf("NMI_Handler ERROR!\n");
  while (1)
  {
		NVIC_SystemReset();
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
	printf("HardFault_Handler ERROR!\n");
  while (1)
  {
		NVIC_SystemReset();
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */
	printf("SVC_Handler ERROR!\n");
  while (1)
  {
		NVIC_SystemReset();
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */
	printf("PendSV_Handler ERROR!\n");

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();	
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN)
{
	uint8_t Touch_Bufer[3] = {0};
	if (GPIO_PIN == GPIO_PIN_12)
	{
		printf("撬开中断\n");
		Lusers.APMData.Inter_Data = PICK_INTER;
		Lusers.APMData.pick_state = 1;
		Lusers.APMData.Pick_toWiFi = 0;
		WTN6_Send_Data(0xFE);
	}
	else if (GPIO_PIN == GPIO_PIN_0)
	{
		printf("刷卡中断\n");
		Lusers.APMData.Inter_Data = NFC_INTER;
	}
	else if (GPIO_PIN == GPIO_PIN_1)
	{
		printf("室内中断\n");
		Lusers.APMData.Inter_Data = RESET_INTER;		
	}
  else if (GPIO_PIN == TOUCH_INTER_PIN)
	{
		printf("触摸中断\n");
		Lusers.APMData.Inter_Data = TOUCH_INTER;
		Touch_Bufer[0] = Touch_ReadByte(0x10);
		Touch_Bufer[1] = Touch_ReadByte(0x11);
		Touch_Bufer[2] = Touch_ReadByte(0x12);
		Touch_ReadKey(Touch_Bufer);
	}
	else if (GPIO_PIN == FINTER_PIN)
	{
		printf("手指触发中断\n");
		Finger_Inter_IT = 1;
		Lusers.APMData.Inter_Data = FINGER_INTER;
		Lusers.Application.ONOFFFinger(FINGER_POWER_ON);

		if (!Finger_Add_State_IT)
		{
			if ((Lusers.APMData.Show_Page == OLEDMAIN) || (Lusers.APMData.Show_Page == OLEDADMIN) || (Lusers.APMData.Show_Page == OLEDOPENPLEASEFINGER) || (Lusers.APMData.Show_Page == OLEDINPUTPWD) || (Lusers.APMData.Show_Page == OLEDADMININPUTPWD))
			{
				printf("发送验证指令\n");
				Lusers.APMData.OLEDReturn = 0;
				Lusers.Application.Finger_Play(FINGER_VERITY, 0);
			}
		}
	}
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN);
}

void EXTI0_1_IRQHandler(void)
{
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
	{
		HAL_GPIO_EXTI_Callback(GPIO_PIN_1);
	}
	else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
	{
	  HAL_GPIO_EXTI_Callback(GPIO_PIN_0);
	}
	else
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	}
}

void EXTI4_15_IRQHandler(void)
{
  if (HAL_GPIO_ReadPin(TOUCH_INTER_PORT, TOUCH_INTER_PIN) == GPIO_PIN_RESET)
	{
		HAL_GPIO_EXTI_Callback(TOUCH_INTER_PIN);
	}
	else if (HAL_GPIO_ReadPin(FINTER_PORT, FINTER_PIN) == GPIO_PIN_SET)
	{
		HAL_GPIO_EXTI_Callback(FINTER_PIN);
	}
	else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == GPIO_PIN_RESET)
	{
		HAL_GPIO_EXTI_Callback(GPIO_PIN_12); //防撬开中断
	}
	else
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
		__HAL_GPIO_EXTI_CLEAR_IT(FINTER_PIN);
		__HAL_GPIO_EXTI_CLEAR_IT(TOUCH_INTER_PIN);
	}
}

/*
手指串口数据回调函数
*/
uint8_t finger_recv = 0;
uint8_t index_bb = 255;
uint8_t finger_index = 0;
uint8_t finger_bufer[48] = {0};

/*
WIFI串口数据回调函数
*/
uint8_t wifi_recv = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		if (HAL_UART_Receive_IT(huart, &wifi_recv, 1) != HAL_OK)
		{
			printf("串口1接收失败\n");
			Lusers.Initialization.Wifi();
			Lusers.Application.MSDelay(2);
			if (HAL_UART_Receive_IT(huart, &wifi_recv, 1) != HAL_OK)
			{
				
			}
		}
		uart_receive_input(wifi_recv);
	}
	if (huart->Instance == USART2)
	{
//		uint16_t finger_checek_sum = 0;
	  if (HAL_UART_Receive_IT(huart, &finger_recv, 1) != HAL_OK)
		{
			printf("数据接收失败\n");
			Lusers.Initialization.Finger_Uart();
			Lusers.Application.MSDelay(2);
			if (HAL_UART_Receive_IT(huart, &finger_recv, 1) != HAL_OK)
			{
				
			}
		}
		Finger_Input_Data(finger_recv); //每收到一个数据存入一位		
//		finger_bufer[finger_index++] = finger_recv;
//		if (finger_recv == 0xBB)
//		{
//			index_bb = finger_index - 1;
//		}
//		if (finger_index - index_bb >= 24) //收到一整包数据后统一处理
//		{
//			for (int i = index_bb; i < index_bb + 22; i++)
//			{
//				finger_checek_sum += finger_bufer[i];
//			}
//			if ((finger_bufer[index_bb + 22] == (finger_checek_sum & 0xFF)) && (finger_bufer[index_bb + 23] == ((finger_checek_sum >> 8) & 0xFF)))
//			{
//			  Finger_Recevie_Service(finger_bufer, index_bb);
//			}
//			finger_index = 0;
//			index_bb = 255;
//		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		if(__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE) != RESET) 
		{
			printf("uart1 errorcallback2");
			__HAL_UART_CLEAR_OREFLAG(huart);
			huart->gState = HAL_UART_STATE_READY;
		}
		HAL_UART_Receive_IT(huart, &wifi_recv, 1);		
	}
	else if (huart->Instance == USART2)
	{
		if(__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE) != RESET) 
		{
			printf("uart2 errorcallback2");
			__HAL_UART_CLEAR_OREFLAG(huart);
			huart->gState = HAL_UART_STATE_READY;
		}
		HAL_UART_Receive_IT(huart, &finger_recv, 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
	{
		if (Lusers.APMData.test_error_time > 0)
		{
			Lusers.APMData.test_error_time --;
			printf("Lusers.APMData.test_error_time = %d\n", Lusers.APMData.test_error_time);
			if (Lusers.APMData.test_error_time == 0)
			{
				Lusers.APMData.Show_Page = OLEDCLEAR;
			}
		}
		if (Lusers.APMData.APP_Open_Time > 0)
		{
			Lusers.APMData.APP_Open_Time --;
			printf("Lusers.APMData.APP_Open_Time = %d\n", Lusers.APMData.APP_Open_Time);
			if (Lusers.APMData.wifi_risi >= 5)
			{
				mcu_dp_value_update(DPID_UNLOCK_REQUEST, Lusers.APMData.APP_Open_Time, NOT_TIME_UPDATA);
			}			
			if (Lusers.APMData.APP_Open_Time == 0)
			{
				Lusers.APMData.Show_Page = OLEDCLEAR;
			}			
		}
		if ((Lusers.APMData.Indoor_time > 0) && (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0))
		{
			Lusers.Application.Voice_Play(0x10);
			Lusers.APMData.Indoor_time --;
			printf("Lusers.APMData.Indoor_time = %d\n", Lusers.APMData.Indoor_time);
		}
		else
		{
			Lusers.APMData.Indoor_time = 0;
		}
		if (Lusers.APMData.OLEDReturn > 60000)
		{
			Lusers.APMData.OLEDReturn = 0;
		}
		Lusers.APMData.OLEDReturn ++; //一秒钟加一次
	}
}

void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}



/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
