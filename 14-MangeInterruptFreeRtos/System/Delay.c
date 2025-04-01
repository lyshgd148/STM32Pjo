#include "stm32f10x.h"

///**
//  * @brief  微秒级延时
//  * @param  xus 延时时长，范围：0~233015
//  * @retval 无
//  */
//void Delay_us(uint32_t xus)
//{
//	SysTick->LOAD = 72 * xus;				//设置定时器重装值
//	SysTick->VAL = 0x00;					//清空当前计数值
//	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
//	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
//	SysTick->CTRL = 0x00000004;				//关闭定时器
//}

///**
//  * @brief  毫秒级延时
//  * @param  xms 延时时长，范围：0~4294967295
//  * @retval 无
//  */
//void Delay_ms(uint32_t xms)
//{
//	while(xms--)
//	{
//		Delay_us(1000);
//	}
//}
// 
///**
//  * @brief  秒级延时
//  * @param  xs 延时时长，范围：0~4294967295
//  * @retval 无
//  */
//void Delay_s(uint32_t xs)
//{
//	while(xs--)
//	{
//		Delay_ms(1000);
//	}
//} 


void TIM4_Config(void)
{
	RCC->APB1ENR |= (1 << 2);
    TIM4->PSC = 72 - 1;    // 预分频器：72MHz / 72 = 1MHz (1us 计数一次)
    TIM4->ARR = 0xFFFF;    // 设定最大计数值（65.535ms）
    TIM4->CR1 |= TIM_CR1_OPM; // 一次性模式，计数到 `ARR` 后停止
}

void Delay_us(uint16_t us)
{
    TIM4->ARR = us;              // 设定延时时间
    TIM4->CNT = 0;               // 清零计数器
    TIM4->SR &= ~TIM_SR_UIF;     // 清除更新标志
    TIM4->CR1 |= TIM_CR1_CEN;    // 启动 TIM4

    while (!(TIM4->SR & TIM_SR_UIF)); // 等待计数完成
    TIM4->SR &= ~TIM_SR_UIF;         // 清除更新标志
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void Delay_s(uint32_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}
