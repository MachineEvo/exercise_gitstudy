#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "sys.h"
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);
int Get_Speed(uint32_t TIMx);
void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
#endif
