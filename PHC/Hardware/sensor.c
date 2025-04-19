#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
uint16_t Num;
int sensor()
{
	int sum,time,i,cnt;
	TIM_SetCounter (TIM3,0);
	for (i=0;i<3;i++)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		Delay_us(20);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
		cnt=0;
		while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
		{
			cnt++;
			Delay_us(20);
			if  (cnt>2000)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_3);
		    GPIO_SetBits(GPIOA,GPIO_Pin_3);
		    Delay_us(20);
		    GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
        cnt=0;				
			}
		}
		TIM_Cmd(TIM3,ENABLE);
		while (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==1);
		TIM_Cmd(TIM3,DISABLE);  
		time=TIM_GetCounter(TIM3);  
		sum+=(time*100)/58;
		TIM_SetCounter(TIM3,0);
		Num=0;
		Delay_ms(100);
	}
	return sum/3;
}
