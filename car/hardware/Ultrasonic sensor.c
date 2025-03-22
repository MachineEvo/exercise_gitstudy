#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Delay.h"
#include "OLED.h"
extern int FTime,distance;

void Ultrasonic_Sensor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	Timer_Init();

}

void GetDistance(void){
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
	


}

void EXTI4_IRQHandler(void){

	if(EXTI_GetITStatus(EXTI_Line4)==SET){
		TIM_SetCounter(TIM3,0);	
		TIM_Cmd(TIM3,ENABLE);		
		int timeout = 0;
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)&&timeout++<10000);
		TIM_Cmd(TIM3,DISABLE); 
		distance = TIM_GetCounter(TIM3)*1.7; 
		EXTI_ClearITPendingBit(EXTI_Line4);		
	}	
}


