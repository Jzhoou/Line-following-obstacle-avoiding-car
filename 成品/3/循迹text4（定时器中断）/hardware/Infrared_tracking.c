#include "stm32f10x.h"                  // Device header

/**
  * @brief  红外循迹模块初始化
  * @param  无
  * @retval 无
  */
void InfraredTracking_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);

}
/**
  * @brief  获取左数第一个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_L1Data(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);
}
/**
  * @brief  获取左数第二个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_L2Data(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
}
/**
  * @brief  获取右数第二个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_R2Data(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}
/**
  * @brief  获取右数第一个红外循迹模块数据
  * @param  无
  * @retval 0/1（白0黑1）
  */
uint16_t Get_R1Data(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
}
