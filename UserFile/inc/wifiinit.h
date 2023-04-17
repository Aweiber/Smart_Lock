#ifndef __wifiinit_h__
#define __wifiinit_h__

#include "include.h"

#define WIFI_POWER_PORT GPIOA
#define WIFI_POWER_PIN GPIO_PIN_8

#define WIFI_POWER_ON  HAL_GPIO_WritePin(WIFI_POWER_PORT, WIFI_POWER_PIN, GPIO_PIN_SET)
#define WIFI_POWER_OFF  HAL_GPIO_WritePin(WIFI_POWER_PORT, WIFI_POWER_PIN, GPIO_PIN_RESET)

extern UART_HandleTypeDef huart1;

void WIFI_Init(void);
#endif

