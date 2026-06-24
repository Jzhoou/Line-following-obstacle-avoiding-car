#include "stm32f10x.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
}

void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1))
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_1,Bit_SET);
	}
	
}

void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2))
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
	}
	
}
