#include "stm32f10x.h"
#include "sys.h" 
#include "sensor.h"
#include "Track.h"
#include "control.h"

extern int MOTO1,MOTO2;

float Pitch,Roll,Yaw;						
short gyrox,gyroy,gyroz;				
short aacx,aacy,aacz;						
int Encoder_Left,Encoder_Right;

int PWM_MAX=5000,PWM_MIN=-5000;	
int MOTO1,MOTO2;								

extern int Vertical_out,Velocity_out,Turn_out;
extern float Target_Speed;
extern float Turn_Speed;
int main(void)	
{	

	Track_Init();
	
	delay_init();
	NVIC_Config();
	uart1_init(115200);	
	uart3_init(9600);
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM1(0,7199);
  while(1)	
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
		{
			OLED_Num2(1,1,0);
		}
		else
		{
			OLED_Num2(1,1,1);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		{
			OLED_Num2(1,2,0);
		}
		else
		{
			OLED_Num2(1,2,1);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		{
			delay_ms(1000);
			Turn_Speed=0;
			Target_Speed=4;
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)
		{
			Target_Speed=0.5;
			MOTO1=-10;
			Turn_Speed=-40;
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==1)
		{
			Target_Speed=0.5;
			MOTO2=-10;
			Turn_Speed=40;
		}
	} 	
}
