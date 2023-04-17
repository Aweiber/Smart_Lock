#include "voice.h"

#define VC_PORT GPIOB
#define VC_BUSY GPIO_PIN_13
#define VC_CLK  GPIO_PIN_14
#define VC_DATA GPIO_PIN_15

#define CLK_LOW   HAL_GPIO_WritePin(VC_PORT, VC_CLK, GPIO_PIN_RESET)
#define CLK_HIGH  HAL_GPIO_WritePin(VC_PORT, VC_CLK, GPIO_PIN_SET)

#define DATA_HIGH  HAL_GPIO_WritePin(VC_PORT, VC_DATA, GPIO_PIN_SET)
#define DATA_LOW   HAL_GPIO_WritePin(VC_PORT, VC_DATA, GPIO_PIN_RESET)

void WTN6_Gpio_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin  = VC_BUSY;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(VC_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin  = VC_CLK | VC_DATA;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(VC_PORT, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(VC_PORT, VC_CLK, GPIO_PIN_SET);
	HAL_GPIO_WritePin(VC_PORT, VC_DATA, GPIO_PIN_SET);
	
	WTN6_Send_Data(0xEF); //设置语音为最大
}

void WTN6_Send_Data(uint8_t data)
{
	CLK_LOW;
	delay_ms(5);
	for (int i = 0; i < 8; i++)
	{
		CLK_LOW;
		if (data & 0x01)
		{
			DATA_HIGH;
		}
		else
		{
			DATA_LOW;
		}
		delay_us(300);
		CLK_HIGH;
		delay_us(300);
		data = data >> 1;
	}
	DATA_HIGH;
	CLK_HIGH;
}

void WTN6_Play(uint8_t cmd)
{
	Lusers.APMData.OLEDReturn = 0;
	WTN6_Send_Data(cmd);
}
