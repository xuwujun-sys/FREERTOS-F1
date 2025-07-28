

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"
#include "includes.h"



 float Constant_weight;//定力重量




	
TIM_HandleTypeDef TIM7_Handler; /* 定时器x句柄 */



void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    TIM7_Handler.Instance = GTIM_TIMX_INT;                     /* 通用定时器x */
    TIM7_Handler.Init.Prescaler = psc;                         /* 预分频系数 */
    TIM7_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    TIM7_Handler.Init.Period = arr;                            /* 自动装载值 */
	  TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM7_Handler);

//    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);             /* 设置中断优先级，抢占优先级1，子优先级3 */
//    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                     /* 开启ITMx中断 */
 
    HAL_TIM_Base_Start_IT(&TIM7_Handler);                      /* 使能定时器x和定时器x更新中断 */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	
  if(htim_base->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();           
		HAL_NVIC_SetPriority(TIM7_IRQn,4,3);    
//		HAL_NVIC_EnableIRQ(TIM7_IRQn);          
		HAL_NVIC_DisableIRQ(TIM7_IRQn);			
	}	
		
}

void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
}





