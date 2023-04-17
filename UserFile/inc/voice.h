#ifndef __voice_h__
#define __voice_h__

#include "include.h"

#define VOICE_BUSY_STATE ((HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 1) ? 1 : 0)

void WTN6_Gpio_Init(void);
void WTN6_Send_Data(uint8_t data);
void WTN6_Play(uint8_t cmd);
#endif

