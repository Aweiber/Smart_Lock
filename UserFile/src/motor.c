#include "motor.h"

#define MOTOR1_PORT GPIOB
#define MOTOR2_PORT GPIOA

#define MOTOR1_PIN  GPIO_PIN_3
#define MOTOR2_PIN  GPIO_PIN_15

#define MOTOR1_RESET HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_PIN, GPIO_PIN_RESET)
#define MOTOR2_RESET HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_PIN, GPIO_PIN_RESET)

#define MOTOR1_SET HAL_GPIO_WritePin(MOTOR1_PORT, MOTOR1_PIN, GPIO_PIN_SET)
#define MOTOR2_SET HAL_GPIO_WritePin(MOTOR2_PORT, MOTOR2_PIN, GPIO_PIN_SET)

void Motor_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin   = MOTOR1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(MOTOR1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin   = MOTOR2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(MOTOR2_PORT, &GPIO_InitStruct);	
}

void Motor_OpenClose(uint8_t MotorState)
{
	if (MotorState == MOTOR_OPEN)
	{
		MOTOR2_RESET;
		MOTOR1_SET;
		delay_ms(300);
		MOTOR1_RESET;
		MOTOR2_RESET;
	}
	else if (MotorState == MOTOR_CLOSE)
	{
		MOTOR1_RESET;
		MOTOR2_SET;
		delay_ms(300);
		MOTOR2_RESET;
		MOTOR1_RESET;		
	}
}

