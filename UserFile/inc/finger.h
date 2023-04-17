#ifndef __finger_h__
#define __finger_h__

#include "include.h"

#define FPOWER_PORT GPIOA
#define FINTER_PORT GPIOA

#define FPOWER_PIN GPIO_PIN_1
#define FINTER_PIN GPIO_PIN_4

#define FINGER_POWER_ON  1
#define FINGER_POWER_OFF 0

#define CONNECT_SUCCESS  1
#define CONNECT_FAIL     0

#define FINGER_CONNECT   0
#define FINGER_ADD       1
#define FINGER_VERITY    2
#define FINGER_FACTORY   3
#define FINGER_BREAK     4
#define FINGER_DELETE    5
#define FINGER_GETINFO   6

#define SLEEP_IN         0
#define SLEEP_OUT        1

#define PLEASE_FINGER        0
#define PLEASE_FINGER_AGAING 1
#define ADD_FINGER_SUCCESS   2
#define ADD_FINGER_FAIL      3
#define FINGER_FAIL          4


#define DELETE_FINGER_SUCCESS 0
#define DELETE_FINGER_FAIL    1

#define FINGER_FACTORY_SUCCESS 0
#define FINGER_FACTORY_FAIL    1


typedef struct
{
	uint8_t Add_Finger_State     :6;
	uint8_t Factory_Finger_State :2;
	uint8_t Sleep_State          :2;
	uint8_t Delete_Finger_State  :3;
	uint8_t finger_open_index;
	uint8_t finger_open_buf[10];
}Finger_Data;

extern Finger_Data finger_state;
extern UART_HandleTypeDef huart2;
extern uint8_t OpenDoorState;
extern uint8_t Finger_Add_State_IT;

void Finger_Input_Data(uint8_t finger_data);
void fingerService(void);
void Finger_Uart2_Init(void);
void Finger_Inter_Init(void);
void Finger_Power_Init(void);
void Finger_Recevie_Service(uint8_t *recevie_buf, uint8_t index);
void Finger_Power_OnOff(uint8_t State);
void Finger_Show_Play(uint8_t finger_data, uint8_t UserID);
#endif

