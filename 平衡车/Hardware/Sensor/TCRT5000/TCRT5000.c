#include "stm32f10x.h"                  // Device header

void TCRT5000_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIODEF;
	GPIODEF.GPIO_Mode = GPIO_Mode_IPU;
	GPIODEF.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIODEF.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIODEF);
}

u8 Get_TrackingState(uint16_t Pin)
{
	return GPIO_ReadInputDataBit(GPIOA,Pin);
}
