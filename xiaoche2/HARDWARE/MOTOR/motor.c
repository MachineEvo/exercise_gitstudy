#include "motor.h"

#define Ain1  PBout(14)
#define Ain2  PBout(15)

#define Bin1  PBout(13)
#define Bin2  PBout(12)

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}

/*限幅函数*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}


void Load(int moto1,int moto2)
{
	if(moto1>0) Ain1=1,Ain2=0;//正转
	else 				Ain1=0,Ain2=1;//反转
	TIM_SetCompare1(TIM1,GFP_abs(moto1));
	
	if(moto2>0)	Bin1=1,Bin2=0;
	else 				Bin1=0,Bin2=1;	
	TIM_SetCompare4(TIM1,GFP_abs(moto2));
}


char PWM_Zero=0,stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>60)
	{
		Load(PWM_Zero,PWM_Zero);
		stop=1;
	}
}

//void car_go_straight(void)
//{
//	TIM_SetCompare1(TIM1,50);
//	TIM_SetCompare4(TIM1,50);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0);
//}

//void car_turnleft(void)
//{
//	TIM_SetCompare1(TIM1,50);
//	TIM_SetCompare4(TIM1,50);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0);
//}

//void car_turnright(void)
//{
//	TIM_SetCompare1(TIM1,50);
//	TIM_SetCompare4(TIM1,50);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)1);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)0);
//	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)1);
//}

