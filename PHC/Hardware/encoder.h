#ifndef __ENCODER_H
#define __ENCODER_H
void Encoder_TIM2_Init(void);
void Encoder_TIM4_Init(void);
uint8_t Read_Speed(uint8_t TIMx);
#endif
