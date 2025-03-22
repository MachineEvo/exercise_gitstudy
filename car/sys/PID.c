#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"
#include "Encoder.h"
#include "Motor.h"
#include <math.h>
extern int speed,turn;
int SpeedL,SpeedR;
float roll ,pitch ,yaw;
short AX, AY, AZ, GX, GY, GZ;	
float Med_Angel = 0; //机械中值
float Vertical_kp=0,
	  Vertical_Kd=0,
	  velocity_Kp=0,
	  velocity_Ki=0,
	  Turn_Kp=0,
	  Turn_Kd=0;


int Vertical_out,velocity_out,Turn_out;
int MOTO1,MOTO2;

//直立环PD控制器
//kp*角度偏差+Kd角度偏差的微分
//输入 期望角度 真实角度 真实角速度
int Vertical(float Med,float Angle,float gyro_Y){
	int PWM_out;
	
	PWM_out = (Vertical_kp-Med)*Angle+Vertical_Kd*gyro_Y;
	return PWM_out;
}

//速度环PI控制器
//Kp2*编码器偏差-Ki*编码器偏差的积分
//输入 左编码器 右编码器
int velocity(int Encoder_L,int Encoder_R,int Speed){
	int Out;
	int Encoder_Err,Encoder_Err_Lowout;
	static int Encoder_Err_Lowout_Last,Encoder_S;
	Encoder_Err = Encoder_L+Encoder_R-Speed*2;
	//低通滤波
	Encoder_Err_Lowout = (1-0.7)*Encoder_Err+0.7*Encoder_Err_Lowout_Last;
	Encoder_Err_Lowout_Last=Encoder_Err_Lowout;
	
	Encoder_S+=Encoder_Err_Lowout;
	
	Out =velocity_Kp*Encoder_Err_Lowout+velocity_Ki*Encoder_S;
	
	return Out;
}

//转向环
int Turn(float gyro_Z,int Tarfet_Turn){
	int Out;
	Out =Turn_Kp*Tarfet_Turn+Turn_Kd*gyro_Z;
	return Out;
}

void EXTI9_5_IRQHandler(void){
	int Out;
	if(EXTI_GetITStatus(EXTI_Line5)==SET){
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		const float a = 0.95238;
		float roll_a=atan2(AY,AZ)/3.141593f * 180.0f;
		float pitch_a=-atan2(AX,AZ)/3.141593f * 180.0f;
		
		float yaw_g = yaw + GZ*0.005;
		float roll_g = roll + GX*0.005;
		float pitch_g = pitch + GY*0.005;
		
		yaw = yaw_g;
		roll = a*roll_g+(1-a)*roll_a;
		pitch = a*pitch+(1-a)*pitch_a;
		SpeedL = Encoder2_Get();
		SpeedR = Encoder4_Get();
		EXTI_ClearITPendingBit(EXTI_Line5);
		velocity_out=velocity(SpeedL,SpeedR,speed);
		Vertical_out=Vertical(velocity_out+Med_Angel,roll,GY);
		
		Turn_out=Turn(GZ,turn);
		
		Out = Vertical_out-velocity_Kp*Vertical_out;
		
		MOTO1= Out+Turn_out;
		MOTO2= Out-Turn_out;
		if(MOTO1>100)MOTO1=100;
		if(MOTO1<-100)MOTO1=-100;
		if(MOTO2>100)MOTO2=100;
		if(MOTO2<-100)MOTO2=-100;
		Motor_SetRSpeed(MOTO1);
		Motor_SetLSpeed(MOTO2);
	}
}


