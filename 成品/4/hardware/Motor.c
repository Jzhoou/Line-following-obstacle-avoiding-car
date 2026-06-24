#include "stm32f10x.h"                  // Device header
#include "PWM1.h"
#include "Delay.h"

extern uint16_t L1,L2,R2,R1;

/**
  * @brief  直流电机初始化
  * @param  无
  * @retval 无
  */
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct2.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct2.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct2);
	
	PWM1_Init();

}

/**
  * @brief  直行
  * @param  无
  * @retval 无
  */


void go(void)
{
//四轮全正
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare2(TIM2,100);
		TIM_SetCompare1(TIM2,100);
		TIM_SetCompare3(TIM2,100);
		TIM_SetCompare4(TIM2,100);

}
/**
  * @brief  后退
  * @param  无
  * @retval 无
  */
void turnaround(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后
		
//		TIM_Cmd(TIM3, DISABLE);
		do
		{
			TIM_SetCompare2(TIM2,100);
			TIM_SetCompare1(TIM2,100);
			TIM_SetCompare3(TIM2,100);
			TIM_SetCompare4(TIM2,100);
		}while((L1!=1)||(L2!=0)||(R2!=0)||(R1!=0));

}
/**
  * @brief  小幅度左转
  * @param 	无
  * @retval 无
  */
void left(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare2(TIM2,40);
		TIM_SetCompare1(TIM2,100);
		TIM_SetCompare3(TIM2,40);
		TIM_SetCompare4(TIM2,100);

}
/**
  * @brief  小幅度右转
  * @param  无
  * @retval 无
  */
void right(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare2(TIM2,100);
		TIM_SetCompare1(TIM2,40);
		TIM_SetCompare3(TIM2,100);
		TIM_SetCompare4(TIM2,40);

}
/**
  * @brief  中幅度左转，直到L2检测到黑线或R2和L2同时检测到黑线停止右转
  * @param  无
  * @retval 无
  */
void Midleft(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

//		TIM_Cmd(TIM3, DISABLE);
//		do
//		{
			TIM_SetCompare2(TIM2,0);
			TIM_SetCompare1(TIM2,80);
			TIM_SetCompare3(TIM2,80);
			TIM_SetCompare4(TIM2,80);
//		}while(((L1!=0)||(L2!=1)||(R2!=0)||(R1!=0))&&((L1!=0)||(L2!=1)||(R2!=1)||(R1!=0)));//左转循环结束条件
//		TIM_Cmd(TIM3, ENABLE);
	
}
/**
  * @brief  中幅度右转，直到R2检测到黑线或R2和L2同时检测到黑线停止右转
  * @param  无
  * @retval 无
  */
void Midright(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);	//右后
	
//		TIM_Cmd(TIM3, DISABLE);
//		do
//		{
			TIM_SetCompare2(TIM2,80);
			TIM_SetCompare1(TIM2,0);
			TIM_SetCompare3(TIM2,80);
			TIM_SetCompare4(TIM2,80);
//		}while(((L1!=0)||(L2!=0)||(R2!=1)||(R1!=0))&&((L1!=0)||(L2!=1)||(R2!=1)||(R1!=0)));
//		TIM_Cmd(TIM3, ENABLE);
}
/**
  * @brief  90°左转，直到L1检测到黑线停止左转
  * @param  无
  * @retval 无
  */
void bigleft(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后
		
//		TIM_Cmd(TIM3, DISABLE);
		do
		{
			TIM_SetCompare2(TIM2,100);
			TIM_SetCompare1(TIM2,100);
			TIM_SetCompare3(TIM2,0);
			TIM_SetCompare4(TIM2,100);
		}while((L1!=1)||(L2!=0)||(R2!=0)||(R1!=0));//
//		TIM_Cmd(TIM3, ENABLE);

}
/**
  * @brief  90°右转，直到R1检测到黑线停止右转
  * @param  无
  * @retval 无
  */
void bigright(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);	//右后
		
//		TIM_Cmd(TIM3, DISABLE);
		do
		{
			TIM_SetCompare2(TIM2,100);
			TIM_SetCompare1(TIM2,100);
			TIM_SetCompare3(TIM2,100);
			TIM_SetCompare4(TIM2,0);
		}while((L1!=0)||(L2!=0)||(R2!=0)||(R1!=1));
//		TIM_Cmd(TIM3, ENABLE);

}
/**
  * @brief  停止
  * @param  无
  * @retval 无
  */
void stop(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//左前
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左后
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare1(TIM2,0);
		TIM_SetCompare2(TIM2,0);
		TIM_SetCompare3(TIM2,0);
		TIM_SetCompare4(TIM2,0);
}
void methon1(void)
{
		//左转
	
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后
		

		TIM_SetCompare2(TIM2,100);
		TIM_SetCompare1(TIM2,100);
		TIM_SetCompare3(TIM2,100);
		TIM_SetCompare4(TIM2,100);
		
	
		stop();
	
		//直线
		

}
void BZ_left(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare2(TIM2,0);
		TIM_SetCompare1(TIM2,100);
		TIM_SetCompare3(TIM2,0);
		TIM_SetCompare4(TIM2,100);
	
}
void BZ_right(void)
{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后

		TIM_SetCompare2(TIM2,100);
		TIM_SetCompare1(TIM2,0);
		TIM_SetCompare3(TIM2,100);
		TIM_SetCompare4(TIM2,0);
	
}
