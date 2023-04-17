#include "power.h"

uint32_t ADCPowerValueBufer[100] = {0};

ADC_HandleTypeDef hadc;

void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */
  /* USER CODE END ADC_Init 2 */
}

void MX_ADC_DeInit(void)
{
	HAL_ADC_Stop(&hadc);//ֹͣADC
	HAL_ADC_DeInit(&hadc);	
}

/*
      2540    6v     ����6vΪ4���
      2360    5.4v   5.4v-6vΪ3���
      2230    5.0v   5.0v-5.4vΪ2���
      2170    4.8v   4.8v-5.0vΪ1��粢����
      2120    4.6v   4.6v-4.8vΪ0��粢����
      ����4.6v�������߲�������ʹ��
*/

int Get_Power_Value(void)
{
	uint16_t Adc_Value = 0;
	uint16_t Value_Sum = 0;
	
  for (int i = 0; i < 10; i++)
	{
	  HAL_ADC_Start(&hadc); //����hadc
		HAL_ADC_PollForConversion(&hadc, 50); //�ȴ�ת����ɣ���ʱʱ��50ms
		if (HAL_IS_BIT_SET((HAL_ADC_GetState(&hadc)), HAL_ADC_STATE_REG_EOC)) //ת��ADC״̬����תģ��if�ж�ת���Ƿ����
		{
			Adc_Value = HAL_ADC_GetValue(&hadc); //��ȡADCת����ֵ
		}
		Value_Sum += Adc_Value;
  }
	printf("Value_Sum = %d\n", Value_Sum/10);
	return Value_Sum/10;
}

