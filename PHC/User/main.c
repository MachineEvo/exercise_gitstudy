#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "motor.h"
#include "tcrt5000.h"
#include "PWM.h"
#include "Timer.h"
#include "MPU6050.h"
#include "NVIC.h"
#include "encoder.h"
#include "usart3.h"
#include "usart.h"
#include "sensor.h"
extern int Fore,Back,Left,Right,MOTO1,MOTO2,Encoder_Left,Encoder_Right,length;
extern float Pitch,Roll,Yaw;		
extern short gyrox,gyroy,gyroz,aacx,aacy,aacz;		
int main(void)
{
	NVIC_Config();
	uart1_init(115200);
  uart3_init(9600);
	MPU6050_Init();
	MPU6050_EXTI_Init();
	OLED_Init();
	Motor_Init();
	Motor_Init();
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Timer_Init();
	tcrt5000_init();
	OLED_ShowString(1,1,"jiao du:");
	OLED_ShowString(2,1,"ju li:");
	OLED_ShowString(3,1,"sd:");
	while (1)
	{
		length=sensor();
		OLED_ShowFNum(1,10,Pitch,2,1);
		OLED_ShowFNum(2,8,length,4,2);
		OLED_ShowNum(3,5,(int)((Encoder_Left+Encoder_Right)*2.38),3);
	}
}
