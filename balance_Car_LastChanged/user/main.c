#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "EXTI.h"
#include "Encoder.h"
#include "Usart.h"
#include "Trace.h"
#include "HC_SR04.H"
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
int main(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	OLED_Init();
	OLED_Clear();
	
	MPU6050_IIC_IO_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	MPU6050_EXTI_Init();
	Motor_PWM_Init(0,7199);
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
  Motor_Init();
	USART3_Init(9600);
	//HC_SR04Init();
	Trace_Init();
	while(1){
//OLED_ShowNum(1,5,TimeCount,5);
		//OLED_ShowNum(1,1,Sonar(),4);
		OLED_ShowSignedNum(2,7,Get_Speed(2),2);
		OLED_ShowSignedNum(2,1,Get_Speed(4),5);
		OLED_ShowSignedNum(3,1,Yaw,2);
		OLED_ShowSignedNum(3,5,gz,3);
		OLED_ShowNum(4,1,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6),2);
		OLED_ShowNum(4,6,GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5),2);
	}
}
