#include "sys.h"

void Sys_Pick_Proof(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin  = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 4);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void INDoor_Reset_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin  = GPIO_PIN_1;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 2, 5);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void Low_Power_Gpio(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | /*GPIO_PIN_13 | GPIO_PIN_14 | */GPIO_PIN_15;
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = /*GPIO_PIN_0 | GPIO_PIN_1 | */GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |/* GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |*/ GPIO_PIN_8 | /*GPIO_PIN_9 |*/ GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin   = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7; 
	GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
//	__HAL_RCC_GPIOA_CLK_DISABLE();
//	__HAL_RCC_GPIOB_CLK_DISABLE();
//	__HAL_RCC_GPIOC_CLK_DISABLE();
//	__HAL_RCC_GPIOF_CLK_DISABLE();
}

