

#ifndef __BTIM_H
#define __BTIM_H

#include "./SYSTEM/sys/sys.h"



extern TIM_HandleTypeDef TIM7_Handler;  


#define GTIM_TIMX_INT                       TIM7
#define GTIM_TIMX_INT_IRQn                  TIM7_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM7_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM7_CLK_ENABLE(); }while(0)      /* TIM3 时钟使能 */



void gtim_timx_int_init(uint16_t arr, uint16_t psc);/* 通用定时器 定时中断初始化函数 */





#endif

















