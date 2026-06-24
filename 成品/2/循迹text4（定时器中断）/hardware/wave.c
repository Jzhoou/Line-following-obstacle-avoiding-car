//超声波例程
//舵机pwm通过 timer1 产生，timer2用来给超声波回波计算时间。wave用来测量超声波回波
//PA12----Echo  接收回波
//PA11----Trig  超声波发射

//#include "bsp_sys.h"
#include "wave.h"
#include "delay.h"

#define Wave_ON()  GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define Wave_OFF() GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define Wave_State()  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)

u16 Distance=0;
u8 OB_L =0,OB_M=0,OB_R=0;//障碍物位置标记
/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无	
 */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=65535;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (360 - 1);				                /* 时钟预分频数 72M/360 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		                /* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;                   /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    //TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
    //TIM_Cmd(TIM7, ENABLE);																		/* 开启时钟 */    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , DISABLE);		/*先关闭等待使用*/    
}





//超声波端口初始化
void Wave_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	       
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					       //PA11接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //初始化外设GPIO 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				         //PA12接ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM2_NVIC_Configuration();//定时器初始化
	TIM2_Configuration();			
}



//测距
//开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
u16 Wave_Start(void)
{
	u16  Distance;
  Wave_ON();  		  //送>10US的高电平
  Delay_us(20);		                      //延时20US
  Wave_OFF();
  
  while(!Wave_State());	             //等待高电平
  TIM_Cmd(TIM2, ENABLE);             //开启时钟
  while(Wave_State());	             //等待低电平
  TIM_Cmd(TIM2, DISABLE);			                             //定时器2失能
  Distance=TIM_GetCounter(TIM2)*5*34/2000;								 //计算距离	厘米cm
  TIM_SetCounter(TIM2,0);	
  return Distance;
}

//设定避障距离25cm
void Wave_BZ(void)
{
		 Distance = Wave_Start(); 
	   if((Distance < 15) && (Distance != 0))//设定避障距离25cm
	   {
			stop();//停车
			OB_M=1;//标记中间有障碍物
			TIM_SetCompare1(TIM1,45);//舵机转到右端
			Duoji_Control(ON);
			Delay_ms(500);//
			Delay_ms(500);//
			Duoji_Control(OFF);

			Distance = Wave_Start();//测量右端是否有障碍物
			 
			if((Distance < 15) && (Distance != 0))//如果有
			{
				stop();//停车
				OB_R=1;//标记右端有障碍物
			}
			else
			{
				OB_R=0;//标记右端没有障碍物
			}

			TIM_SetCompare1(TIM1,115);//舵机转到左端
			Duoji_Control(ON);
			Delay_ms(500);//
			Delay_ms(500);//
			Duoji_Control(OFF);

			Distance = Wave_Start();//测量左端是否有障碍物


			if((Distance < 15) && (Distance != 0))//如果有
			{
				stop();//停车
				OB_L=1;    //标记左端有障碍物
			}
			else
			{
				OB_L=0;
			}


			TIM_SetCompare1(TIM1,81);//占空比5%，90度;//舵机转到中间
			Duoji_Control(ON);
			Delay_ms(500);//
			Delay_ms(500);//
			Duoji_Control  (OFF);



	   }
	   else 
	   {
	   	go();
	   }
	   
	   if((OB_L==0)&&(OB_M==0)&&(OB_R==0))//没有障碍物
	   {
	   		go();OB_L=0; OB_M=0; OB_R=0;

	   }
	   if((OB_L==0)&&(OB_M==1)&&(OB_R==0))//中间障碍物
	   {	
	   		OB_L=0; OB_M=0; OB_R=0;
				
	   		Car_Turn_Right();//向右横着走
		    Delay_ms(500);
			  Delay_ms(200);//行走1s，改为前进
				go();
	   }
	   if((OB_L==0)&&(OB_M==1)&&(OB_R==1))//中间和右边障碍物
	   {
				OB_L=0; OB_M=0; OB_R=0;
				Car_Turn_Left();
				Delay_ms(500);Delay_ms(500);
				go();
	   }
	   if((OB_L==1)&&(OB_M==1)&&(OB_R==0))//中间和左边障碍物
	   {
				OB_L=0; OB_M=0; OB_R=0;
				Car_Turn_Right();
				Delay_ms(500);Delay_ms(500);
				go();
	   }
	   if((OB_L==1)&&(OB_M==1)&&(OB_R==1))//中间和左边右边都有障碍物
	   {
	   		OB_L=0; OB_M=0; OB_R=0;
				Car_Back();
				Delay_ms(500);
	   		Car_Turn_Right();
			  Delay_ms(500);
			  Delay_ms(500);
				Delay_ms(500);
				Delay_ms(500);//转180度，粗略,精确请调节延时
			  go();
	   }

	 }
