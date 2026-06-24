#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Motor.h"

uint16_t Openmv_leftline_x;
uint16_t Openmv_leftline_y;
uint16_t Openmv_middleline;
uint16_t Openmv_rightline_x;
uint16_t Openmv_rightline_y;
uint16_t Object_Distance;
uint16_t Barrier_Distance;
uint16_t Openmv_Data[13];//接收9个数据


void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1,ENABLE);
	
	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t Serial_ReceiveDate(void)
{
	return USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		static uint16_t i;
		Openmv_Data[i] = USART_ReceiveData(USART1);
		i++;
		if(Openmv_Data[0]!=0xA3)
		{
			i = 0;
		}
		if((i == 2)&&(Openmv_Data[1] != 0xB3))
		{
			i = 0;
		}
		if(i==5&&Openmv_Data[4]==0xC3)
		{
			Object_Distance = Openmv_Data[2];
			Barrier_Distance = Openmv_Data[3];
			i = 0;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
}

