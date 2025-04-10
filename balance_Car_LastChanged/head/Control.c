#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "EXTI.h"
#include "Encoder.h"
#include "Motor.h"
#include "usart.h"
#include "trace.h"
float Med_Angle=-5;
int Encoder_Left,Encoder_Right;
int Vertical(float Med,float Angle,float gx);
int Velocity(float Target_Speed,int Encoder_Left,int Encoder_Right);
int Turn(short gz,int Remote_Control);
void Motor_Stop(float Med,float Angle);
int Vertical_Out,Velocity_Out,Turn_Out;
int TraceLeft,TraceRight,TraceStright;
float Vertical_Kp=280,Vertical_Kd=0.67;//kp:0-1000//kd:0-10//270//0.67
float Velocity_Kp=-0.72,Velocity_Ki;//kp:0-1//ki:1/200kp//+0.70
float Turn_Kp,Turn_Kd;//+
int MOTO1,MOTO2; 
float Target_Speed=0;
float Speed_YM=0,Speed_ZM;
float Remote_Control_Speed=0;
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{
		int PWM_Out;
		if(PBin(5)==0)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			//采集速度和mpu6050数据
			Encoder_Left=-Get_Speed(2);
			Encoder_Right=Get_Speed(4);
			
			MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);//角度
			MPU_Get_Gyroscope(&gx,&gy,&gz);//角速度
			MPU_Get_Accelerometer(&ax,&ay,&az);//加速度
			
			TraceLeft=-GetTrace_Left();
			TraceRight=GetTrace_Right();
			
			//蓝牙遥控
//			if(Trace==0)
//			{
//				
//				if(Fore==1)Target_Speed--;
//				if(Stop==1||(Fore==0&&Back==0))Target_Speed=0;
//				if(Back==1)Target_Speed++;
//				if(Left==1)Remote_Control_Speed+=5;
//				if(Right==1)Remote_Control_Speed-=5;
//				if((Left==0)&&(Right==0))Remote_Control_Speed=0;
//				else if((Left==1)||(Right==1)) Turn_Kd=0;
//			}
			//循迹模式
//			if(Trace==1)
//			{		
//				if(TraceLeft==1&&TraceRight==1)Target_Speed-=0.2,Remote_Control_Speed=0;//轨道中直行
//				else if(TraceRight==1&&TraceLeft==0)Target_Speed-=0.01,Remote_Control_Speed-=80;//左转
//				else if(TraceLeft==1&&TraceRight==0)Target_Speed-=0.01,Remote_Control_Speed+=80;//右转
//				else Remote_Control_Speed=0,Target_Speed=0;//出轨道
////			}
//			Target_Speed=Target_Speed>Speed_YM?Speed_YM:(Target_Speed<(-Speed_YM)?(-Speed_YM):Target_Speed);
			Remote_Control_Speed=Remote_Control_Speed>Speed_ZM?Speed_ZM:(Remote_Control_Speed<(-Speed_ZM)?(-Speed_ZM):Remote_Control_Speed);
			
			Velocity_Out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);//速度环
			Vertical_Out=Vertical(Velocity_Out+Med_Angle,Pitch,gy);//直立环
		Turn_Out=Turn(gx,Remote_Control_Speed);//转向环
			                                            
			//控制输出量
			PWM_Out=Vertical_Out;
			//控制
			MOTO1=PWM_Out-Turn_Out;
			MOTO2=PWM_Out+Turn_Out;
			Motor_Limit(&MOTO1,&MOTO2);
			Motor_SetSpeed(MOTO1,MOTO2);
//Motor_Stop(Med_Angle,Roll);
			//Debug_printf("%d,%d,%d\n",Encoder_Left,Encoder_Right,Target_Speed);
		}
	}
}
//P算法：Kp*误差
//I算法：Ki*误差积分
//D算法：Kd*误差微分



//直立环   期望角度   实际角度  Y轴角速度
//PD:Kp*Ek+Kd*Ek_D(误差微分)
int Vertical(float Med,float Angle,float gy)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gy-0);
	return PWM_out;
}
//速度环   左速度    右速度
//PI:Kp*Ek+Ki*Ek_S(误差积分)
int Velocity(float Target_Speed,int Encoder_Left,int Encoder_Right)
{
	static int PWM_Out,Encoder_Err,Encoder_S,Encoder_Err_LowOut,Encoder_Err_LowOut_Last;
	float a=0.7;
	Velocity_Ki=Velocity_Kp/200;
	Encoder_Err=(Encoder_Left+ Encoder_Right)-Target_Speed;//表示设置期望速度；
	//低通滤波
	Encoder_Err_LowOut=(1-a)*Encoder_Err+a*Encoder_Err_LowOut_Last;
	Encoder_Err_LowOut_Last=Encoder_Err_LowOut;
	Encoder_S=Encoder_Err_LowOut+Encoder_S;
	//积分限幅
	Encoder_S=Encoder_S>20000?20000:(Encoder_S<(-20000)?(-20000):Encoder_S);
	if(Stop==1)Encoder_S=0,Stop=0;
	PWM_Out=Velocity_Kp*Encoder_Err_LowOut+Velocity_Ki*Encoder_S;
	return PWM_Out;
}
//转向环 非规范PD
int Turn(short gz,int Remote_Control)
{
	int PWM_Out;
	PWM_Out=Turn_Kp*Remote_Control+Turn_Kd*gz;
	return PWM_Out;
}
void Motor_Stop(float Med,float Angle)
{
	if(Motor_abs(Med-Angle)>75)
	{
		Motor_SetSpeed(0,0);
		Stop=1;
	}
}
