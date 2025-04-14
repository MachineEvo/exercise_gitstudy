#ifndef  _CONTROL_H
#define  _CONTROL_H
#include "stm32f10x.h"                  // Device header

#include "TCRT5000.h"

#define L Get_TrackingState(GPIO_Pin_4)
#define M Get_TrackingState(GPIO_Pin_5)
#define R Get_TrackingState(GPIO_Pin_6)

void TrackingFunc(u32 ms);
int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);
void EXTI9_5_IRQHandler(void);

extern char stop;
extern u8 Fore,Back,Left,Right;
extern int PWM_MAX,PWM_MIN;
extern int MOTO1,MOTO2;
extern float Pitch,Roll,Yaw;		//角度
extern short gyrox,gyroy,gyroz;	//陀螺仪--角速度
extern short aacx,aacy,aacz;		//加速度
extern int Encoder_Left,Encoder_Right;
void NVIC_Config(void);

#endif

