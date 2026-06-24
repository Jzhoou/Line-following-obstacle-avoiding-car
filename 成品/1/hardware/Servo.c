#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * berif：		舵机初始化
  * param：		无
  * reteval：	无
  */
void Servo_Init(void)
{
	void PWM_Servo_Init();								//初始化舵机的底层PWM
}

/**
  * berif：		舵机设置角度
  * param：		Angle 要设置的舵机角度，范围：0~180
  * reteval：	无
  */
void Servo_SetAngle(float Angle)
{
	PWM_SetCompare1(Angle / 180 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}
