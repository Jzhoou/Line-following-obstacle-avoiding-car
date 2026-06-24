#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "OLED.h"
#include "HCSR04.h"
#include "Delay.h"

uint16_t Distance;	//全局变量距离
uint16_t StartTime;	//上电需要等一段时间测量距离，中断存在导致直接主函数Delay有bug，
					//定时器计时给主函数，时间到了再执行对应操作
					
/**
  * @brief  	TIM3初始化：将TIM3用于定时中断，在中断中测距，不影响主函数
  *				每0.0001s=10^(-4)s进一次中断
  * @param  	无
  * @retval 	无
  */

void Timer3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//选择APB1总线下的定时器Timer2
	
	TIM_InternalClockConfig(TIM3);		//TIM2使用内部时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数模式，此处为向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 7199;		//ARR 1 = 0.0001S=10^(-4) 0
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//高级计时器特有，重复计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		//使能中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		//中断通道选择
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//优先级，同上
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);		//打开定时器
}

/**
  * @brief  	TIM3中断，用来测距
  *				每0.0001s=10^(-4)s进一次中断
  * @param  	无
  * @retval 	无
  */

void TIM3_IRQHandler(void)		//定时器3的中断函数0.0001s触发一次10000=1s
{
	static uint16_t HCSR04_Time = 0;//给超声波测距计时，每0.1s交换进行发送开始测量信号/获取数据信号，
									//超声波模块要求间隔0.2s以上测距一次
	static int8_t exchange = 0;		//根据exchange为0或1决定是测距还是读取
	static uint16_t Time;			//计时测距过程echo为高电平的时间，每一个Time为0.0001s，计算出
									//测量距离
	if(StartTime<=30000)			//0.2s完成一次测量，在上电后0.3s再开始判断是否转动舵机
	{
		StartTime++;
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		HCSR04_Time++;
		if(HCSR04_Time >= 200)		//0.0001s*1000=0.1s
		{
			if(exchange == 0)
			{
				HCSR04_Start();		//发出测距信号
				Time = 0;			//设定时间为0，测量echo口每次高电平Time+1，开始计时
				exchange = 1;
			}
			else if(exchange == 1)
			{
				Distance = ((Time * 0.0001) * 34000) / 2;	//Time中断数据
				exchange = 0;
			}
			HCSR04_Time = 0;
		}			
		if (GPIO_ReadInputDataBit(Echo_Port, Echo_Pin) == 1)
		{
			Time++;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//清空标志位
	}
}
