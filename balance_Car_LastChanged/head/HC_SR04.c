#include "stm32f10x.h"                  // Device header
#include "Delay.h"
uint16_t TimeCount;
void HC_SR04Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);           //Trig
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStructure);            //Echo
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=10-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;     
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}
float Sonar(void)
{
	float Distance_m,Distance_mm;
	uint32_t Time_end;
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_SET);         
	delay_us(15);
	GPIO_WriteBit(GPIOB,GPIO_Pin_3,Bit_RESET);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0);//
	TimeCount=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==1);
	Time_end=TimeCount*10;//1us
	Distance_m=Time_end*0.000001f*340/2;
	Distance_mm=Distance_m*1000;
	return Distance_mm;
	
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		TimeCount++;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		
		
		
		
		
	}
	
}
