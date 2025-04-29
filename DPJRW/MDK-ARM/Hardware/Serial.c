#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"
#include "math.h"
#include "OLED.h"
#include "crc.h"

void Serial_SendByte(uint8_t Byte)
{
	HAL_UART_Transmit(&huart1, &Byte, sizeof(uint8_t), 0x400);	
}

void Serial_ReceiveByte(uint8_t *Byte)
{
	HAL_UART_Receive(&huart1, Byte, sizeof(uint8_t), 0x400);	
}

void Serial_SendArray(uint8_t *Array,uint8_t Size)
{
	HAL_UART_Transmit(&huart1, Array,Size,0x400);	
}

void Serial_ReceiveArray(uint8_t *Array,uint8_t Size)
{
	HAL_UART_Receive(&huart1, Array, Size, 0x400);	
}

void Serial_SendString(char *str)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 0x400);
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

uint32_t CRC_Calculate(uint32_t *Byte, uint8_t Length)
{
	uint32_t crc = 0;
	crc = HAL_CRC_Calculate(&hcrc,(uint32_t *)Byte, Length);
	return crc;
}


