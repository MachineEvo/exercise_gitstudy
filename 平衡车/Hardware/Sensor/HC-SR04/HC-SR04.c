#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "HC-SR04.h"

int overcount=0; 

void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIODEF;
	TIM_TimeBaseInitTypeDef TIMDEF;
	NVIC_InitTypeDef NVICDEF;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			
	//PA3作为TRIG
	GPIODEF.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIODEF.GPIO_Pin=GPIO_Pin_3;
    GPIODEF.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIODEF);
    
	//PA2作为ECHO
	GPIODEF.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIODEF.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIODEF);
	
	TIMDEF.TIM_Period = 999;
	TIMDEF.TIM_Prescaler = 7199;
	TIMDEF.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMDEF.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIMDEF);

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVICDEF.NVIC_IRQChannel = TIM3_IRQn;
	NVICDEF.NVIC_IRQChannelPreemptionPriority = 0;
	NVICDEF.NVIC_IRQChannelSubPriority = 3;
	NVICDEF.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICDEF);

	TIM_Cmd(TIM3, DISABLE);
}

int Senor_Using(void) 
{
		unsigned int sum=0;
		unsigned int tim;
		unsigned int i=0;
		unsigned int length;
		u16 cnt_i=0;
		while(i!=3)        
		{
			TRIG_SET;          
			Delay_us(20); //TRIG输入高电平，延迟20us   
			TRIG_RESET;          
			cnt_i=0;
			while(ECHO==0)//防止超声波没有传出去
			{
				cnt_i++;
				Delay_us(1);
				if(cnt_i>30000)
				{	TRIG_SET;          
					Delay_us(20);    
					TRIG_RESET;cnt_i=0;          
				}
			} 
			TIM_Cmd(TIM3,ENABLE);
			
			i+=1;                     
			
			while(ECHO==1);	//在高电平期间，保持TIM3计数
			TIM_Cmd(TIM3,DISABLE);    
			
			tim=TIM_GetCounter(TIM3);         
			length=(tim*100)/58.0; 
			sum=length+sum;
			TIM3->CNT=0; 
			overcount=0; 
			Delay_ms(100);
		}
		length=sum/3;
		return length; 
}

void TIM3_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET) //检查是否发生TIM3中断
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update );   //清楚中断更新标志
			overcount++;
		}
}
