#ifndef __CONTROL_H__
#define __CONTROL_H__
extern void EXTI9_5_IRQHandler(void);
int Velocity(float Target_Speed,int Encoder_Left,int Encoder_Right);
int Vertical(float Med,float Angle,float gx);
int Turn(short gz,int Remote_Control);
void Motor_Stop(float Med,float Angle);
extern float Target_Speed,Remote_Control_Speed;
#endif
