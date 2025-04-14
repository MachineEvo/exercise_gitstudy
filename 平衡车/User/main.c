#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "HC-SR04.h"
#include "BLUETOOTH.h"
#include "TCRT5000.h"

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）
int length;

int MOTO1,MOTO2;	

int main(void)
{
	
}
