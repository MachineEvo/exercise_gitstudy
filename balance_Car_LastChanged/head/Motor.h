#ifndef __MOTOR_H__
#define __MOTOR_H__
#include "sys.h"
//extern int MOTO1,MOTO2;
//extern int PWM_MAX=7200;
//extern int PWM_MIN=-7200;
void Motor_PWM_Init(u16 Psc,u16 Per);
void Motor_PWM_SetCompare1(uint32_t Compare);
void Motor_PWM_SetCompare4(uint32_t Compare);
void Motor_SetSpeed(int Motor1,int Motor2);
void Motor_Init(void);
void Motor_Limit(int*motoA,int*motoB);
int Motor_abs(int p);
extern int MotorFlag1,MotorFlag2;
#endif
