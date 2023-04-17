#ifndef __include_h__
#define __include_h__

#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"

#include <stdio.h>
#include "string.h"

#include "delay.h"
#include "led.h"
#include "finger.h"
#include "voice.h"
#include "oled.h"
#include "power.h"
#include "motor.h"
#include "touch.h"
#include "sys.h"
#include "flash.h"
#include "rtc.h"
#include "tim.h"

#include "bsp_spinfc.h"
#include "type_a.h"
#include "type_b.h"
#include "LPCD_API.h"
#include "fm175xx.h"

#include "wifiinit.h"
#include "lock_api.h"
#include "mcu_api.h"
#include "protocol.h"
#include "system.h"
#include "wifi.h"

#define TEST_ERROR_MAX   5
#define TEST_ERROR_TIME  60


#define TRUE 1
#define FALSE 0
#define OK 1
#define NO 0

typedef struct
{
	void (*Delay)();
	void (*Led)();
	void (*Finger_Uart)();
	void (*Finger_PowerPin)();
	void (*Finger_InterPin)();
	void (*Voice_Pin)();
	void (*Oled)();
	void (*Power_Adc)();
	void (*Motor)();
	void (*Touch)();
	void (*Flash)();
	void (*Rtc)();
	void (*Tim3)();
	void (*Wifi)();
}APM32_Init;

typedef struct
{
	void (*USDelay)(uint32_t);
	void (*MSDelay)(uint32_t);
	void (*ONOFFLed)(uint8_t);
	void (*ONOFFFinger)(uint8_t);
	void (*Finger_Play)(uint8_t, uint8_t);
	void (*Voice_Play)(uint8_t);
	void (*OLED_Show)(uint8_t);
	int (*Get_Power)();
	void (*Motor_Play)(uint8_t);
	void (*FLASH_WPlay)(uint8_t);
	void (*FLASH_RPlay)(uint8_t);
	void (*GetTime)();
}APM32_Appli;

typedef struct
{
	uint8_t Show_Page;
	uint8_t Inter_Data;
	uint8_t Finger_Userc;
	uint8_t Pwd_Userc;
	uint8_t Nfc_Userc;
	uint8_t Open_Finger_Userc;
	uint8_t Open_Pwd_Userc;
	uint8_t Open_Nfc_Userc;
	uint8_t Pwd_Bufer[30];
	uint8_t Pwd_Index;
	uint8_t Input_PwdBufer[10];
	uint8_t Input_PwdIndex;
	uint8_t Confirm_PwdBufer[10];
	uint8_t Confirm_PwdIndex;
	uint8_t time_index;
	uint8_t time_bufer[12];
	uint8_t Audio_Index;
	uint8_t Open_Record_Userc;
	uint16_t OLEDReturn;
	uint8_t Indoor_time;
	uint8_t pick_state;
	uint8_t test_error;
	uint8_t test_error_time;
	uint8_t wifi_risi;
	uint8_t Start_UPState;
	uint8_t APP_Open_Time;
	uint8_t Temparory_pwd_state;
	uint8_t Temparoary_Pwd_Bufer[7];
	uint8_t Low_Power_toWiFi;
	uint8_t Pick_toWiFi;
	uint8_t UPData_Open_Record;
}APM32_Data;

typedef struct
{
	APM32_Init Initialization;
	APM32_Appli Application;
	APM32_Data APMData;
}APM32_Users;

extern APM32_Users Lusers;

void Error_Handler(void);
void printf_bufer(uint8_t *p);
#endif

