#ifndef __EXTI_H__
#define __EXTI_H__
#include "mpu6050.h"
#include "inv_mpu.h"
#include "Motor.h"
extern float Pitch,Roll,Yaw;//¸©Ñö½Ç£ÄÄ¬ÈÏ¸úÖÐÖµÒ»Ñù£¬·­¹ö½Ç£¬Æ«º½½Ç
extern short ax,ay,az,gx,gy,gz;
void MPU6050_EXTI_Init(void);
void EXTI9_5_IRQHandler(void);
#endif
