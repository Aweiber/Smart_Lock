#ifndef __rtc_h__
#define __rtc_h__

#include "include.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_DateTypeDef hymd;
extern RTC_TimeTypeDef hhms;

void MX_Set_Ymd(uint8_t year, uint8_t month, uint8_t day);
void MX_Set_Hms(uint8_t hours, uint8_t minutes, uint8_t second);
void MX_RTC_Init(void);
void RTC_GET_Time(void);
void BeiJTime_To_GelinUTC(uint8_t *InpueTime);
#endif

