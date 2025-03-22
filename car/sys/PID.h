#ifndef __PID_H
#define __PID_H

int Vertical(float Med,float Angle,float gyro_Y);
int velocity(int Encoder_L,int Encoder_R,int Speed);
int Turn(int gyro_Z);

#endif
