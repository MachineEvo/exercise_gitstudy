#include "stm32f10x.h"                  // Device header
#include "PWM.h"
void Motor_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	PWM_Init();
}
void Limit(int *motoA,int *motoB)
{
	if(*motoA>7200)*motoA=7200;
	if(*motoA<-7200)*motoA=-7200;
	
	if(*motoB>7200)*motoB=7200;
	if(*motoB<-7200)*motoB=-7200;
}
void Load(int moto1,int moto2)
{ 
	if(moto1<0)	
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	}
	else 			
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	}
	TIM_SetCompare1(TIM1,abs(moto1));
	if(moto2<0)	
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	else 			
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
	TIM_SetCompare4(TIM1,abs(moto2));
}
char stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(abs(*Jiaodu-*Med_Jiaodu)>60)
	{
		Load(0,0);
		stop=1;
	}
}
