#include "stm32f10x.h"     // Device header
#include "sys.h"
#include <stdio.h>
#include <stdarg.h>
char Stringddd[100];
void USART3_Init(u32 bound)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}
u8 Fore,Back,Left,Right,Stop,Trace;
void USART3_IRQHandler(void)
{
	int Bluetooth_data;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//接收中断标志位
	{
			USART3_Send_Data(0x52);
		Bluetooth_data=USART_ReceiveData(USART3);
			if(Bluetooth_data==0x09)//开始循迹
		{
			Trace=1;
			Fore=0,Back=0,Left=0,Right=0,Stop=0;
			USART3_Send_Data(1);
		}
			else if(Bluetooth_data==0x00)//刹车
		{
			Stop=1;
			Fore=0,Back=0,Left=0,Right=0;
			USART3_Send_Data(2);
		}
			else if(Bluetooth_data==0x01)//前进
		{
			Fore=1;
			Stop=0,Back=0,Left=0,Right=0;
			USART3_Send_Data(3);
		}
			else if(Bluetooth_data==0x05)//后退
		
			Back=1;
			Stop=0,Fore=0,Left=0,Right=0;
			USART3_Send_Data(4);
		}
			else if(Bluetooth_data==0x03)//左转
		{
			Left=1;
			Stop=0,Back=0,Fore=0,Right=0;
			USART_SendData(USART3,3);
		}
			else if(Bluetooth_data==0x07)//右转
		{
			Right=1;
			Stop=0,Back=0,Left=0,Fore=0;
			USART3_Send_Data(6);
		}
		else Stop=1,Fore=0,Back=0,Left=0,Right=0,Trace=0;
		USART3_Send_Data(0);
	}
void USART3_Send_Data(char Data)
{
	USART_SendData(USART3,Data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
}
void USART3_Send_String(char* String)
{
	u16 len,j;
	len=strlen(String);
	for(j=0;j<len;j++)
	{
	USART3_Send_Data(*String++);
	}
}

//#include <stdarg.h>
//#include <stdio.h>
//void Debug_printf(char *fmt, ...)
//{
//    char printf_buf[100]; 
//    uint16_t len = 0;
//    va_list ap;
//    va_start(ap, fmt);
//    vsprintf((char *)printf_buf, fmt, ap);
//    va_end(ap);
//    len = strlen((const char *)printf_buf); 
//	
//	  USART3_Send_String(printf_buf);
//}

