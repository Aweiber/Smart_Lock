#include "oled.h"
#include "oledfont.h"
#include "string.h"

#define OLED_PORT GPIOB
#define OLED_CS   GPIO_PIN_12
#define OLED_DC   GPIO_PIN_2
#define OLED_SCL  GPIO_PIN_10
#define OLED_SDA  GPIO_PIN_11

#define CS_HIGH  HAL_GPIO_WritePin(OLED_PORT, OLED_CS, GPIO_PIN_SET)
#define DC_HIGH  HAL_GPIO_WritePin(OLED_PORT, OLED_DC, GPIO_PIN_SET)
#define SCL_HIGH HAL_GPIO_WritePin(OLED_PORT, OLED_SCL, GPIO_PIN_SET)
#define SDA_HIGH HAL_GPIO_WritePin(OLED_PORT, OLED_SDA, GPIO_PIN_SET)

#define CS_LOW  HAL_GPIO_WritePin(OLED_PORT, OLED_CS, GPIO_PIN_RESET)
#define DC_LOW  HAL_GPIO_WritePin(OLED_PORT, OLED_DC, GPIO_PIN_RESET)
#define SCL_LOW HAL_GPIO_WritePin(OLED_PORT, OLED_SCL, GPIO_PIN_RESET)
#define SDA_LOW HAL_GPIO_WritePin(OLED_PORT, OLED_SDA, GPIO_PIN_RESET)

#define OLED_CMD  0
#define OLED_DATA 1

void OLED_WR_Byte(uint8_t data, uint8_t cmd)
{
	if (cmd)
	{
		DC_HIGH;
	}
	else
	{
		DC_LOW;
	}
	CS_LOW;
	for (uint8_t i = 0; i < 8; i++)
	{
		SCL_LOW;
		if (data & 0x80)
		{
			SDA_HIGH;
		}
		else
		{
			SDA_LOW;
		}
		SCL_HIGH;
		data = data << 1;
	}
	DC_HIGH;
	CS_HIGH;
}

void OLED_WR_Byte_Fill(uint8_t data, uint8_t cmd)
{
	if (cmd)
	{
		DC_HIGH;
	}
	else
	{
		DC_LOW;
	}
	CS_LOW;
	for (uint8_t i = 0; i < 8; i++)
	{
		SCL_LOW;
		if (data & 0x80)
		{
			SDA_LOW;
		}
		else
		{
			SDA_HIGH;
		}
		SCL_HIGH;
		data = data << 1;
	}
	DC_HIGH;
	CS_HIGH;
}

void OLED_Power_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = OLED_POWER_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(OLED_POWER_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin   = OLED_CS | OLED_DC | OLED_SCL | OLED_SDA;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	
	HAL_GPIO_Init(OLED_PORT, &GPIO_InitStruct);
	
  OLED_ON;
	delay_ms(300);
	CS_HIGH;
	SCL_HIGH;
	SDA_HIGH;
	DC_HIGH;
}

void OLED_Init(void)
{
	OLED_Power_Init();
	
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ�� D5
	OLED_WR_Byte(0x80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ�� 80
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA0,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;  0xa0���ҷ�  0xa1����
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� 0xc0���·�  0xc8����
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    		a6 a7 
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 	

	OLED_WR_Byte(0x8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0x14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0xAF,OLED_CMD);  //DISPLAY ON
	
  OLED_Clear();
}

//��������� y 0-7
void OLED_SetPos(unsigned char  x, unsigned char  y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)| 0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)| 0x01,OLED_CMD); 	
} 

void OLED_AllFill(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte(0x01,OLED_CMD);
		OLED_WR_Byte(0x10,OLED_CMD);
		for(x=0;x<128;x++)
		{
			OLED_WR_Byte(0xff,OLED_DATA);
		}
	}
}

void OLED_Clear(void)  
{  
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WR_Byte(0xb0+y,OLED_CMD);
		OLED_WR_Byte(0x01,OLED_CMD);
		OLED_WR_Byte(0x10,OLED_CMD);
		for(x=0;x<128;x++)
		{
			OLED_WR_Byte(0x00,OLED_DATA);
		}
	}
}

void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
 unsigned int j=0;
 unsigned char x,y;

  if(y1%8==0)
	{
		y=y1/8;
	}
  else
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			OLED_WR_Byte(BMP[j++],OLED_DATA);
		}
	}
}
/*****************************�����***************************************/
void OLED_ShowCHinese24(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ24*24����
{      		
	unsigned char wm=0;
	unsigned int adder=72*no;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 2);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
}

void OLED_Show_Chinese_24(unsigned char x, unsigned char y, unsigned char *chabufer)//�Ż���ʾ24*24����
{
	uint8_t lenth = 0;
	uint8_t input_lenth = 0;
	
	lenth = strlen((const char *)chinese_font_24);
	input_lenth = strlen((const char *)chabufer);
	
	lenth = lenth / 2;
	input_lenth = input_lenth / 2;
	
	for (uint8_t j = 0; j < input_lenth; j++)
	{
		for (uint8_t i = 0; i < lenth; i++)
		{
			if ((chinese_font_24[i*2] == *(chabufer + (2*j))) && (chinese_font_24[i*2 + 1] == *(chabufer + (2*j + 1))))
			{
				OLED_ShowCHinese24(x + (j*24), y, i);
				break;
			}
		}
  }
}

void OLED_Show_Chinese_24_Center(unsigned char y, unsigned char *chabufer)//X�������ʾ24*24����
{
	uint8_t x_center = 0;
	uint8_t input_lenth = 0;
	
	input_lenth = strlen((const char *)chabufer);
	input_lenth = input_lenth / 2;
	
	x_center = 64 - (12 * input_lenth);
	
	OLED_Show_Chinese_24(x_center, y, chabufer);
}

