#include "stm32f10x.h"                  // Device header
#include "IIC.h"


void IIC_Init(void)
{
	//将PB3,PB4重定义为普通IO口
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    //PB4是SCL，PB3是SDA                    
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);		
	GPIO_SetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4);	
}

void IIC_Start(void)
{
	SDA_SET;
	SCL_SET;
 	SDA_RESET;
	SCL_RESET;
}	 

void IIC_Stop(void)
{
	SDA_RESET;
	SCL_SET;
	SDA_SET;
}

void IIC_SendByte(u8 Byte)
{
	u8 i;
	for (i = 0; i < 8; i ++)
	{
		if(Byte & (0x80 >> i)){SDA_SET;}
		else {SDA_RESET;}
		SCL_SET;
		SCL_RESET;
	}
}

u8 IIC_ReceiveByte(void)
{
	u8 i, Byte = 0x00;
	SDA_SET;
	for (i = 0; i < 8; i ++)
	{
		SCL_SET;
		if (SDA_GET == 1){Byte |= (0x80 >> i);}
		SCL_RESET;
	}
	return Byte;
}

//ACK为1表示应答，0表示不应答
void IIC_SendAck(u8 AckBit)
{
	if(AckBit){SDA_SET;}
	else{SDA_RESET;}
	SCL_SET;
	SCL_RESET;
}

u8 IIC_ReceiveAck(void)
{
	u8 AckBit;
	SDA_SET;
	AckBit = SDA_GET;
	SCL_RESET;
	return AckBit;
}
