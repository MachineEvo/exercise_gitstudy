#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "encoder.h"
#include "MPU6050.h"
#include "inv_mpu.h"
float 
	Vertical_Kp=360,                                     
	Vertical_Kd=2;
float 
	Velocity_Kp=-0.4,
	Velocity_Ki=-0.002;
float 
	Turn_Kd=-0.6,
	Turn_Kp=-20;
float Med_Angle=8; 	
float Pitch,Roll,Yaw,Target_Speed=0,Turn_Speed=0;
short gyrox,gyroy,gyroz,aacx,aacy,aacz;				
extern int Fore,Back,Left,Right;
int MOTO1,MOTO2,Encoder_Left,Encoder_Right,length,Vertical_out,Velocity_out,Turn_out;
extern char stop;
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);
int Vertical(float Med,float Angle,float gyro_Y);
void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)==0)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			Encoder_Left=-Read_Speed(2);
			Encoder_Right=Read_Speed(4);
			jd(&Roll,&Pitch,&Yaw);			
			MPU6050_GetData(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);
			if((Fore==0)&&(Back==0))Target_Speed=0;
			if(Fore==1)
			{
				if(length<20)Target_Speed++;
				else
				Target_Speed--;
			}
			if(Back==1){Target_Speed++;}
			if (Target_Speed>20) Target_Speed=20;
			if (Target_Speed<-20) Target_Speed=-20;
			if((Left==0)&&(Right==0))Turn_Speed=0;
			if(Left==1)Turn_Speed+=30;	
			if(Right==1)Turn_Speed-=30;	
			if (Turn_Speed>40) Turn_Speed=40;
			if (Turn_Speed<-40) Turn_Speed=-40;
			if((Left==0)&&(Right==0))Turn_Kd=0.6;
			else if((Left==1)||(Right==1))Turn_Kd=0;
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			
			Turn_out=Turn(gyroz,Turn_Speed);															
			PWM_out=Vertical_out;
			MOTO1=PWM_out-Turn_out;
			MOTO2=PWM_out+Turn_out;
			Limit(&MOTO1,&MOTO2);			
			Load(MOTO1,MOTO2);		
			Stop(&Med_Angle,&Pitch);
		}
	}
}
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
	Encoder_Err=((encoder_left+encoder_right)-Target);
	EnC_Err_Lowout=0.3*Encoder_Err+0.7*EnC_Err_Lowout_last;
	EnC_Err_Lowout_last=EnC_Err_Lowout;
	Encoder_S+=EnC_Err_Lowout;
	if (Encoder_S>20000) Encoder_S=20000;
	if (Encoder_S<-20000) Encoder_S=-20000;
	if(stop==1)Encoder_S=0,stop=0;
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	PWM_out=Turn_Kd*gyro_Z + Turn_Kp*RC;
	return PWM_out;
}
