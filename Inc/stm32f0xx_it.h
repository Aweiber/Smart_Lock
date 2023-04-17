/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0xx_IT_H
#define __STM32F0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define NOT_INTER     0
#define FINGER_INTER  1
#define TOUCH_INTER   2
#define NFC_INTER     3
#define RESET_INTER   4
#define PICK_INTER    5
	 
void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */
extern uint8_t Finger_Inter_IT;
extern uint8_t finger_recv;
extern uint8_t wifi_recv;
#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
