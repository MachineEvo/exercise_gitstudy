#include "stm32f10x.h"                  // Device header
#include "infrared transducer.h"
#include "MPU6050.h"
#include "Serial.h"
#include "Ultrasonic sensor.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "Exit.h"
#include "PID.h"

int speed =0,turn=0,distance;
int state = 0;


int main(void){
	OLED_Init();
	MPU6050_Init();
	EXIT_Init();
	Motor_Init();
	Encoder_Init();
	Timer_Init();
	Serial_Init();
	Infared_Transducer_Init();
	Ultrasonic_Sensor_Init();
	OLED_ShowChar(3,1,'a');
	while (1)
	{

		
		
		
		
	
//		if (state == 0 ){
//			speed=0;
//			turn=0;
//			continue;
//		}else if (state == 1){
//			Infared_Transducer_Tracking();
//			continue;
//		}else if (state ==2){
//			if(distance<6){
//				speed=10;
//				turn=5;
//				Delay_s(5);
//			}else{
//				speed=10;
//				turn=0;
//			}
//		}
//		  
//		
	}
}


