#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

void BlueTooth_Init(void);
void Bluetooth_SendData(u8 Byte);
void Bluetooth_SendString(char *String);
void Bluetooth_SendNumber(uint32_t Number, uint8_t Length);

#endif
