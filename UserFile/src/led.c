#include "led.h"

void LED_Init(void)
{
	__HAL_RCC_GPIOF_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = LED1_PIN | LED2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void LED_OnOff(uint8_t OnOff)
{
	if (OnOff == LED_ON)
	{
		HAL_GPIO_WritePin(LED_PORT, LED1_PIN | LED2_PIN, GPIO_PIN_SET);
	}
	else if (OnOff == LED_OFF)
	{
		HAL_GPIO_WritePin(LED_PORT, LED1_PIN | LED2_PIN, GPIO_PIN_RESET);
	}
}


