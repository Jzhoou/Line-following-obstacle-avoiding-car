#include "stm32f10x.h"
#include "Delay.h"
#include "Motor.h"
#include "Infrared_tracking.h"
#include "Timer4.h"
#include "serial.h"
#include "buzzer.h"

uint16_t L1,L2,R1,R2;
extern uint16_t Distance;	//全局变量距离
extern uint16_t StartTime;	//上电需要等一段时间测量距离，中断存在导致直接主函数Delay有bug，
					//定时器计时给主函数，时间到了再执行对应操作
uint16_t i,j;

int main()
{
	Buzzer_Init();
	Timer4_Init();
	Serial_Init();
	InfraredTracking_Init();		
	Motor_Init();
	Buzzer_state(DISABLE);
	while(1)
	{
		if(L1==1&&L2==1&&R2==1&&R1==1)
		{
			while(1)
			{
				stop();
			}
		}
		if(L1==0&&L2==1&&R2==1&&R1==0)//直行      0110
		{
			go();
		}
		if(L1==0&&L2==0&&R2==1&&R1==0)//右微调    0110
		{
			right();
		}
		if(L1==0&&L2==1&&R2==0&&R1==0)//左微调    0110
		{
			left();
		}
		if(L1==1&&L2==0&&R2==0&&R1==0)//（中）左转 1000
		{
			Midleft();
		}		
		if(L1==0&&L2==0&&R2==0&&R1==1)//（中）右转 0001
		{
			Midright();
		}
		if(Object_Distance<=20&&Object_Distance!=0)	//检测到目标物警报+掉头
		{
			stop();
			for(i=0;i<=50000;i++)
			{
				for(j=0;j<=5;j++)
				Buzzer_state(ENABLE);
				
			}
			Buzzer_state(DISABLE);
			turnaround();
		}
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

