#ifndef __USART_H__
#define __USART_H__
#include "sys.h"

void USART3_Init(u32 bound);
void USART3_IRQHandler(void);
extern u8 Fore,Back,Left,Right,Stop,Trace;
void USART3_Send_String(char* String);
void USART3_Send_Data(char Data);
void Debug_printf(char *format,...);
#endif

