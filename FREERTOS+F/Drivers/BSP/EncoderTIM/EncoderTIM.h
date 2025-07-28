#ifndef __ENCODER_TIM_H__
#define __ENCODER_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "./SYSTEM/sys/sys.h"

#include "./BSP/TTIM/TTIM.h"




#define GTIM_TIMX_ENCODER_CH1_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH1_GPIO_PIN          GPIO_PIN_6
#define GTIM_TIMX_ENCODER_CH1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0) /* PC口时钟使能 */

#define GTIM_TIMX_ENCODER_CH2_GPIO_PORT         GPIOC
#define GTIM_TIMX_ENCODER_CH2_GPIO_PIN          GPIO_PIN_7
#define GTIM_TIMX_ENCODER_CH2_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0) /* PC口时钟使能 */

/* TIMX 引脚复用设置
 * 因为PC6、PC7默认并不是TIM3的功能脚, 必须开启复用, 才能用作TIM3的编码器功能引脚
 */
#define GTIM_TIMX_ENCODERCH1_GPIO_AF            GPIO_AF2_TIM3                               /* 端口复用到TIM3 */
#define GTIM_TIMX_ENCODERCH2_GPIO_AF            GPIO_AF2_TIM3

#define GTIM_TIMX_ENCODER                       TIM3                                        /* TIM3 */
#define GTIM_TIMX_ENCODER_INT_IRQn              TIM3_IRQn
#define GTIM_TIMX_ENCODER_INT_IRQHandler        TIM3_IRQHandler
#define GTIM_TIMX_ENCODER_CH1                   TIM_CHANNEL_1                               /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH1_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

#define GTIM_TIMX_ENCODER_CH2                   TIM_CHANNEL_2                               /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_ENCODER_CH2_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

/*************************************************************************************/

void gtim_timx_encoder_chy_init(uint16_t arr, uint16_t psc);                                /* 通用定时器 编码器接口初始化 */
int gtim_get_encode(void);                                                                  /* 获取编码器总计数值 */
void ClearTimer(void); 


extern uint32_t time_count_1s;//定时器7定时标志
extern uint32_t time_count_100ms;//定时器7定时标志
extern int Test_mark,Test_mark_1;//Test_mark_1用于串口发送标志






#endif	/* __ENCODER_TIM_H__ */



