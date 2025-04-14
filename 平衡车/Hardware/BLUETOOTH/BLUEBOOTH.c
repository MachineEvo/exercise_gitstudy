#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "String.h"
#include "math.h"


u8 Fore,Back,Left,Right,Tracking,Avoiding;
extern float Vertical_Kp,Vertical_Kd;
extern float Velocity_Kp,Velocity_Ki;

void BlueTooth_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//PB10为单片机TX端，PB11为单片机RX端
	GPIO_InitTypeDef GPIODEF;
    GPIODEF.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIODEF.GPIO_Pin = GPIO_Pin_10;
	GPIODEF.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIODEF);
	
    GPIODEF.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIODEF.GPIO_Pin = GPIO_Pin_11;
	GPIODEF.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIODEF);

	//USART初始化
	USART_InitTypeDef USARTDEF;
    USARTDEF.USART_BaudRate = 9600;
	USARTDEF.USART_WordLength = USART_WordLength_8b;
	USARTDEF.USART_StopBits = USART_StopBits_1;
	USARTDEF.USART_Parity = USART_Parity_No;
	USARTDEF.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTDEF.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USARTDEF);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE); 
	
	
	//配置NVIC通道
	NVIC_InitTypeDef NVICDEF;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVICDEF.NVIC_IRQChannel=USART3_IRQn;
	NVICDEF.NVIC_IRQChannelCmd=ENABLE;
	NVICDEF.NVIC_IRQChannelPreemptionPriority=2;
	NVICDEF.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVICDEF);
}	

void Bluetooth_SendData(u8 Byte)
{
	USART_SendData(USART3,Byte);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);
}

void Bluetooth_SendString(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		Bluetooth_SendData(*String++);
	}
}

void Bluetooth_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Bluetooth_SendData(Number / (u8)pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}



void USART3_IRQHandler(void)
{
	int Bluetooth_data;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		Bluetooth_data=USART_ReceiveData(USART3);
		
		/*控制指令*/
		//0x1113：后退    0x1115：前进    0x1117：右转    0x1119：左转    0x0000：停止
		if(Bluetooth_data==0x1111)Fore=0,Back=0,Left=0,Right=0;
		else if(Bluetooth_data==0x1113)Fore=1,Back=0,Left=0,Right=0,Tracking = 0;
		else if(Bluetooth_data==0x1115)Fore=0,Back=1,Left=0,Right=0,Tracking = 0;
		else if(Bluetooth_data==0x1117)Fore=0,Back=0,Left=0,Right=1,Tracking = 0;
		else if(Bluetooth_data==0x1119)Fore=0,Back=0,Left=1,Right=0,Tracking = 0;
		else if(Bluetooth_data==0x1121)Fore=0,Back=0,Left=0,Right=0,Tracking = 1;
		
		
		
		/*调试指令*/
		//0x09：直立环Kp+10    0x11：直立环Kp-10    0x13：直立环Kd+0.2    0x15直立环Kd-0.2
		//0x21：直立环x0.6
		//0x17：速度环Kp+0.1,Ki+0.0005    0x17：速度环Kp-0.1,Ki-0.0005
		else if(Bluetooth_data==0x09){Vertical_Kp+=10;Bluetooth_SendString(" Vertical_Kp：");
		Bluetooth_SendNumber((int)(Vertical_Kp),3);}
		else if(Bluetooth_data==0x11){Vertical_Kp-=10;Bluetooth_SendString(" Vertical_Kp：");
		Bluetooth_SendNumber((int)(Vertical_Kp),3);}
		else if(Bluetooth_data==0x21)
		{
		Vertical_Kd = Vertical_Kd * 0.6 ;Vertical_Kp = Vertical_Kp * 0.6 ;
		Bluetooth_SendString(" Vertical_Kp：");
		Bluetooth_SendNumber((int)(Vertical_Kp),3);
		Bluetooth_SendString(" Vertical_Kd：");
		Bluetooth_SendNumber((int)(Vertical_Kd * 100),3);
		}
		else if(Bluetooth_data==0x13){Vertical_Kd+=0.2;Bluetooth_SendString(" Vertical_Kd：");
		Bluetooth_SendNumber((int)(Vertical_Kd * 100),3);}
		else if(Bluetooth_data==0x15){Vertical_Kd-=0.2;Bluetooth_SendString(" Vertical_Kd：");
		Bluetooth_SendNumber((int)(Vertical_Kd * 100),3);}
		else if(Bluetooth_data==0x17){Velocity_Kp+=0.1;Velocity_Ki = Velocity_Kp * 0.005;Bluetooth_SendString(" Velocity_Kp：");
		Bluetooth_SendNumber((int)(Velocity_Kp * 100),3);}
		else if(Bluetooth_data==0x19){Velocity_Kp-=0.1;Velocity_Ki = Velocity_Kp * 0.005;Bluetooth_SendString(" Velocity_Kp：");
		Bluetooth_SendNumber((int)(Velocity_Kp * 100),3);}

		else													Fore=0,Back=0,Left=0,Right=0;
	}
	
	
	
}
