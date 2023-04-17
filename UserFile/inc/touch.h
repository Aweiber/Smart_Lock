#ifndef __touch_h__
#define __touch_h__

#include "include.h"


#define TOUCH_INTER_PORT GPIOB
#define TOUCH_INTER_PIN  GPIO_PIN_5

#define TOUCH_GOING   12
#define TOUCH_BACK    10
#define TOUCH_UP      2
#define TOUCH_DOWN    8

extern uint8_t Key_Data;
extern I2C_HandleTypeDef hi2c1;

void Touch_DeInit(void);
void Touch_Init(void);
uint8_t Touch_ReadByte(uint8_t DAddress);
void Touch_ReadKey(uint8_t *Touch3_Bufer);
#endif