void OLED_ShowCHinese16(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ16*16����
{      		
	unsigned char wm=0;
	unsigned int adder=32*no;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(CHINESE16[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte(CHINESE16[adder],OLED_DATA);
		adder += 1;
	}
}

void OLED_Show_Chinese_16(unsigned char x, unsigned char y, unsigned char *chabufer)//�Ż���ʾ16*16����
{
	uint8_t lenth = 0;
	uint8_t input_lenth = 0;
	
	lenth = strlen((const char *)chinese_font_16);
	input_lenth = strlen((const char *)chabufer);
	
	lenth = lenth / 2;
	input_lenth = input_lenth / 2;
	
	for (uint8_t j = 0; j < input_lenth; j++)
	{
		for (uint8_t i = 0; i < lenth; i++)
		{
			if ((chinese_font_16[i*2] == *(chabufer + (2*j))) && (chinese_font_16[i*2 + 1] == *(chabufer + (2*j + 1))))
			{
				OLED_ShowCHinese16(x + (j*16), y, i);
				break;
			}
		}
  }
}

void OLED_Show_Chinese_16_Center(unsigned char y, unsigned char *chabufer)//������ʾ16*16����
{
	uint8_t x_center = 0;
	uint8_t input_lenth = 0;
	
	input_lenth = strlen((const char *)chabufer);
	input_lenth = input_lenth / 2;
	
	x_center = 64 - (8 * input_lenth);
	
	OLED_Show_Chinese_16(x_center, y, chabufer);
}

void OLED_Show_24Char(unsigned char x, unsigned y, unsigned char Data)
{
	uint8_t wm = 0;
	uint8_t data_num = 0;
	data_num = Data - 0x30;
	
	OLED_SetPos(x, y);
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte(oled_num24_bufer[data_num*36 + wm], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 1);	
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte(oled_num24_bufer[data_num*36 + wm + 12], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 2);
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte(oled_num24_bufer[data_num*36 + wm + 24], OLED_DATA);
	}
}

void OLED_Show_16Char(unsigned char x, unsigned y, unsigned char Data)
{
	uint8_t wm = 0;
	uint8_t data_num = 0;
	data_num = Data - 0x30;
	
	OLED_SetPos(x, y);
	for (wm = 0; wm < 8; wm ++)
	{
		OLED_WR_Byte(oled_num16_bufer[data_num*16 + wm], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 1);	
	for (wm = 0; wm < 8; wm ++)
	{
		OLED_WR_Byte(oled_num16_bufer[data_num*16 + wm + 8], OLED_DATA);
	}
}

/******************************�����*********************************/
void OLED_ShowCHinese24_Fill(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ24*24����
{      		
	unsigned char wm=0;
	unsigned int adder=72*no;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte_Fill(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte_Fill(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 2);
	for(wm = 0;wm < 24;wm++)
	{
		OLED_WR_Byte_Fill(CHINESE24[adder],OLED_DATA);
		adder += 1;
	}
}

void OLED_Show_Chinese_24_Fill(unsigned char x, unsigned char y, unsigned char *chabufer)//�Ż���ʾ24*24����
{
	uint8_t lenth = 0;
	uint8_t input_lenth = 0;
	
	lenth = strlen((const char *)chinese_font_24);
	input_lenth = strlen((const char *)chabufer);
	
	lenth = lenth / 2;
	input_lenth = input_lenth / 2;
	
	for (uint8_t j = 0; j < input_lenth; j++)
	{
		for (uint8_t i = 0; i < lenth; i++)
		{
			if ((chinese_font_24[i*2] == *(chabufer + (2*j))) && (chinese_font_24[i*2 + 1] == *(chabufer + (2*j + 1))))
			{
				OLED_ShowCHinese24_Fill(x + (j*24), y, i);
				break;
			}
		}
  }
}

void OLED_Show_Chinese_24_Center_Fill(unsigned char y, unsigned char *chabufer)//X�������ʾ24*24����
{
	uint8_t x_center = 0;
	uint8_t input_lenth = 0;
	
	input_lenth = strlen((const char *)chabufer);
	input_lenth = input_lenth / 2;
	
	x_center = 64 - (12 * input_lenth);
	
	OLED_Show_Chinese_24_Fill(x_center, y, chabufer);
}

void OLED_ShowCHinese16_Fill(unsigned char  x,unsigned char  y,unsigned char  no)//��ʾ16*16����
{      		
	unsigned char wm=0;
	unsigned int adder=32*no;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte_Fill(CHINESE16[adder],OLED_DATA);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WR_Byte_Fill(CHINESE16[adder],OLED_DATA);
		adder += 1;
	}
}

void OLED_Show_Chinese_16_Fill(unsigned char x, unsigned char y, unsigned char *chabufer)//�Ż���ʾ16*16����
{
	uint8_t lenth = 0;
	uint8_t input_lenth = 0;
	
	lenth = strlen((const char *)chinese_font_16);
	input_lenth = strlen((const char *)chabufer);
	
	lenth = lenth / 2;
	input_lenth = input_lenth / 2;
	
	for (uint8_t j = 0; j < input_lenth; j++)
	{
		for (uint8_t i = 0; i < lenth; i++)
		{
			if ((chinese_font_16[i*2] == *(chabufer + (2*j))) && (chinese_font_16[i*2 + 1] == *(chabufer + (2*j + 1))))
			{
				OLED_ShowCHinese16_Fill(x + (j*16), y, i);
				break;
			}
		}
  }
}

void OLED_Show_Chinese_16_Center_Fill(unsigned char y, unsigned char *chabufer)//������ʾ16*16����
{
	uint8_t x_center = 0;
	uint8_t input_lenth = 0;
	
	input_lenth = strlen((const char *)chabufer);
	input_lenth = input_lenth / 2;
	
	x_center = 64 - (8 * input_lenth);
	
	OLED_Show_Chinese_16_Fill(x_center, y, chabufer);
}

void OLED_Show_24Char_Fill(unsigned char x, unsigned y, unsigned char Data)
{
	uint8_t wm = 0;
	uint8_t data_num = 0;
	data_num = Data - 0x30;
	
	OLED_SetPos(x, y);
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte_Fill(oled_num24_bufer[data_num*36 + wm], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 1);	
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte_Fill(oled_num24_bufer[data_num*36 + wm + 12], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 2);
	for (wm = 0; wm < 12; wm ++)
	{
		OLED_WR_Byte_Fill(oled_num24_bufer[data_num*36 + wm + 24], OLED_DATA);
	}
}

void OLED_Show_16Char_Fill(unsigned char x, unsigned y, unsigned char Data)
{
	uint8_t wm = 0;
	uint8_t data_num = 0;
	data_num = Data - 0x30;
	
	OLED_SetPos(x, y);
	for (wm = 0; wm < 8; wm ++)
	{
		OLED_WR_Byte_Fill(oled_num16_bufer[data_num*16 + wm], OLED_DATA);
	}
	
	OLED_SetPos(x, y + 1);	
	for (wm = 0; wm < 8; wm ++)
	{
		OLED_WR_Byte_Fill(oled_num16_bufer[data_num*16 + wm + 8], OLED_DATA);
	}
}


extern uint16_t GetPowerData;

void Show_Baterry(void)
{
	if (GetPowerData > 2540)
	{
		OLED_Show_Chinese_16(111, 0, "��");
	}
	else if ((GetPowerData > 2360) && (GetPowerData <= 2540))
	{
		OLED_Show_Chinese_16(111, 0, "��");
	}
	else if ((GetPowerData > 2230) && (GetPowerData <= 2360))
	{
		OLED_Show_Chinese_16(111, 0, "��");
	}
	else if ((GetPowerData > 2170) && (GetPowerData <= 2230))
	{
		OLED_Show_Chinese_16(111, 0, "Ҽ");
	}
	else if ((GetPowerData > 2120) && (GetPowerData <= 2170))
	{
		OLED_Show_Chinese_16(111, 0, "��");
	}
	else
	{
		printf("��������\n");
		OLED_Show_Chinese_16(111, 0, "��");
	}
}

void Show_Wifi_Risi(void)
{
	if (Lusers.APMData.wifi_risi >= 75)
	{
		OLED_ShowCHinese16(0, 0, 0x44);
	}
	else if ((Lusers.APMData.wifi_risi >= 50) && (Lusers.APMData.wifi_risi < 75))
	{
		OLED_ShowCHinese16(0, 0, 0x43);
	}
	else if ((Lusers.APMData.wifi_risi >= 25) && (Lusers.APMData.wifi_risi < 50))
	{
		OLED_ShowCHinese16(0, 0, 0x42);
	}
	else if ((Lusers.APMData.wifi_risi >= 5) && (Lusers.APMData.wifi_risi < 25))
	{
		OLED_ShowCHinese16(0, 0, 0x41);
	}
	else
	{
		OLED_ShowCHinese16(0, 0, 0x40);
	}
}

#define OLDLOWMONTH ((Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3] > 7) ? (((Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3]) % 2 == 0) ? 0 : 1) : (((Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3]) % 2 == 1) ? 0 : 1))
//��С���жϣ�0Ϊ���£�1ΪС��
#define LEAPYEARSTATE (((Lusers.APMData.time_bufer[0]*10 + Lusers.APMData.time_bufer[1])%4 == 0) ? 0 : 1)
//�����жϣ�0Ϊ���꣬1Ϊ������

uint8_t Last_Show_Data = 0;
void OLED_Show_Play(uint8_t show_data)
{
	if (show_data != Last_Show_Data)
	{
		OLED_Clear();
		if (show_data == OLEDCLEAR)
		{
			OLED_Clear();
		}
		else if (show_data == OLEDSTARTUP)
		{
			OLED_DrawBMP(0, 0, 128, 8, gImage_logo1);
		}
		else if (show_data == OLEDVFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(3,"��֤ʧ��");
		}
		else if (show_data == OLEDVSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(5,"��֤ͨ��");
			if (Lusers.APMData.Open_Finger_Userc <= 5)
			{
				OLED_Show_Chinese_16(32, 3, "��ָ");
			}
			else
			{
				OLED_Show_Chinese_16(32, 3, "��ָ");
			}
			OLED_Show_16Char(64, 3, 0x3F);
			OLED_Show_16Char(72, 3, (Lusers.APMData.Open_Finger_Userc)/100 + 0x30);
			OLED_Show_16Char(80, 3, (Lusers.APMData.Open_Finger_Userc)%100/10 + 0x30);
			OLED_Show_16Char(88, 3, (Lusers.APMData.Open_Finger_Userc)%100%10 + 0x30);
		}
		else if (show_data == OLEDADDFINGER)
		{
			OLED_Show_Chinese_16_Center(0,"�����ָ");
			OLED_Show_Chinese_24_Center(3,"�����ָ");		
		}
		else if (show_data == OLEDADDFINGERAGING)
		{
			OLED_Show_Chinese_16_Center(0,"�����ָ");
			OLED_Show_Chinese_24_Center(3,"����¼һ��");		
		}
		else if (show_data == OLEDADDFINGERSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"�����ָ");
			OLED_Show_Chinese_24_Center(3,"��ӳɹ�");			
		}
		else if (show_data == OLEDADDFINGERFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"�����ָ");
			OLED_Show_Chinese_24_Center(3,"���ʧ��");			
		}
		else if (show_data == OLEDDELETEFINGERSUCC)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ����ָ");
			OLED_Show_Chinese_24_Center(3,"ɾ���ɹ�");			
		}
		else if (show_data == OLEDDELETEFINGERFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ����ָ");
			OLED_Show_Chinese_24_Center(3,"ɾ��ʧ��");			
		}		
		else if (show_data == OLEDFACTORYSUCCESS)
		{
			OLED_Show_Chinese_24_Center(0,"�ָ�����");
      OLED_Show_Chinese_24_Center(4,"�ɹ�");			
		}
		else if (show_data == OLEDFACTORYFAIL)
		{
			OLED_Show_Chinese_24_Center(0,"�ָ�����");
      OLED_Show_Chinese_24_Center(4,"ʧ��");
		}
		else if (show_data == OLEDLOWPOWER)
		{
			Lusers.Application.Voice_Play(0x11);
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(2,"������");
      OLED_Show_Chinese_24_Center(5,"��������");
		}
		else if (show_data == OLEDADMIN)
		{
			Lusers.Application.ONOFFLed(LED_ON);			
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(3,"��֤����Ա");
		}
		else if (show_data == OLEDMENU1)
		{
			LED2_OFF;
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"��ָ����");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"��������");
		}
		else if (show_data == OLEDMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"��ָ����");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"��������");
		}
		else if (show_data == OLEDMENU3)
		{
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char_Fill(4, 2, '3');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"��Ƭ����");
			OLED_Show_24Char(4, 5, '4');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"ϵͳ����");
		}
		else if (show_data == OLEDMENU4)
		{
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char(4, 2, '3');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"��Ƭ����");
			OLED_Show_24Char_Fill(4, 5, '4');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"ϵͳ����");
		}
		else if (show_data == OLEDMENU5)
		{
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char_Fill(4, 2, '5');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"��¼��ѯ");
			OLED_Show_24Char(4, 5, '6');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"Զ������");
		}
		else if (show_data == OLEDMENU6)
		{
			OLED_Show_Chinese_16_Center(0,"�˵�");
			OLED_Show_24Char(4, 2, '5');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"��¼��ѯ");
			OLED_Show_24Char_Fill(4, 5, '6');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"Զ������");
		}
		else if (show_data == OLEDFINGERMENU1)
		{
			OLED_Show_Chinese_16_Center(0,"��ָ����");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"�����ָ");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"ɾ����ָ");
		}
		else if (show_data == OLEDFINGERMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"��ָ����");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"�����ָ");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"ɾ����ָ");
		}
		else if (show_data == OLEDWORDMENU1)
		{
			OLED_Show_Chinese_16_Center(0,"��������");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"�������");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"ɾ������");
		}
		else if (show_data == OLEDWORDMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"��������");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"�������");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"ɾ������");
		}
		else if (show_data == OLEDNFCMENU1)
		{
			OLED_Show_Chinese_16_Center(0,"��Ƭ����");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"��ӿ�Ƭ");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"ɾ����Ƭ");
		}
		else if (show_data == OLEDNFCMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"��Ƭ����");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"��ӿ�Ƭ");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"ɾ����Ƭ");
		}
		else if (show_data == OLEDSYSTEMMENU1)
		{
			OLED_Show_Chinese_16_Center(0,"ϵͳ����");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"����ʱ��");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"��������");
		}
		else if (show_data == OLEDSYSTEMMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"ϵͳ����");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"����ʱ��");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"��������");
		}
		else if (show_data == OLEDSYSTEMMENU3)
		{
			OLED_Show_Chinese_16_Center(0,"ϵͳ����");
			OLED_Show_24Char_Fill(4, 2, '3');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"���ŷ�ʽ");
			OLED_Show_24Char(4, 5, '4');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"�ָ�����");
		}
		else if (show_data == OLEDSYSTEMMENU4)
		{
			OLED_Show_Chinese_16_Center(0,"ϵͳ����");
			OLED_Show_24Char(4, 2, '3');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"���ŷ�ʽ");
			OLED_Show_24Char_Fill(4, 5, '4');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"�ָ�����");
		}				
		else if (show_data == OLEDNOTHAVEFINGER)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ����ָ");
      OLED_Show_Chinese_24_Center(3,"��");
		}
		else if (show_data == OLEDNOTHAVEPWD)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ������");
      OLED_Show_Chinese_24_Center(3,"��");
		}
		else if (show_data == OLEDNOTHAVENFC)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ����Ƭ");
      OLED_Show_Chinese_24_Center(3,"��");
		}
		else if (show_data == OLEDSETOPENONLY)
		{
			OLED_Show_Chinese_16_Center(0,"���ŷ�ʽ");
			OLED_Show_Chinese_24_Center_Fill(2, "��һ��ʽ");
			OLED_Show_Chinese_24_Center(5, "��Ϸ�ʽ");
		}
		else if (show_data == OLEDSETOPENCONMBO)
		{
			OLED_Show_Chinese_16_Center(0,"���ŷ�ʽ");
			OLED_Show_Chinese_24_Center(2, "��һ��ʽ");
			OLED_Show_Chinese_24_Center_Fill(5, "��Ϸ�ʽ");			
		}
		else if (show_data == OLEDFACTORYMENU)
		{
			OLED_Show_Chinese_16_Center(0,"ϵͳ����");
      OLED_Show_Chinese_24_Center(2,"�ָ�����");
			OLED_Show_Chinese_16(0,6,"��");
			OLED_Show_16Char(16,6,0x3B);
			OLED_Show_16Char(24,6,0x3C);
			OLED_Show_16Char(32,6,0x3E);
			OLED_Show_Chinese_16(88,6,"��");
			OLED_Show_16Char(104,6,0x3B);
			OLED_Show_16Char(112,6,0x3D);
			OLED_Show_16Char(120,6,0x3E);
		}
		else if (show_data == OLEDWAITFACTORY)
		{
      OLED_Show_Chinese_24_Center(0,"�ָ�����");
			OLED_Show_Chinese_24_Center(4,"��ȴ�");
		}
		else if (show_data == OLEDPWDNOTLENTH)
		{
			OLED_Show_Chinese_16_Center(0,"�������");
			OLED_Show_Chinese_24_Center(2,"������");
			OLED_Show_Chinese_24_Center(5,"��λ����");
		}
		else if (show_data == OLEDPWDDISHAVE)
		{
			OLED_Show_Chinese_16_Center(0,"�������");
			OLED_Show_Chinese_24_Center(3,"���벻ͬ");
		}
		else if (show_data == OLEDADDPWDSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"�������");
			OLED_Show_Chinese_24_Center(3,"��ӳɹ�");
		}
		else if (show_data == OLEDPWDISHAVE)
		{
			OLED_Show_Chinese_16_Center(0,"�������");
			OLED_Show_Chinese_24_Center(3,"����ظ�");
		}
		else if (show_data == OLEDPWDISHAVE)
		{
			OLED_Show_Chinese_16_Center(0,"�������");
			OLED_Show_Chinese_24_Center(3,"����ظ�");
		}
		else if (show_data == OLEDPWDPASS)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(5,"��֤ͨ��");
			if (Lusers.APMData.Open_Pwd_Userc <= 2)
			{
				OLED_Show_Chinese_16(36, 3, "����");
			}
			else
			{
				OLED_Show_Chinese_16(36, 3, "����");
			}
			OLED_Show_16Char(68, 3, 0x3F);
			OLED_Show_16Char(76, 3, (Lusers.APMData.Open_Pwd_Userc)/10 + 0x30);
			OLED_Show_16Char(84, 3, (Lusers.APMData.Open_Pwd_Userc)%10 + 0x30);
		}
		else if (show_data == OLEDDELETEPWDSUCC)
		{
			OLED_Show_Chinese_16_Center(0,"ɾ������");
			OLED_Show_Chinese_24_Center(3,"ɾ���ɹ�");
		}
		else if (show_data == OLEDSETTIMESUCCESS)
		{
			printf("ʱ�����óɹ�\n");
			OLED_Show_Chinese_16_Center(0,"����ʱ��");
			OLED_Show_Chinese_24_Center(3,"���óɹ�");
		}
		else if (show_data == OLEDSETAUDIOSUCCESS)
		{
			printf("�������óɹ�\n");
			OLED_Show_Chinese_16_Center(0,"��������");
			OLED_Show_Chinese_24_Center(3,"���óɹ�");
		}
		else if (show_data == OLEDSETOPENSUCCESS)
		{
			printf("���ŷ�ʽ���óɹ�\n");
			OLED_Show_Chinese_16_Center(0,"���ŷ�ʽ");
			OLED_Show_Chinese_24_Center(3,"���óɹ�");
		}
		else if (show_data == OLEDOPENPLEASEFINGER)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(3,"�����ָ");	
		}
		else if (show_data == OLEDUSERRECORD)
		{
			OLED_Show_Chinese_16_Center(0,"��¼��ѯ");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"�û���ѯ");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"���ż�¼");
		}
		else if (show_data == OLEDOPENRECORD)
		{
			OLED_Show_Chinese_16_Center(0,"��¼��ѯ");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"�û���ѯ");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"���ż�¼");
		}
		else if (show_data == OLEDUSERREGISTER)
		{
			OLED_Show_Chinese_16_Center(0,"�û���ѯ");
			OLED_Show_Chinese_16(16,2,"��ָ");
			OLED_Show_16Char(48, 2, 0x3F);
			OLED_Show_16Char(56, 2, (flash_list.finger_user_num)/100 + 0x30);
			OLED_Show_16Char(64, 2, (flash_list.finger_user_num)%100/10 + 0x30);
			OLED_Show_16Char(72, 2, (flash_list.finger_user_num)%100%10 + 0x30);
			OLED_Show_16Char(80, 2, 0x3A);
			OLED_Show_16Char(88, 2, '1');
			OLED_Show_16Char(96, 2, '0');
			OLED_Show_16Char(104, 2, '0');
			OLED_Show_Chinese_16(24,4,"����");		
			OLED_Show_16Char(56, 4, 0x3F);
			OLED_Show_16Char(64, 4, (flash_list.pwd_user_num)/10 + 0x30);
			OLED_Show_16Char(72, 4, (flash_list.pwd_user_num)%10 + 0x30);
			OLED_Show_16Char(80, 4, 0x3A);
			OLED_Show_16Char(88, 4, '1');
			OLED_Show_16Char(96, 4, '0');
			OLED_Show_Chinese_16(24,6,"��Ƭ");		
			OLED_Show_16Char(56, 6, 0x3F);
			OLED_Show_16Char(64, 6, (flash_list.nfc_user_num)/10 + 0x30);
			OLED_Show_16Char(72, 6, (flash_list.nfc_user_num)%10 + 0x30);
			OLED_Show_16Char(80, 6, 0x3A);
			OLED_Show_16Char(88, 6, '8');
			OLED_Show_16Char(96, 6, '0');
		}
		else if (show_data == OLEDFINGERFULLING)
		{
			OLED_Show_Chinese_16_Center(0, "�����ָ");
			OLED_Show_Chinese_24_Center(3, "��ָ����");
		}
		else if (show_data == OLEDPWDFULLING)
		{
			OLED_Show_Chinese_16_Center(0, "�������");
			OLED_Show_Chinese_24_Center(3, "��������");			
		}
		else if (show_data == OLEDNFCFULLING)
		{
			OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
			OLED_Show_Chinese_24_Center(3, "��Ƭ����");
		}
		else if (show_data == OLEDNOTHAVERECORD)
		{
			OLED_Show_Chinese_16_Center(0, "��¼��ѯ");
			OLED_Show_Chinese_24_Center(3, "��");
		}
		else if (show_data == OLEDSYSPICKPROOF)
		{
			OLED_Show_Chinese_24_Center(2, "ϵͳ����");
		}
		else if (show_data == OLEDADDNFCDOING)
		{
			OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
			OLED_Show_Chinese_24_Center(3, "��ˢ��");
		}
		else if (show_data == OLEDADDNFCFAIL)
		{
			OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
			OLED_Show_Chinese_24_Center(3, "���ʧ��");
		}
		else if (show_data == OLEDADDNFCSAME)
		{
			OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
			OLED_Show_Chinese_24_Center(3, "����ظ�");
		}
		else if (show_data == OLEDADDNFCSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
			OLED_Show_Chinese_24_Center(3, "��ӳɹ�");
		}
		else if (show_data == OLEDDELETENFCSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0, "ɾ����Ƭ");
			OLED_Show_Chinese_24_Center(3, "ɾ���ɹ�");			
		}
		else if (show_data == OLEDVNFCSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(5,"��֤ͨ��");
			OLED_Show_Chinese_16(36, 3, "��Ƭ");
			OLED_Show_16Char(68, 3, 0x3F);
			OLED_Show_16Char(76, 3, (Lusers.APMData.Open_Nfc_Userc)/10 + 0x30);
			OLED_Show_16Char(84, 3, (Lusers.APMData.Open_Nfc_Userc)%10 + 0x30);			
		}
		else if (show_data == OLEDNOTHAVEADMIN)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_24_Center(3,"����ģʽ");
		}
		else if (show_data == OLEDAPPMENU1)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_24Char_Fill(4, 2, '1');
			OLED_Show_24Char_Fill(16, 2, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 2,"Զ�����");
			OLED_Show_24Char(4, 5, '2');
			OLED_Show_24Char(16, 5, 0x3A);
			OLED_Show_Chinese_24(28, 5,"Զ�̹ر�");			
		}
		else if (show_data == OLEDAPPMENU2)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_24Char(4, 2, '1');
			OLED_Show_24Char(16, 2, 0x3A);
			OLED_Show_Chinese_24(28, 2,"Զ�����");
			OLED_Show_24Char_Fill(4, 5, '2');
			OLED_Show_24Char_Fill(16, 5, 0x3A);
			OLED_Show_Chinese_24_Fill(28, 5,"Զ�̹ر�");						
		}
		else if (show_data == OLEDAPPPAIRSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_Chinese_24_Center(3,"��Գɹ�");
		}
		else if (show_data == OLEDAPPPAIRFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_Chinese_24_Center(3,"���ʧ��");
		}
		else if (show_data == OLEDAPPPAIR)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_Chinese_24_Center(3, "��ȴ�");
		}
		else if (show_data == OLEDAPPOPENSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"Զ�̿���");
			OLED_Show_Chinese_24_Center(3,"��֤ͨ��");
		}
		else if (show_data == OLEDAPPOPENSUCCESS)
		{
			OLED_Show_Chinese_16_Center(0,"Զ�̿���");
			OLED_Show_Chinese_24_Center(3,"��֤ʧ��");
		}
		else if (show_data == OLEDDYMANICPASS)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_16_Center(3,"��̬����");
			OLED_Show_Chinese_24_Center(5,"��֤ͨ��");
		}
		else if (show_data == OLEDDYMANICFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_16_Center(3,"��̬����");
			OLED_Show_Chinese_24_Center(5,"��֤ʧ��");
		}
		else if (show_data == OLEDTEMPORARYWAIT)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_16_Center(3,"��ʱ����");
      OLED_Show_Chinese_24_Center(5,"��ȴ�");
		}
		else if (show_data == OLEDTEMPORARYPASS)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_16_Center(3,"��ʱ����");
			OLED_Show_Chinese_24_Center(5,"��֤ͨ��");
		}
		else if (show_data == OLEDTEMPORARYFAIL)
		{
			OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
			OLED_Show_Chinese_16_Center(3,"��ʱ����");
			OLED_Show_Chinese_24_Center(5,"��֤ʧ��");
		}
		else if (show_data == OLEDCLOSEAPPPAIR)
		{
			OLED_Show_Chinese_16_Center(0,"Զ������");
			OLED_Show_Chinese_24_Center(3,"������ģ��");			
		}
  }
	if (show_data == OLEDMAIN)
	{
		Lusers.Application.ONOFFFinger(FINGER_POWER_ON);
		Lusers.Application.ONOFFLed(LED_ON);
		Show_Baterry();
		OLED_Show_Chinese_16_Center(0,"��ӭ�ؼ�");
		Lusers.Application.GetTime();
		OLED_Show_24Char(16, 2, (hhms.Hours)/10 + 0x30);
		OLED_Show_24Char(28, 2, (hhms.Hours)%10 + 0x30);
		OLED_Show_24Char(40, 2, 0x3B);
		OLED_Show_24Char(52, 2, (hhms.Minutes)/10 + 0x30);
		OLED_Show_24Char(64, 2, (hhms.Minutes)%10 + 0x30);
		OLED_Show_24Char(76, 2, 0x3B);
		OLED_Show_24Char(88, 2, (hhms.Seconds)/10 + 0x30);
		OLED_Show_24Char(100, 2, (hhms.Seconds)%10 + 0x30);
		
		OLED_Show_16Char(24, 6, '2');
		OLED_Show_16Char(32, 6, '0');
		OLED_Show_16Char(40, 6, (hymd.Year)/10 + 0x30);
		OLED_Show_16Char(48, 6, (hymd.Year)%10 + 0x30);
		OLED_Show_16Char(56, 6, 0x41);
		OLED_Show_16Char(64, 6, (hymd.Month)/10 + 0x30);
		OLED_Show_16Char(72, 6, (hymd.Month)%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x41);
		OLED_Show_16Char(88, 6, (hymd.Date)/10 + 0x30);
		OLED_Show_16Char(96, 6, (hymd.Date)%10 + 0x30);
		Show_Wifi_Risi();
	}
	else if (show_data == OLEDADDFINGERMENU1)
	{
		OLED_Show_Chinese_16_Center(0, "�����ָ");
		OLED_Show_Chinese_16(16,6,"���");
		OLED_Show_16Char(48, 6, 0x3F);
		OLED_Show_16Char(56, 6, (Lusers.APMData.Finger_Userc + 1)/100 + 0x30);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Finger_Userc + 1)%100/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Finger_Userc + 1)%100%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '1');
		OLED_Show_16Char(96, 6, '0');
		OLED_Show_16Char(104, 6, '0');
		if (Lusers.APMData.Finger_Userc < 5)
		{
		  OLED_Show_Chinese_16_Center(3, "����Ա");
		}
		else
		{
			OLED_Show_Chinese_16_Center(3, "���û���");
		}
		
	}
	else if (show_data == OLEDDELETEFINGERMENU)
	{
		OLED_Show_Chinese_16_Center(0, "ɾ����ָ");
		OLED_Show_Chinese_16(16,6,"���");		
		OLED_Show_16Char(48, 6, 0x3F);
		OLED_Show_16Char(56, 6, (Lusers.APMData.Finger_Userc + 1)/100 + 0x30);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Finger_Userc + 1)%100/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Finger_Userc + 1)%100%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '1');
		OLED_Show_16Char(96, 6, '0');
		OLED_Show_16Char(104, 6, '0');
		if (Lusers.APMData.Finger_Userc < 5)
		{
		  OLED_Show_Chinese_16_Center(3, "����Ա");
		}
		else
		{
			OLED_Show_Chinese_16_Center(3, "���û���");
		}
	}
	else if (show_data == OLEDADDPWDMENU1)
	{
		OLED_Show_Chinese_16_Center(0, "�������");
		OLED_Show_Chinese_16(24,6,"���");		
		OLED_Show_16Char(56, 6, 0x3F);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Pwd_Userc + 1)/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Pwd_Userc + 1)%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '1');
		OLED_Show_16Char(96, 6, '0');
		if (Lusers.APMData.Pwd_Userc < 2)
		{
		  OLED_Show_Chinese_16_Center(3, "����Ա");
		}
		else
		{
			OLED_Show_Chinese_16_Center(3, "���û���");
		}
	}
	else if (show_data == OLEDDELETEFPWDMENU)
	{
		OLED_Show_Chinese_16_Center(0, "ɾ������");
		OLED_Show_Chinese_16(24,6,"���");		
		OLED_Show_16Char(56, 6, 0x3F);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Pwd_Userc + 1)/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Pwd_Userc + 1)%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '1');
		OLED_Show_16Char(96, 6, '0');
		if (Lusers.APMData.Pwd_Userc < 2)
		{
		  OLED_Show_Chinese_16_Center(3, "����Ա");
		}
		else
		{
			OLED_Show_Chinese_16_Center(3, "���û���");
		}
	}
	else if (show_data == OLEDADDNFCMENU1)
	{
		OLED_Show_Chinese_16_Center(0, "��ӿ�Ƭ");
		OLED_Show_Chinese_16(24,6,"���");		
		OLED_Show_16Char(56, 6, 0x3F);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Nfc_Userc + 1)/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Nfc_Userc + 1)%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '8');
		OLED_Show_16Char(96, 6, '0');
		OLED_Show_Chinese_16_Center(3, "�û�");
	}
	else if (show_data == OLEDDELETEFNFCMENU)
	{
		OLED_Show_Chinese_16_Center(0, "ɾ����Ƭ");
		OLED_Show_Chinese_16(24,6,"���");		
		OLED_Show_16Char(56, 6, 0x3F);
		OLED_Show_16Char(64, 6, (Lusers.APMData.Nfc_Userc + 1)/10 + 0x30);
		OLED_Show_16Char(72, 6, (Lusers.APMData.Nfc_Userc + 1)%10 + 0x30);
		OLED_Show_16Char(80, 6, 0x3A);
		OLED_Show_16Char(88, 6, '8');
		OLED_Show_16Char(96, 6, '0');
		OLED_Show_Chinese_16_Center(3, "�û�");
	}
	else if (show_data == OLEDINPUTPWD)
	{
		OLED_Show_Chinese_16_Center(0, "��ӭ�ؼ�");
		OLED_Show_Chinese_16_Center(3, "��������");
		
		if (Lusers.APMData.Pwd_Index == 1)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				OLED_Show_16Char(32 + 8*i, 6, 0x40);
			}
		}
		else if (Lusers.APMData.Pwd_Index == 0)
		{
			for (uint8_t i = 0; i < 10; i++)
			{
				OLED_Show_16Char(24 + 8*i, 6, 0x40);
			}
		}
		if (Lusers.APMData.Pwd_Index <= 10)
		{
			for (uint8_t i = 0; i < Lusers.APMData.Pwd_Index; i++)
			{
				OLED_Show_16Char(24 + 8*(Lusers.APMData.Pwd_Index - 1), 6, 0x3C);
			}
	  }
	}
	else if (show_data == OLEDADMININPUTPWD)
	{
		OLED_Show_Chinese_16_Center(0, "��֤����Ա");
		OLED_Show_Chinese_16_Center(3, "��������");
		
		if (Lusers.APMData.Pwd_Index == 1)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				OLED_Show_16Char(32 + 8*i, 6, 0x40);
			}
		}
		else if (Lusers.APMData.Pwd_Index == 0)
		{
			for (uint8_t i = 0; i < 10; i++)
			{
				OLED_Show_16Char(24 + 8*i, 6, 0x40);
			}
		}
		if (Lusers.APMData.Pwd_Index <= 10)
		{
			for (uint8_t i = 0; i < Lusers.APMData.Pwd_Index; i++)
			{
				OLED_Show_16Char(24 + 8*(Lusers.APMData.Pwd_Index - 1), 6, 0x3C);
			}
	  }
	}
	else if (show_data == OLEDINPUTADDPWD)
	{
		OLED_Show_Chinese_16_Center(0, "�������");
		OLED_Show_Chinese_16_Center(3, "��������");
		for (uint8_t i = 0; i < 10; i++)
		{
			OLED_Show_16Char(24 + 8*i, 6, 0x40);
		}
	}
	else if (show_data == OLEDINPUTADDPWDIN)
	{
		OLED_Show_Chinese_16_Center(0, "�������");
		OLED_Show_Chinese_16_Center(3, "��������");
		
		if (Lusers.APMData.Input_PwdIndex == 1)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				OLED_Show_16Char(32 + 8*i, 6, 0x40);
			}
		}
		else if (Lusers.APMData.Input_PwdIndex == 0)
		{
			for (uint8_t i = 0; i < 10; i++)
			{
				OLED_Show_16Char(24 + 8*i, 6, 0x40);
			}
		}
		if (Lusers.APMData.Input_PwdIndex <= 11)
		{
			for (uint8_t i = 0; i < Lusers.APMData.Input_PwdIndex; i++)
			{
				OLED_Show_16Char(24 + 8*(Lusers.APMData.Input_PwdIndex - 1), 6, Lusers.APMData.Input_PwdBufer[Lusers.APMData.Input_PwdIndex - 1]);
			}
	  }
	}
	else if (show_data == OLEDCONFIRMPWD)
	{
		OLED_Show_Chinese_16_Center(0, "�������");
		OLED_Show_Chinese_16_Center(3, "ȷ������");
		for (uint8_t i = 0; i < 10; i++)
		{
			OLED_Show_16Char(24 + 8*i, 6, 0x40);
		}
	}
	else if (show_data == OLEDCONFIRMPWDIN)
	{
		OLED_Show_Chinese_16_Center(0, "�������");
		OLED_Show_Chinese_16_Center(3, "ȷ������");
		
		if (Lusers.APMData.Confirm_PwdIndex == 1)
		{
			for (uint8_t i = 0; i < 9; i++)
			{
				OLED_Show_16Char(32 + 8*i, 6, 0x40);
			}
		}
		else if (Lusers.APMData.Confirm_PwdIndex == 0)
		{
			for (uint8_t i = 0; i < 10; i++)
			{
				OLED_Show_16Char(24 + 8*i, 6, 0x40);
			}
		}
		if (Lusers.APMData.Confirm_PwdIndex <= 11)
		{
			for (uint8_t i = 0; i < Lusers.APMData.Confirm_PwdIndex; i++)
			{
				OLED_Show_16Char(24 + 8*(Lusers.APMData.Confirm_PwdIndex - 1), 6, Lusers.APMData.Confirm_PwdBufer[Lusers.APMData.Confirm_PwdIndex - 1]);
			}
	  }
	}
	else if (show_data == OLEDSETTIMEMENU)
	{
    OLED_Show_Chinese_16_Center(0, "����ʱ��");
		OLED_Show_Chinese_16(12, 2, "����");
		OLED_Show_Chinese_16(12, 4, "ʱ��");
		
		//������ʱ��������ǿ�����жϣ����·ݴ���12��ǿ����Ϊ12��Сʱ����23ǿ����Ϊ23�����Ӽ����ٴ���59ǿ����Ϊ59
		//���꼰��С���жϣ�����31�죬С��30�졣�����2��29�죬������2��28��
		if (Lusers.APMData.time_bufer[2] > 1)
		{
			Lusers.APMData.time_bufer[2] = 1;
		}
		else if ((Lusers.APMData.time_bufer[2] == 1) && (Lusers.APMData.time_bufer[3] > 2))
		{
			Lusers.APMData.time_bufer[3] = 2;
		}
		else if ((Lusers.APMData.time_bufer[2] == 0) && (Lusers.APMData.time_bufer[3] == 0))
		{
			Lusers.APMData.time_bufer[3] = 1;
		}
		else if ((Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3] == 2) && (Lusers.APMData.time_bufer[4] > 2))
		{
			Lusers.APMData.time_bufer[4] = 2;
		}
		else if (Lusers.APMData.time_bufer[4] > 3)
		{
			Lusers.APMData.time_bufer[4] = 3;
		}
		else if ((LEAPYEARSTATE == 1) && (Lusers.APMData.time_bufer[2]*10 + Lusers.APMData.time_bufer[3] == 2) && (Lusers.APMData.time_bufer[5] > 8))
		{
			Lusers.APMData.time_bufer[5] = 8;
		}		
		else if ((OLDLOWMONTH == 0) && (Lusers.APMData.time_bufer[4] == 3) && (Lusers.APMData.time_bufer[5] > 1))
		{
			Lusers.APMData.time_bufer[5] = 1;
		}
		else if ((OLDLOWMONTH == 1) && (Lusers.APMData.time_bufer[4] == 3) && (Lusers.APMData.time_bufer[5] > 0))
		{
			Lusers.APMData.time_bufer[5] = 0;
		}
		else if (Lusers.APMData.time_bufer[6] > 2)
		{
			Lusers.APMData.time_bufer[6] = 2;
		}		
		else if ((Lusers.APMData.time_bufer[6] == 2) && (Lusers.APMData.time_bufer[7] > 3))
		{
			Lusers.APMData.time_bufer[7] = 3;
		}
		else if (Lusers.APMData.time_bufer[8] > 5)
		{
			Lusers.APMData.time_bufer[8] = 5;
		}
		else if (Lusers.APMData.time_bufer[10] > 5)
		{
			Lusers.APMData.time_bufer[10] = 5;
		}
		
		if (Lusers.APMData.time_index == 0)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char_Fill(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);			
		}
		else if (Lusers.APMData.time_index == 1)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char_Fill(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);				
		}
		else if (Lusers.APMData.time_index == 2)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char_Fill(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 3)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char_Fill(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 4)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char_Fill(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 5)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char_Fill(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 6)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char_Fill(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 7)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char_Fill(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 8)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char_Fill(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 9)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char_Fill(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 10)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char_Fill(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		else if (Lusers.APMData.time_index == 11)
		{
			OLED_Show_16Char(44, 2, 0x42);
			OLED_Show_16Char(52, 2, Lusers.APMData.time_bufer[0] + 0x30);			
			OLED_Show_16Char(60, 2, Lusers.APMData.time_bufer[1] + 0x30);
			OLED_Show_16Char(68, 2, 0x41);
			OLED_Show_16Char(76, 2, Lusers.APMData.time_bufer[2] + 0x30);
			OLED_Show_16Char(84, 2, Lusers.APMData.time_bufer[3] + 0x30);
			OLED_Show_16Char(92, 2, 0x41);
			OLED_Show_16Char(100, 2, Lusers.APMData.time_bufer[4] + 0x30);
			OLED_Show_16Char(108, 2, Lusers.APMData.time_bufer[5] + 0x30);
			
			OLED_Show_16Char(44, 4, 0x42);
			OLED_Show_16Char(52, 4, Lusers.APMData.time_bufer[6] + 0x30);
			OLED_Show_16Char(60, 4, Lusers.APMData.time_bufer[7] + 0x30);
			OLED_Show_16Char(68, 4, 0x41);
			OLED_Show_16Char(76, 4, Lusers.APMData.time_bufer[8] + 0x30);
			OLED_Show_16Char(84, 4, Lusers.APMData.time_bufer[9] + 0x30);
			OLED_Show_16Char(92, 4, 0x41);
			OLED_Show_16Char(100, 4, Lusers.APMData.time_bufer[10] + 0x30);
			OLED_Show_16Char_Fill(108, 4, Lusers.APMData.time_bufer[11] + 0x30);		
		}
		OLED_Show_Chinese_16(0,6,"��");
		OLED_Show_16Char(16,6,0x3B);
		OLED_Show_16Char(24,6,0x3C);
		OLED_Show_16Char(32,6,0x3E);
		OLED_Show_Chinese_16(88,6,"��");
		OLED_Show_16Char(104,6,0x3B);
		OLED_Show_16Char(112,6,0x3D);
		OLED_Show_16Char(120,6,0x3E);
	}
	else if (show_data == OLEDSETAUDIOMENU)
	{
		OLED_Show_Chinese_16_Center(0, "��������");
		if (Lusers.APMData.Audio_Index == 0)
		{
			OLED_Show_Chinese_24_Fill(16, 2, "��");
			OLED_Show_Chinese_24(52, 2, "��");
			OLED_Show_Chinese_24(88, 2, "��");
		}
		else if (Lusers.APMData.Audio_Index == 1)
		{
			OLED_Show_Chinese_24(16, 2, "��");
			OLED_Show_Chinese_24_Fill(52, 2, "��");
			OLED_Show_Chinese_24(88, 2, "��");			
		}
		else if (Lusers.APMData.Audio_Index == 2)
		{
			OLED_Show_Chinese_24(16, 2, "��");
			OLED_Show_Chinese_24(52, 2, "��");
			OLED_Show_Chinese_24_Fill(88, 2, "��");
		}		
		OLED_Show_Chinese_16(0,6,"��");
		OLED_Show_16Char(16,6,0x3B);
		OLED_Show_16Char(24,6,0x3C);
		OLED_Show_16Char(32,6,0x3E);
		OLED_Show_Chinese_16(88,6,"��");
		OLED_Show_16Char(104,6,0x3B);
		OLED_Show_16Char(112,6,0x3D);
		OLED_Show_16Char(120,6,0x3E);		
	}
	else if (show_data == OLEDFINDRECORD)
	{
		OLED_Show_Chinese_16(12, 2, "����");
		OLED_Show_Chinese_16(12, 4, "ʱ��");
		OLED_Show_16Char(52, 6, Lusers.APMData.Open_Record_Userc/100 + 0x30);
		OLED_Show_16Char(60, 6, Lusers.APMData.Open_Record_Userc%100/10 + 0x30);
		OLED_Show_16Char(68, 6, Lusers.APMData.Open_Record_Userc%100%10 + 0x30);
		if (open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][0] == FINGEROPEN)
		{
			OLED_Show_Chinese_16(32,0,"��ָ");
			OLED_Show_16Char(64, 0, 0x3F);
			OLED_Show_16Char(72, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]/100 + 0x30);
			OLED_Show_16Char(80, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]%100/10 + 0x30);
			OLED_Show_16Char(88, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]%100%10 + 0x30);
		}
		else if (open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][0] == PWDOPEN)
		{
			OLED_Show_Chinese_16(32,0,"����");
			OLED_Show_16Char(64, 0, 0x3F);
			OLED_Show_16Char(72, 0, 0x42);
			OLED_Show_16Char(80, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]/10 + 0x30);
			OLED_Show_16Char(88, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]%10 + 0x30);			
		}
		else if (open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][0] == NFCOPEN)
		{
			OLED_Show_Chinese_16(32,0,"��Ƭ");
			OLED_Show_16Char(64, 0, 0x3F);
			OLED_Show_16Char(72, 0, 0x42);
			OLED_Show_16Char(80, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]/10 + 0x30);
			OLED_Show_16Char(88, 0, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][1]%10 + 0x30);						
		}		
		OLED_Show_16Char(44, 2, 0x42);
		OLED_Show_16Char(52, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][2]/10 + 0x30);
		OLED_Show_16Char(60, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][2]%10 + 0x30);
		OLED_Show_16Char(68, 2, 0x41);
		OLED_Show_16Char(76, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][3]/10 + 0x30);
		OLED_Show_16Char(84, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][3]%10 + 0x30);
		OLED_Show_16Char(92, 2, 0x41);
		OLED_Show_16Char(100, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][4]/10 + 0x30);
		OLED_Show_16Char(108, 2, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][4]%10 + 0x30);
		
		OLED_Show_16Char(44, 4, 0x42);
		OLED_Show_16Char(52, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][5]/10 + 0x30);
		OLED_Show_16Char(60, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][5]%10 + 0x30);
		OLED_Show_16Char(68, 4, 0x41);
		OLED_Show_16Char(76, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][6]/10 + 0x30);
		OLED_Show_16Char(84, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][6]%10 + 0x30);
		OLED_Show_16Char(92, 4, 0x41);
		OLED_Show_16Char(100, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][7]/10 + 0x30);
		OLED_Show_16Char(108, 4, open_list.open_bufer[Lusers.APMData.Open_Record_Userc -1][7]%10 + 0x30);				
	}
	else if (show_data == OLEDTESTERROR)
	{
		OLED_Show_Chinese_24_Center(1, "ϵͳ����");

		OLED_Show_24Char(52, 4, Lusers.APMData.test_error_time/10 + 0x30);
		OLED_Show_24Char(64, 4, Lusers.APMData.test_error_time%10 + 0x30);
	}
	else if (show_data == OLEDAPPOPENDOOR)
	{
		OLED_Show_Chinese_16_Center(0, "Զ�̿���");
		Show_Wifi_Risi();
		OLED_Show_24Char(52, 3, Lusers.APMData.APP_Open_Time/10 + 0x30);
		OLED_Show_24Char(64, 3, Lusers.APMData.APP_Open_Time%10 + 0x30);
	}

	Last_Show_Data = show_data;
}

