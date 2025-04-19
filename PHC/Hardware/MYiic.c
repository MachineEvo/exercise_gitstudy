#include "stm32f10x.h"                  // Device header
#include "delay.h"
void MYIIC_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);
}

void MYIIC_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}

uint8_t MYIIC_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}
void MYIIC_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}
void MYIIC_Start()
{
	MYIIC_W_SDA(1);
	MYIIC_W_SCL(1);
	MYIIC_W_SDA(0);
	MYIIC_W_SCL(0);
}
void MYIIC_Stop()
{
	MYIIC_W_SDA(0);
	MYIIC_W_SCL(1);
	MYIIC_W_SDA(1);
}
void MYIIC_Send(uint8_t Byte)
{
	uint8_t i;
	for (i=0;i<8;i++)
	{
	  MYIIC_W_SDA(Byte&(0X80>>i));
	  MYIIC_W_SCL(1);
	  MYIIC_W_SCL(0);
	}
}
uint8_t MYIIC_Receive()
{
	uint8_t i,Byte=0x00;
	MYIIC_W_SDA(1);
	for (i=0;i<8;i++)
	{
	MYIIC_W_SCL(1);
	if (MYIIC_R_SDA()==1)
		Byte|=(0x80>>i);
	MYIIC_W_SCL(0);
  }
	return Byte;
}
void MYIIC_Sendack(uint8_t Byte)
{
	MYIIC_W_SDA(Byte);
	MYIIC_W_SCL(1);
	MYIIC_W_SCL(0);
}
uint8_t MYIIC_Receiveack()
{
	uint8_t Byte=0x00;
	MYIIC_W_SDA(1);
  MYIIC_W_SCL(1);
  Byte=MYIIC_R_SDA();
	MYIIC_W_SCL(0);
	
	return Byte;
}
