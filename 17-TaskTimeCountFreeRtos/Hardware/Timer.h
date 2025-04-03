#ifndef _TIMER_H
#define _TIMER_H

void Timer_Init(uint16_t Period, uint16_t Perscaler);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
#endif
