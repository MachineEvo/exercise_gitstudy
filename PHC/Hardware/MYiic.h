#ifndef __MYIIC_H
#define __MYIIC_H
void MYIIC_W_SCL(uint8_t BitValue);
void MYIIC_W_SDA(uint8_t BitValue);
uint8_t MYIIC_R_SDA(void);
void MYIIC_Init(void);
void MYIIC_Start(void);
void MYIIC_Stop(void);
void MYIIC_Send(uint8_t Byte);
uint8_t MYIIC_Receive(void);
void MYIIC_Sendack(uint8_t Byte);
uint8_t MYIIC_Receiveack(void);
#endif
