#include "stm32f10x.h"                  // Device header

/**
  * @brief  红外循迹模块初始化
  * @param  无
  * @retval 无
  */
  //B12 13 14 15
  // A8 11 12 B5
void InfraredTracking_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct2.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStruct2.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct2);

}
/**
  * @brief  获取左数第一个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_L1Data(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}
/**
  * @brief  获取左数第二个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_L2Data(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
}
/**
  * @brief  获取右数第二个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_R2Data(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11);
}
/**
  * @brief  获取右数第一个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_R1Data(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
}
