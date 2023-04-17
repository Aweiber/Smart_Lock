#ifndef __motor_h__
#define __motor_h__

#include "include.h"

#define MOTOR_OPEN  0
#define MOTOR_CLOSE 1

void Motor_Init(void);
void Motor_OpenClose(uint8_t MotorState);

#endif


