#ifndef __IIC_H
#define __IIC_H
#include "Delay.h"

#define SDA_SET {GPIO_SetBits(GPIOB,GPIO_Pin_3);Delay_us(10);}
#define SDA_RESET {GPIO_ResetBits(GPIOB,GPIO_Pin_3);Delay_us(10);}
#define SCL_SET {GPIO_SetBits(GPIOB,GPIO_Pin_4);Delay_us(10);}
#define SCL_RESET {GPIO_ResetBits(GPIOB,GPIO_Pin_4);Delay_us(10);}
#define SDA_GET GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define SCL_GET GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(u8 Byte);
u8 IIC_ReceiveByte(void);
void IIC_SendAck(u8 AckBit);
u8 IIC_ReceiveAck(void);

#endif
