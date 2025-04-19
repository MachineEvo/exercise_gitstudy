#ifndef __USART3_H
#define __USART3_H
void USART3_Send_String(char *String);
void uart3_init(int bound);					
void USART3_IRQHandler(void);     	
#endif
