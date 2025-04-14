#ifndef __HCSR04_H
#define __HCSR04_H

#define TRIG_SET GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define TRIG_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define ECHO_SET GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define ECHO_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define ECHO GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
int Senor_Using(void);
void HCSR04_Init(void);

#endif
