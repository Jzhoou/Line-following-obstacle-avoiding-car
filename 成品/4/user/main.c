#include "stm32f10x.h"
#include "Delay.h"
#include "Motor.h"
#include "Infrared_tracking.h"
#include "Timer4.h"
#include "serial.h"
#include "buzzer.h"

uint16_t L1,L2,R1,R2;
uint16_t i,j,Object_flag,find_Object;

int main()
{
	Buzzer_Init();
	Timer4_Init();
	Serial_Init();
	InfraredTracking_Init();		
	Motor_Init();
	Buzzer_state(DISABLE);
	uint8_t Object_Counter=0;
	while(1)
	{
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
		if(((L1==1&&L2==1&&R2==1&&R1==0)||(L1==1&&L2==1&&R2==0&&R1==0))&&find_Object==0)//第一个左转 
		{
			bigleft();
		}
		
		if(Barrier_Distance<=30&&Barrier_Distance!=0)	//避障
		{
			find_Object = 2;
			for(i=0;i<=17;i++)//3*50000=150000
			{
				if(Object_Distance<=170&&Object_Distance!=0)
				{
					Object_Counter++;
					if(Object_Counter>=7)
					{
						find_Object=1;
					}
				}
				for(j=0;j<=10000;j++)
				BZ_left();
			}
			for(i=0;i<=25000;i++)
			{
				go();
			}
			if(find_Object!=1)
			{
				if(Object_Counter!=0)
				{
					Object_Counter = 0;
				}
			}
			for(i=0;i<14;i++)//3*54000=162000
			{
				if(Object_Distance<=170&&Object_Distance!=0)
				{
					Object_Counter++;
					if(Object_Counter>=5)
					{
						find_Object=1;
					}
				}
				for(j=0;j<=13500;j++)
				BZ_right();
			}
			for(i=0;i<=30000;i++)
			{
				for(j=0;j<=2;j++)
				go();
			}
			for(i=0;i<=42000;i++)
			{
				for(j=0;j<=2;j++)
				BZ_right();
			}
			do
			{
				go();
			}while(L1==1||L2==1||R2==1||R1==1);
		}
		if(((L1==1&&L2==1&&R2==1&&R1==0)||(L1==1&&L2==1&&R2==1&&R1==1))&&find_Object==1)//目标物在左
		{
			bigleft();
		}
		if(((L1==1&&L2==1&&R2==1&&R1==0)||(L1==1&&L2==1&&R2==1&&R1==1))&&find_Object==2)//目标物在右
		{
			bigright();
		}
		if(Object_Distance<=25&&Object_Distance!=0)	//检测到目标物警报+掉头
		{
			stop();
			Object_flag++;
			for(i=0;i<=50000;i++)
			{
				for(j=0;j<=5;j++)
				Buzzer_state(ENABLE);
			}
			Buzzer_state(DISABLE);
			if(Object_flag==1)
			{
				while(1)
				{
					stop();
				}
			}
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

