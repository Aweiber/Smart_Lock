#ifndef __led_h__
#define __led_h__

#include "include.h"

#define LED_PORT GPIOF

#define LED1_PIN GPIO_PIN_6
#define LED2_PIN GPIO_PIN_7

#define LED_OFF 0
#define LED_ON  1

#define LED1_ON HAL_GPIO_WritePin(GPIOF, LED1_PIN, GPIO_PIN_SET);
#define LED2_ON HAL_GPIO_WritePin(GPIOF, LED2_PIN, GPIO_PIN_SET);
#define LED1_OFF HAL_GPIO_WritePin(GPIOF, LED1_PIN, GPIO_PIN_RESET);
#define LED2_OFF HAL_GPIO_WritePin(GPIOF, LED2_PIN, GPIO_PIN_RESET);

void LED_Init(void);
void LED_OnOff(uint8_t OnOff);

#endif

