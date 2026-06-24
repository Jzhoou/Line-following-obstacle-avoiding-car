#ifndef __HCSR04_H
#define __HCSR04_H

#define Trig_Port 		GPIOB
#define Trig_Pin 		GPIO_Pin_9
#define Trig_RCC		RCC_APB2Periph_GPIOB

#define Echo_Port 		GPIOB
#define Echo_Pin 		GPIO_Pin_8
#define Echo_RCC		RCC_APB2Periph_GPIOB

void HCSR04_Init(void);
void HCSR04_Start(void);

#endif 
