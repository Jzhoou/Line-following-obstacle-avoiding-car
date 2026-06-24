#include "stm32f10x.h"
#include "Delay.h"
#include "Motor.h"
#include "Infrared_tracking.h"
#include "PWM.h"
#include "Timer4.h"
//#include "Timer3.h"

/**
  *在Motor.c修改状态的参数(CCR)
  *在PWM.c修改ARR和PSC
  *在main.c修改循迹逻辑
  *红外循迹		黑1白0
  *main.c里注释掉的Time3中断是用于在循迹途中（还在黑线上，应该要循迹直行）但四个红外循迹为0000（全白，见缝插针地错开了黑线）时，
  *让小车能直行（左右微调）”找回“黑线，继续循迹。但经实测，有一点效果但不多，淦！
  *主要问题：1.循迹途中突然原地转弯（在Motor.c的do while循环里），也可能是场地原因
  *			 2.红外循迹的0000（在main.c的0000里）
  *			 3.“T”形路口转向判别（1111），由于小车在行驶时车头的四路红外循迹无法保证完全与黑线平行，因此可能“随机！”引起下列情况：
  *			 （1）车头稍微有一点倾斜，导致红外循迹获取的数据：先1000（中左转）再1110（停止中左转，进行90°左转）
  *			 （2）原理同（1），但是右转
  *			 （3）车头刚好平行于黑线，红外循迹获得数据1111（1111的运动逻辑我下面没写）。
  *
  *	idea：直接不写0000，用其他传感器检测掉头、停止需求。比如：用视觉传感器检测前方是否有黑线，再根据当前运动状态结合进行判断掉头或停止。该idea或许可以解决以上所有问题。
  */
  

uint16_t L1,L2,R1,R2;
//uint16_t Flag1=0,Flag2=0,Flag3=0,flag=0;

int main()
{
	Timer4_Init();
//	Timer3_Init();
	InfraredTracking_Init();		
	Motor_Init();
	while(1)
	{
		if(L1==0&&L2==1&&R2==1&&R1==0)//直行      0110
		{
			go();
		}
		if(L1==0&&L2==1&&R2==0&&R1==0)//偏左转    0100
		{
			left();		
		}		
		if(L1==0&&L2==0&&R2==1&&R1==0)//偏右转    0010
		{
			right();
		}
		if(L1==1&&L2==0&&R2==0&&R1==0)//（中）左转 1000
		{
			Midleft();
		}		
				
		if(L1==0&&L2==0&&R2==0&&R1==1)//（中）右转 0001
		{
			Midright();
		}
		if(L1==1&&L2==1&&R2==1&&R1==0)//90°左转   1110
		{
			bigleft();
		}		
		if(L1==0&&L2==1&&R2==1&&R1==1)//90°右转   0111
		{
			bigright();
		}
//		if((L1==0&&L2==0&&R2==0&&R1==0)&&(Flag1==1&&Flag2==1&&Flag3==1))  //0000
//		{
//			stop();
////			flag=1;
//		}
//		if((L1==0&&L2==0&&R2==0&&R1==0)&&(Flag1==0||Flag2==0||Flag3==0))  //0000
//		{
//			go();
//		}
		if(L1==0&&L2==0&&R2==0&&R1==0)  //？？停止/掉头/直行（左右微调，保持原运动状态）    0000
		{								//停止：回到起点
			stop();						//掉头：检测到尽头的目标物后需要掉头
		}								//直行（左右微调，保持原运动状态）：循迹时稍微偏离黑线（四个红外循迹都检测到白色区域，ps：可能没有偏离很多，就是四个红外循迹正好见缝插针地躲开了黑线md...）需要直行（左右微调）
	}
}
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		L1=Get_L1Data();
		L2=Get_L2Data();
		R2=Get_R2Data();
		R1=Get_R1Data();
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
//void TIM3_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
//	{
//		if(flag==0)
//		{
//			if(L1==0&&L2==0&&R2==0&&R1==0)
//			{
//				Flag1=1;
//				go();
//				Delay_ms(60);
//				if(L1==0&&L2==0&&R2==0&&R1==0)
//				{
//					Flag2=1;
//					GPIO_SetBits(GPIOA,GPIO_Pin_7);
//					GPIO_ResetBits(GPIOA,GPIO_Pin_6);   //左前
//					GPIO_SetBits(GPIOA,GPIO_Pin_5);
//					GPIO_ResetBits(GPIOA,GPIO_Pin_4);	//右前
//					GPIO_SetBits(GPIOB,GPIO_Pin_1);
//					GPIO_ResetBits(GPIOB,GPIO_Pin_0);	//左后
//					GPIO_SetBits(GPIOB,GPIO_Pin_10);
//					GPIO_ResetBits(GPIOB,GPIO_Pin_11);	//右后
//		
//					TIM_SetCompare2(TIM2,100);
//					TIM_SetCompare1(TIM2,100);
//					TIM_SetCompare3(TIM2,100);
//					TIM_SetCompare4(TIM2,100);
//					Delay_ms(100);
//				if(L1==0&&L2==0&&R2==0&&R1==0)
//				{
//					Flag3=1;
//					GPIO_SetBits(GPIOA,GPIO_Pin_6);
//					GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //左前
//					GPIO_SetBits(GPIOA,GPIO_Pin_4);
//					GPIO_ResetBits(GPIOA,GPIO_Pin_5);	//右前
//					GPIO_SetBits(GPIOB,GPIO_Pin_0);
//					GPIO_ResetBits(GPIOB,GPIO_Pin_1);	//左后
//					GPIO_SetBits(GPIOB,GPIO_Pin_11);
//					GPIO_ResetBits(GPIOB,GPIO_Pin_10);	//右后
//								
//					TIM_SetCompare2(TIM2,100);
//					TIM_SetCompare1(TIM2,50);
//					TIM_SetCompare3(TIM2,100);
//					TIM_SetCompare4(TIM2,100);
//					Delay_ms(140);
//					}
//				}
//			}
//		}
//		if(flag==1)
//		{
//			TIM_Cmd(TIM3, DISABLE);
//		}
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//	}
//}

