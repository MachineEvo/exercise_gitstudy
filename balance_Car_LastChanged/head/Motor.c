#include "stm32f10x.h"    // Device header
#include "sys.h"
#define PWM_MAX  7200 
#define PWM_MIN  -7200
int MotorFlag1,MotorFlag2;
void Motor_PWM_Init(u16 Psc,u16 Per){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=Per;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=Psc;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
  TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
//	TIM_OCInitStructure.TIM_OCIdleState
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OCNIdleState
//	TIM_OCInitStructure.TIM_OCNPolarity
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_OutputNState
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0; //CCR
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1,ENABLE);
	
}
void Motor_Init(void){
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}
void Motor_PWM_SetCompare1(uint32_t Compare){

TIM_SetCompare1(TIM1,Compare);
}
void Motor_PWM_SetCompare4(uint32_t Compare){

TIM_SetCompare4(TIM1,Compare);
}
int Motor_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

void Motor_SetSpeed(int Motor1,int Motor2)
{
	if(Motor1>0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);//正转
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		Motor_PWM_SetCompare1(Motor_abs(Motor1));
		MotorFlag1=1;
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//反转
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		Motor_PWM_SetCompare1(Motor_abs(Motor1));
	}
	
	if(Motor2<0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);//正传
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		Motor_PWM_SetCompare4(Motor_abs(Motor2));
		MotorFlag2=1;
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);//反转
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		Motor_PWM_SetCompare4(Motor_abs(Motor2));
	}
	
	
}
void Motor_Limit(int*motoA,int*motoB)
{
		if(*motoA>PWM_MAX)
			*motoA=PWM_MAX;
		if(*motoA<PWM_MIN)
			*motoA=PWM_MIN;
		
		if(*motoB>PWM_MAX)
			*motoB=PWM_MAX;
		if(*motoB<PWM_MIN)
			*motoB=PWM_MIN;
}
