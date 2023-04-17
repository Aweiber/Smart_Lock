#include "rtc.h"

RTC_HandleTypeDef hrtc;
RTC_DateTypeDef   hymd;
RTC_TimeTypeDef   hhms;

void MX_Set_Ymd(uint8_t year, uint8_t month, uint8_t day)
{
	hymd.Year  = year;
	hymd.Month = month;
	hymd.Date  = day;
	
	HAL_RTC_SetDate(&hrtc, &hymd, RTC_FORMAT_BIN);
}

void MX_Set_Hms(uint8_t hours, uint8_t minutes, uint8_t second)
{
	hhms.Hours   = hours;
	hhms.Minutes = minutes;
	hhms.Seconds = second;
	
	HAL_RTC_SetTime(&hrtc, &hhms, RTC_FORMAT_BIN);
}

void MX_RTC_Init(void)
{
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
}

void MX_RTC_GetYmd(void)
{
	HAL_RTC_GetDate(&hrtc, &hymd, RTC_FORMAT_BIN);
	delay_ms(20);               //加二十毫秒延时，消除显示屏显示秒速时的顿挫
//	printf("当前时间为：%d:%d:%d\r\n", hymd.Year, hymd.Month, hymd.Date);
}

void MX_RTC_GetHms(void)
{
	HAL_RTC_GetTime(&hrtc, &hhms, RTC_FORMAT_BIN);
	delay_ms(20);              //加二十毫秒延时，消除显示屏显示秒速时的顿挫
//	printf("%d:%d:%d\r\n", hhms.Hours, hhms.Minutes, hhms.Seconds);	
}

void RTC_GET_Time(void)
{
	MX_RTC_GetYmd();
	MX_RTC_GetHms();
}

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t data;
	uint8_t hours;
	uint8_t mintues;
	uint8_t seconds;
}GeLin_UTC;

void BeiJTime_To_GelinUTC(uint8_t *InpueTime) //年月日，时分秒
{
	GeLin_UTC *GeLin_Time;
	
  GeLin_Time = (GeLin_UTC *)InpueTime;

	if (GeLin_Time->hours < 8) //小于八小时往前天借一天
	{
		if (GeLin_Time->data <= 1) //小于一天往前月借一天
		{
			if (GeLin_Time->month <= 1) //小于一月往前年借一天
			{
				if (GeLin_Time->year == 0)
				{
					GeLin_Time->year = 99;
				}
				else
				{
				  GeLin_Time->year = GeLin_Time->year - 1;
				}
				GeLin_Time->month = 12;
				GeLin_Time->data = 31;
				GeLin_Time->hours = GeLin_Time->hours + 24 - 8;
			}
			else
			{
				GeLin_Time->month = GeLin_Time->month - 1;
				if((GeLin_Time->month == 1) || (GeLin_Time->month == 3) || (GeLin_Time->month == 5) || (GeLin_Time->month == 7) || (GeLin_Time->month == 8) || (GeLin_Time->month == 10))
				{
					GeLin_Time->data = 31;
				}
				else if ((GeLin_Time->month == 4) || (GeLin_Time->month == 6) || (GeLin_Time->month == 9) || (GeLin_Time->month == 11))
				{
					GeLin_Time->data = 30;
				}
				else
				{
					if (GeLin_Time->year%4 == 0)
					{
						GeLin_Time->data = 29;
					}
					else
					{
						GeLin_Time->data = 28;
					}
				}
				GeLin_Time->hours = GeLin_Time->hours + 24 - 8;
			}
		}
		else
		{
			GeLin_Time->data = GeLin_Time->data - 1;
			GeLin_Time->hours = GeLin_Time->hours + 24 - 8;
		}
	}
	else
	{
		GeLin_Time->hours = GeLin_Time->hours - 8;
	}

	printf("UTC-8当前格林时间为 : ");
	for(int i = 0; i < 6; i++)
	{
		printf("%d-", *(InpueTime + i));
	}
	printf("\r\n");
}

