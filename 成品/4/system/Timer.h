#ifndef __TIMER_H
#define __TIMER_H

extern uint16_t Distance;
extern uint16_t Time;
void Timer3_Init(void);
void Timer1_Init(void);
void TIM3_IRQHandler(void);


#endif
