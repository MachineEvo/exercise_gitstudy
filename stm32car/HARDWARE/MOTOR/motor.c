#include "motor.h"
#include "sys.h"
extern float Target_Speed;
/*电机初始化函数*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//初始化GPIO--PB12、PB13、PB14、PB15为推挽输出
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

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	Ain1=0,Ain2=1;//正转
	else 				Ain1=1,Ain2=0;//反转
	//2.研究PWM值
	TIM_SetCompare1(TIM1,GFP_abs(moto1));
	
	if(moto2>0)	Bin1=0,Bin2=1;
	else 				Bin1=1,Bin2=0;	
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

void XunJi(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
void Run(void)
{
	/*TIM_SetCompare1(TIM1,1000);
	Bin1=0,Bin2=1;
	TIM_SetCompare4(TIM1,1000);
	Ain1=0,Ain2=1;*/
	
	
}
void GoBack(void)
{
	TIM_SetCompare1(TIM1,3000);
	Bin1=1,Bin2=0;
	TIM_SetCompare4(TIM1,3000);
	Ain1=1,Ain2=0;
}
void Turn_Left(void)
{
	TIM_SetCompare1(TIM1,0);
	Bin1=0,Bin2=1;
	TIM_SetCompare4(TIM1,6500);
	Ain1=0,Ain2=1;
}
void Turn_Right(void)
{
	TIM_SetCompare1(TIM1,6500);
	Bin1=0,Bin2=1;
	TIM_SetCompare4(TIM1,0);
	Ain1=0,Ain2=1;
}
void Mode_XunJi(void)
{
	if((PAin(4)==0)&&(PAin(5)==0))
	{
		Run();
	}
	else if((PAin(5)==1)&&(PAin(4)==0))
	{
		Turn_Right();
	}
	else if((PAin(5)==0)&&(PAin(4)==1))
	{
		Turn_Left();
	}
	else if((PAin(5)==1)&&(PAin(4)==1))
	{
		Run();
	}
		
}










