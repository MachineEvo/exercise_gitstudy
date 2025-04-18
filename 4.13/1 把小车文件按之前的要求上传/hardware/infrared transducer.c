#include "stm32f10x.h"                  // Device header

#define LEFT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define RIGHT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define BLACK 1
#define WHITE 0

extern int state,speed,turn;

void Infared_Transducer_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

uint8_t Infared_Transducer_GetStatus(char dv){
	if (dv=='L') return LEFT;
	else return RIGHT;
}

void Infared_Transducer_Tracking(void){
	if(LEFT == BLACK &&RIGHT==BLACK){
		state = 0;
	}else if(LEFT == WHITE && RIGHT==WHITE){
		speed = 10;
		turn = 0;
	}else if(LEFT == BLACK &&RIGHT==WHITE){
		speed = 10;
		turn = 10;
	}else if(LEFT == WHITE &&RIGHT==BLACK){
		speed = 10;
		turn = -10;
	}
}

