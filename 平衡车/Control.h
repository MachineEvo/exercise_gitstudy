#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "sys.h" 
#include "TCRT5000.h"

#define L Get_TrackingState(GPIO_Pin_4)
#define M Get_TrackingState(GPIO_Pin_5)
#define R Get_TrackingState(GPIO_Pin_6)

void TrackingFunc(u32 ms);
int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC);
void EXTI9_5_IRQHandler(void);
#endif

