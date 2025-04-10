#include "stm32f10x.h"                  // Device header

//遇黑->输出高电平->灯熄灭
void Trace_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//返回1右转//1遇到白色
int GetTrace_Left(void)
{
	int TraceLeft;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0) TraceLeft=1;
	else TraceLeft=0;
	return TraceLeft;
}
//返回1左转//1遇到白色
int GetTrace_Right(void)
{
	int TraceRight;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0) TraceRight=1;
	else TraceRight=0;
	return TraceRight;
}
//返回1前进//全黑前进
