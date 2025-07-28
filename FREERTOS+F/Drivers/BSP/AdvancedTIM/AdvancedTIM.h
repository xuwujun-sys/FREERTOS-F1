#ifndef __ADVANCED_TIM_H__
#define __ADVANCED_TIM_H__

#include "stm32f4xx_hal.h"


#define FALSE                                 0  //极限标志位  True:到达极限位  False:未到达极限位
#define TRUE                                  1  //极限标志位  True:到达极限位  False:未到达极限位
#define LIM_POS_LEVEL						              1	 // 上极限引脚有效电平
#define LIM_NEG_LEVEL						              1	 // 下极限引脚有效电平，上升沿有效就位高电平，下降沿有效就是低电平




#define ADVANCED_TIMy                       TIM8
#define ADVANCED_TIMy_GPIO_AF               GPIO_AF3_TIM8
#define ADVANCED_TIMy_RCC_CLK_ENABLE()      __HAL_RCC_TIM8_CLK_ENABLE()
#define ADVANCED_TIMy_RCC_CLK_DISABLE()     __HAL_RCC_TIM8_CLK_DISABLE()
#define ADVANCED_TIMy_GPIO_RCC_CLK_ENABLE() __HAL_RCC_GPIOI_CLK_ENABLE()
#define ADVANCED_TIMy_GPIO1_RCC_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

#define ADVANCED_TIMy_OC_IRQn               TIM8_CC_IRQn
#define ADVANCED_TIMy_OC_IRQHANDLER         TIM8_CC_IRQHandler

#define ADVANCED_TIMy_CH1_PORT              GPIOI
#define ADVANCED_TIMy_CH1_PIN               GPIO_PIN_5

/*#define ADVANCED_TIMy_DIR1_PORT             GPIOH
#define ADVANCED_TIMy_DIR1_PIN              GPIO_PIN_2*/
#define ADVANCED_TIMy_DIR1_PORT             GPIOD
#define ADVANCED_TIMy_DIR1_PIN              GPIO_PIN_3

// 定义定时器预分频，定时器实际时钟频率为：168MHz/（ADVANCED_TIMx_PRESCALER+1）
#define ADVANCED_TIMy_PRESCALER             83  // 实际时钟频率为：2MHz   83

// 定义定时器周期，当定时器开始计数到ADVANCED_TIMx_PERIOD值并且重复计数寄存器为0时更新定时器并生成对应事件和中断
#define ADVANCED_TIMy_PERIOD                0xFFFF  // 
// 定义高级定时器重复计数寄存器值，
#define ADVANCED_TIMy_REPETITIONCOUNTER     0

// 最终定时器频率计算为： 168MHz/(ADVANCED_TIMx_PRESCALER+1)/(ADVANCED_TIM_REPETITIONCOUNTER+1)/(ADVANCED_TIMx_PERIOD+1)
// 比如需要产生1ms周期定时，可以设置为： 168MHz/（167+1）/（0+1）/(999+1)=1KHz，即1ms周期



#define LIMPOS_GPIO_CLK_ENABLE()      				__HAL_RCC_GPIOE_CLK_ENABLE()     // 上限位输入引脚
#define LIMPOS_PORT                   				GPIOE                            // 有效电平是低电平
#define LIMPOS_PIN                    				GPIO_PIN_2                       // 
#define LIMPOS_EXTI_IRQn											EXTI2_IRQn
#define LIMPOS_EXTI_IRQHandler                EXTI2_IRQHandler                 // 中断服务函数入口

#define LIMNEG_GPIO_CLK_ENABLE()      				__HAL_RCC_GPIOE_CLK_ENABLE()     // 下限位输入引脚
#define LIMNEG_PORT                   				GPIOE                            // 有效电平是低电平
#define LIMNEG_PIN                    				GPIO_PIN_3                       // 
#define LIMNEG_EXTI_IRQn											EXTI3_IRQn
#define LIMNEG_EXTI_IRQHandler                EXTI3_IRQHandler                 // 中断服务函数入口








extern __IO uint8_t  LimPosi; //上极限标志位  True:到达极限位  False:未到达极限位
extern __IO uint8_t  LimNega; //下极限标志位


extern TIM_HandleTypeDef hAdvtimy;

/* 函数声明 ------------------------------------------------------------------*/

void ADVANCED_TIMy_Init(void);


void Msp_TIM_OC_PostInit(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



#endif	/* __ADVANCED_TIM_H__ */





