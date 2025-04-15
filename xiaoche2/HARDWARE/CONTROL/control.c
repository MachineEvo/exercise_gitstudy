#include "control.h"
float Med_Angle=0;
float Target_Speed=0;	
float Turn_Speed=0;	

float 
	Vertical_Kp=250,//直立环KP、KD
	Vertical_Kd=0.6;
float 
	Velocity_Kp=0.65,//速度环KP、KI
	Velocity_Ki=0.00325;
float 
	Turn_Kd=0.6,//转向环KP、KD
	Turn_Kp=20;
extern int length;
#define SPEED_Y 15
#define SPEED_Z 80


int Vertical_out,Velocity_out,Turn_out;

int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 

void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	{
		if(PBin(5)==0)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			
			
			Encoder_Left=-Read_Speed(2);
			Encoder_Right=Read_Speed(4);
			
			mpu_dmp_get_data(&Roll,&Pitch,&Yaw);			
			MPU_Get_Gyroscope(&gyroy,&gyrox,&gyroz);	
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
			
			
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);
			Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);
			
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	
			Vertical_out=Vertical(Velocity_out+Med_Angle,Pitch,gyroy);			
			Turn_out=Turn(gyroz,Turn_Speed);																
			
			PWM_out=Vertical_out+Velocity_out;
			
			MOTO1=PWM_out+Turn_out;
			MOTO2=PWM_out-Turn_out;
			Limit(&MOTO1,&MOTO2);	 			
			Load(MOTO1,-MOTO2);		
			
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
	float a=0.7;
	
	//1.计算速度偏差
	Encoder_Err=((encoder_left+encoder_right)-Target);
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;
	EnC_Err_Lowout_last=EnC_Err_Lowout;
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
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

