#include "stm32f10x.h"                  // Device header

void xunji_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

}

uint16_t xunjiR_Get_State()
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
}
uint16_t xunjiL_Get_State()
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
}


