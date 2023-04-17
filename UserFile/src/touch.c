#include "touch.h"

#define TC_EN_PORT GPIOB
#define TC_EN_PIN  GPIO_PIN_4

#define TC_EN_RESET HAL_GPIO_WritePin(TC_EN_PORT, TC_EN_PIN, GPIO_PIN_RESET)
#define TC_EN_SET   HAL_GPIO_WritePin(TC_EN_PORT, TC_EN_PIN, GPIO_PIN_SET)

#define Touch_Sens(a, b)   0x##a##b

I2C_HandleTypeDef hi2c1;

void Touch_WriteData(uint8_t DAddress, uint8_t DData)
{
	HAL_I2C_Mem_Write(&hi2c1, 0xD0, DAddress, 1, &DData, 1, 0x10); //0xD0为芯片地址，DAddress为寄存器地址, data为发送至寄存器的数据
}

uint8_t Touch_ReadByte(uint8_t DAddress)
{
  uint8_t Data = 0;
	
	HAL_I2C_Master_Transmit(&hi2c1, 0xD0, &DAddress, 1, 0x10); //0xD0为芯片地址，DAddress为寄存器地址
	HAL_I2C_Master_Receive(&hi2c1, 0xD1, &Data, 1, 0x10);
	
	return Data;
}

void TSM12M_Check(void)
{
	TC_EN_RESET;
	delay_ms(10);
	
	Touch_WriteData(0x02, 0xAA);
	
	if (Touch_ReadByte(0x02) == 0xAA)
	{
		printf("TOUCH_OK!");
	}
	else
	{
		printf("TOUCH_ERROR!");
	}
}

void Touch_DeInit(void)
{
	TC_EN_RESET;
	delay_ms(10);	
	Touch_WriteData(0x09, 0x07);
}

void Touch_Inter_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = TC_EN_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(TC_EN_PORT, &GPIO_InitStruct);
	
	TC_EN_RESET;
//	
//	TSM12M_Check();
	
	Touch_WriteData(0x08,0x33);
	Touch_WriteData(0x09,0x0f);
	Touch_WriteData(0x09,0x07);//0x03
	
	Touch_WriteData(0x02,Touch_Sens(2, 0)); //key* & key0
	Touch_WriteData(0x03,Touch_Sens(2, 2)); //key# & key7
	Touch_WriteData(0x04,Touch_Sens(2, 2)); //key6 & key9
	Touch_WriteData(0x05,Touch_Sens(0, 2)); //key2 & key3
	Touch_WriteData(0x06,Touch_Sens(0, 2)); //key5 & key1
	Touch_WriteData(0x07,Touch_Sens(0, 2)); //key8 & key4	

	Touch_WriteData(0x08,0x18);
	Touch_WriteData(0x0a,0x00);
	Touch_WriteData(0x0b,0x00);
	Touch_WriteData(0x0c,0x00);
	Touch_WriteData(0x0d,0x00);
	
	GPIO_InitStruct.Pin   = TOUCH_INTER_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	
	HAL_GPIO_Init(TOUCH_INTER_PORT, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1 , 4); //2, 2
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);	
}

void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

void Touch_Init(void)
{
	MX_I2C1_Init();
	Touch_Inter_Init();
}	

uint8_t Key_Data = 13;

void Touch_ReadKey(uint8_t *Touch3_Bufer)
{
	uint8_t Td0 = 0, Td1 = 0, Td2 = 0;
	
	Td0 = Touch3_Bufer[0];
	Td1 = Touch3_Bufer[1];
	Td2 = Touch3_Bufer[2];
	
	if (Td0 != 0)
	{
		if ((Td0 & 0x0C) == 0x0C)
		{
			Key_Data = 10;
		}	
		else if ((Td0 & 0x03) == 0x03)
		{
			Key_Data = 0;
		}
		else if ((Td0 & 0x30) == 0x30)
		{
			Key_Data = 7;
		}
		else if ((Td0 & 0xC0) == 0XC0)
		{
			Key_Data = 12;
		}
//		else
//		{
//			Key_Data = 13;   //不打开此判断项，否则会导致按键数据还未处理完时已归位按键值
//		}
	}
	else if (Td1 != 0)
	{
		if ((Td1 & 0xC0) == 0xC0)
		{
			Key_Data = 2;
		}
		else if ((Td1 & 0x30) == 0x30)
		{
			Key_Data = 3;
		}
		else if ((Td1 & 0x0C) == 0x0C)
		{
			Key_Data = 6;
		}
		else if ((Td1 & 0x03) == 0x03)
		{
			Key_Data = 9;
		}
//		else
//		{
//			Key_Data = 13;
//		}
	}
	else if (Td2 != 0)
	{
		if ((Td2 & 0x03) == 0x03)
		{
			Key_Data = 1;
		}
		else if ((Td2 & 0x0C) == 0x0C)
		{
			Key_Data = 5;
		}
		else if ((Td2 & 0x30) == 0x30)
		{
			Key_Data = 4;
		}
		else if ((Td2 & 0xC0) == 0xC0)
		{
			Key_Data = 8;
		}
//		else
//		{
//			Key_Data = 13;
//		}
	}
//	else
//	{
//		Key_Data = 13;
//	}
//	if (Key_Data != 13)
//	{
//		Lusers.Application.Voice_Play(0x10);		
//	}
}


