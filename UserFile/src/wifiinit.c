#include "wifiinit.h"

UART_HandleTypeDef huart1;

void WIFI_Power_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin  = WIFI_POWER_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	
	HAL_GPIO_Init(WIFI_POWER_PORT, &GPIO_InitStruct);
}

void WIFI_Init(void)
{
	wifi_protocol_init();
	WIFI_Power_Init();
	WIFI_POWER_ON;
	
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_UART_Receive_IT(&huart1, &wifi_recv, 1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_ERR);
}
