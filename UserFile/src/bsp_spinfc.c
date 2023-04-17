/*
  NFC SPI2
*/

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_spinfc.h"


/* ˽�б��� ------------------------------------------------------------------*/
SPI_HandleTypeDef hspinfc;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

//�жϻ��ѳ�ʼ��
void nfc_interrupt_init(void)
{
	//PC6  �ж�
  GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
#if 1

  /*Configure GPIO pin : PtPin */  
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  /* EXTI interrupt init*/

  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 2, 3);
#endif
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
}
void FM175XX_HardReset(void)
{	
	HAL_GPIO_WritePin(NFC_SPI_CS_PORT,NFC_SPI_CS_PIN,GPIO_PIN_SET);
	FM175XX_HPD(0);//NPD=0
	delay_ms(1);		
	FM175XX_HPD(1);//NPD=1	
	delay_ms(1);
	return;
}
//SPI2 ���ѳ�ʼ��
void SPI_WakeUp(void)
{
	//SPI��ʼ��
  hspinfc.Instance = NFC_SPIx;
  hspinfc.Init.Mode = SPI_MODE_MASTER;
  hspinfc.Init.Direction = SPI_DIRECTION_2LINES;
  hspinfc.Init.DataSize = SPI_DATASIZE_8BIT;
  hspinfc.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspinfc.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspinfc.Init.NSS = SPI_NSS_SOFT;
  hspinfc.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspinfc.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspinfc.Init.TIMode = SPI_TIMODE_DISABLE;
  hspinfc.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspinfc.Init.CRCPolynomial = 7;
  HAL_SPI_Init(&hspinfc);
	
	if(MCU_TO_PCD_TEST() == TRUE)
	{
		Nfc_Status=OK;
	}
	else
	{
		Nfc_Status=NO;
	}
}
/**
  * ��������: ����FLASH��ʼ��
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
*/
uint8_t MX_SPINfc_Init(void)
{
	//SPI��ʼ��
/*  hspiflash.Instance = FLASH_SPIx;
  hspiflash.Init.Mode = SPI_MODE_MASTER;
  hspiflash.Init.Direction = SPI_DIRECTION_2LINES;
  hspiflash.Init.DataSize = SPI_DATASIZE_8BIT;
  hspiflash.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspiflash.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspiflash.Init.NSS = SPI_NSS_SOFT;
  hspiflash.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspiflash.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspiflash.Init.TIMode = SPI_TIMODE_DISABLE;
  hspiflash.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspiflash.Init.CRCPolynomial = 7;
*/

  hspinfc.Instance               = NFC_SPIx;
  hspinfc.Init.Mode = SPI_MODE_MASTER;
  hspinfc.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspinfc.Init.Direction         = SPI_DIRECTION_2LINES; //�ɷ�����ģʽ
  hspinfc.Init.CLKPhase          = SPI_PHASE_1EDGE;      //CLK�ڵ�һ����ȡ����    CPHA = 0
  hspinfc.Init.CLKPolarity       = SPI_POLARITY_LOW;     //CLK��ʼ��ƽΪ�͵�ƽ    CPOL = 0ѡ��ģʽ0
  hspinfc.Init.DataSize          = SPI_DATASIZE_8BIT;
  hspinfc.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspinfc.Init.TIMode            = SPI_TIMODE_DISABLE;
  hspinfc.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspinfc.Init.CRCPolynomial     = 7;
  hspinfc.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  hspinfc.Init.NSS               = SPI_NSS_SOFT;
  hspinfc.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
  HAL_SPI_Init(&hspinfc);
//	delay_ms(200);/////

#if 1
	if(MCU_TO_PCD_TEST() == TRUE)
	{
		Nfc_Status=OK;
		nfc_interrupt_init();
		FM175XX_HardReset();   
		if(Lpcd_Calibration_Event()== SUCCESS)
		{
			Lpcd_Calibration_Backup();
		}
	}
	else
	{
		Nfc_Status=NO;
	}
        
	return 1;
#else

	Nfc_Status=OK;
#endif

}




/**
  * ��������: ������Flash��ȡд��һ���ֽ����ݲ�����һ���ֽ�����
  * �������: byte������������
  * �� �� ֵ: uint8_t�����յ�������
  * ˵    ������
  */
uint8_t SPI_NFC_SendByte(uint8_t byte)
{
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(&hspinfc,&d_send,&d_read,1,0xFFFFFF)!=HAL_OK)
    d_read=0xFF;
  
  return d_read; 
}


   
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

