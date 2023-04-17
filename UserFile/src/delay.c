#include "delay.h"

uint32_t fac_us = 0, fac_ms = 0;

void delay_init(void)
{
	uint32_t reload;
	
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);   //选择外部时钟
	
	fac_us = SystemCoreClock/1000000;
	reload = SystemCoreClock/1000000;
	reload *= 1000000/1000;
	
	fac_ms = 1000/1000;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->LOAD = reload;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
}

void delay_us(uint32_t nus)
{
	uint32_t ticks = 0;
	uint32_t told = 0, tnow = 0, tcnt = 0;
	uint32_t reload = SysTick->LOAD;
	ticks = nus * fac_us;
	told = SysTick->VAL;
	
	while(1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow;
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if(tcnt >= ticks)
				break;
		}
	}
}

void delay_ms(uint32_t nms)
{
	delay_us((uint32_t)(nms*1000));				//普通方式延时
}

