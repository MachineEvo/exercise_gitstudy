#ifndef __SERIAL_H
#define __SERIAL_H

void Serial_SendByte(uint8_t Byte);
void Serial_ReceiveByte(uint8_t *Byte);
void Serial_SendArray(uint8_t *Array,uint8_t Size);
void Serial_ReceiveArray(uint8_t *Array,uint8_t Size);
void Serial_SendString(char *str);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
uint32_t CRC_Calculate(uint32_t *Byte, uint8_t Length);

#endif
