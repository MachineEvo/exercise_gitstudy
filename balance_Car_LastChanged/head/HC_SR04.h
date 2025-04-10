#ifndef __HC_SR04_H__
#define __HC_SR04_H__
extern uint16_t TimeCount;
void HC_SR04Init(void);
float Sonar(void);
void TIM3_IRQHandler(void);
#endif
