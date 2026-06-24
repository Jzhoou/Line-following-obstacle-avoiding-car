#include "stm32f10x.h"                  // Device header
#include "buzzer.h"

void Buzzer_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void Buzzer_state(uint16_t state)
{
	if(state==ENABLE)
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	else 
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
