#ifndef __flash_h__
#define __flash_h__

#include "include.h"

typedef struct
{
	uint8_t finger_user_num;
	uint8_t finger_user_bufer[100];	
	uint8_t pwd_user_num;
	uint8_t pwd_user_bufer[10];
	uint8_t pwd_user_pwd_bufer[10][6];
	uint8_t nfc_user_num;
	uint8_t nfc_user_bufer[80];
	uint8_t nfc_user_nfc_bufer[80][4];
	uint8_t time_bufer[7];
}flash_list_t;

typedef struct
{
	uint8_t open_mode;
	uint8_t open_number;
	uint8_t open_bufer[200][8]; //[x][0]为开门方式，1为手指，2为密码，3为IC卡。[x][1]为用户编号，其余为开门时间
}open_list_t;


#define DATA_FALSH_DRESS 61
#define OPEN_DRESS       62

#define DATAFLASH     0
#define OPENFLASH     1


#define FINGEROPEN    1
#define PWDOPEN       2
#define NFCOPEN       3

extern flash_list_t flash_list;
extern open_list_t open_list;

void Flash_Init(void);
void FLASH_ReadPlay(uint8_t FLASHState);
void FLASH_WritePlay(uint8_t FLASHState);
uint8_t *TUYA_WiFi_Zone_Page(uint8_t *inputbuf, uint8_t *len);
#endif

