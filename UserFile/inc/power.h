#ifndef __power_h__
#define __power_h__

#include "include.h"

extern ADC_HandleTypeDef hadc;

void MX_ADC_DeInit(void);
void MX_ADC_Init(void);
int Get_Power_Value(void);
#endif


