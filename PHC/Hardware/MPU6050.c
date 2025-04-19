#include "stm32f10x.h"                  // Device header
#include "MYiic.h"
#include "MPU6050_Reg.h"

#define  MPU6050_ADDRESS  0XD0
void MPU6050_Writereg(uint8_t Regaddress,uint8_t 	Data)
{
	MYIIC_Start();
	MYIIC_Send(MPU6050_ADDRESS );
	MYIIC_Receiveack();
	MYIIC_Send(Regaddress);
	MYIIC_Receiveack();
	MYIIC_Send(Data);
	MYIIC_Receiveack();
	MYIIC_Stop();
}
uint8_t MPU6050_Readreg(int8_t Regaddress)
{
	uint8_t Data;
	MYIIC_Start();
	MYIIC_Send(MPU6050_ADDRESS );
	MYIIC_Receiveack();
	MYIIC_Send(Regaddress);
	MYIIC_Receiveack();
	MYIIC_Start();
	MYIIC_Send(MPU6050_ADDRESS|0x01);
	MYIIC_Receiveack();
	Data=MYIIC_Receive();
	MYIIC_Sendack(1);
	MYIIC_Stop();
	return Data;
}
void MPU6050_Init()
{
	MYIIC_Init();
	MPU6050_Writereg(MPU6050_PWR_MGMT_1,0X01);
	MPU6050_Writereg(MPU6050_PWR_MGMT_2,0X00);
	MPU6050_Writereg(MPU6050_SMPLRT_DIV	,0X09);
	MPU6050_Writereg(MPU6050_CONFIG,0X06);
	MPU6050_Writereg(MPU6050_GYRO_CONFIG,0X18);
	MPU6050_Writereg(MPU6050_ACCEL_CONFIG,0X18);
}
uint8_t MPU6050_GetID()
{
	return MPU6050_Readreg(MPU6050_WHO_AM_I);
}
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint16_t Datah,Datal;
	Datah=MPU6050_Readreg(MPU6050_ACCEL_XOUT_H);
	Datal=MPU6050_Readreg(MPU6050_ACCEL_XOUT_L);
	*AccX=(Datah<<8)|Datal;
	Datah=MPU6050_Readreg(MPU6050_ACCEL_YOUT_H);
	Datal=MPU6050_Readreg(MPU6050_ACCEL_YOUT_L);
	*AccY=(Datah<<8)|Datal;
	Datah=MPU6050_Readreg(MPU6050_ACCEL_ZOUT_H);
	Datal=MPU6050_Readreg(MPU6050_ACCEL_ZOUT_L);
	*AccZ=(Datah<<8)|Datal;
	Datah=MPU6050_Readreg(MPU6050_GYRO_XOUT_H);
	Datal=MPU6050_Readreg(MPU6050_GYRO_XOUT_L);
	*GyroX=(Datah<<8)|Datal;
	Datah=MPU6050_Readreg(MPU6050_GYRO_YOUT_H);
	Datal=MPU6050_Readreg(MPU6050_GYRO_YOUT_L);
	*GyroY=(Datah<<8)|Datal;
	Datah=MPU6050_Readreg(MPU6050_GYRO_ZOUT_H);
	Datal=MPU6050_Readreg(MPU6050_GYRO_ZOUT_L);
	*GyroZ=(Datah<<8)|Datal;
}
void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;/**【1】**///GPIO_Mode_AF_PP
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;//PB5配置为上拉输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);//
	
	EXTI_InitStruct.EXTI_Line=EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
}
