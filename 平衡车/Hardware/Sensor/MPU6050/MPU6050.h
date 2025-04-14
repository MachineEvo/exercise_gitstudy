#ifndef __MPU6050_H
#define __MPU6050_H

u8 MPU_SendByte(u8 reg,u8 data);
u8 MPU_Send_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_ReadByte(u8 reg);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU6050_Init(void);
u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);
void MPU6050_EXTI_Init(void);

#endif
