
#include "./BSP/AdvancedTIM/AdvancedTIM.h" 
#include "includes.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

TIM_HandleTypeDef hAdvtimy;




__IO uint8_t  LimPosi = FALSE ; //上极限标志位  True:到达极限位  False:未到达极限位
__IO uint8_t  LimNega = FALSE ; //下极限标志位

extern int BBB;

extern int AAA;
extern int CCC;
extern int status_flag;
extern int BRB_A;



void HAL_TIM_OC_PostMspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
  LIMPOS_GPIO_CLK_ENABLE();     //外部中断
  LIMNEG_GPIO_CLK_ENABLE();     //外部中断
	

    /* 定时器通道功能引脚端口时钟使能 */
    ADVANCED_TIMy_GPIO_RCC_CLK_ENABLE();
    ADVANCED_TIMy_GPIO1_RCC_CLK_ENABLE();

    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ADVANCED_TIMy_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = ADVANCED_TIMy_GPIO_AF;
    HAL_GPIO_Init(ADVANCED_TIMy_CH1_PORT, &GPIO_InitStruct);
		
    GPIO_InitStruct.Pin = ADVANCED_TIMy_DIR1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ADVANCED_TIMy_DIR1_PORT, &GPIO_InitStruct);
       
    HAL_NVIC_SetPriority(ADVANCED_TIMy_OC_IRQn,1,0);
    HAL_NVIC_EnableIRQ(ADVANCED_TIMy_OC_IRQn);
	
	
  /*上限输入*/
  GPIO_InitStruct.Pin = LIMPOS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;      // GPIO_MODE_IT_FALLING 下降沿触发  GPIO_MODE_IT_RISING 上升沿
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(LIMPOS_PORT, &GPIO_InitStruct);

	/*下限输入*/
  GPIO_InitStruct.Pin = LIMNEG_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;      //上升沿触发
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(LIMNEG_PORT, &GPIO_InitStruct);

  
	HAL_NVIC_SetPriority(LIMPOS_EXTI_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LIMPOS_EXTI_IRQn);
  
  HAL_NVIC_SetPriority(LIMNEG_EXTI_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(LIMNEG_EXTI_IRQn);
	
}
/**
  * 函数功能: 高级定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ADVANCED_TIMy_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
//  TIM_MasterConfigTypeDef sMasterConfig;
//  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;
  
  /* 定时器外设时钟使能 */
  ADVANCED_TIMy_RCC_CLK_ENABLE();
  
  /* 定时器基本环境配置 */
  hAdvtimy.Instance = ADVANCED_TIMy;                          // 定时器编号
  hAdvtimy.Init.Prescaler = ADVANCED_TIMy_PRESCALER;           // 定时器预分频器   PSC
  hAdvtimy.Init.CounterMode = TIM_COUNTERMODE_UP;              // 计数方向：向上计数
  hAdvtimy.Init.Period = ADVANCED_TIMy_PERIOD;                 // 定时器周期
  hAdvtimy.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;          // 时钟分频
  HAL_TIM_Base_Init(&hAdvtimy);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       		// 使用内部时钟源
  HAL_TIM_ConfigClockSource(&hAdvtimy, &sClockSourceConfig);

  /* 配置CH1为比较输出模式 */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // 比较输出模式：反转输出
  sConfigOC.Pulse = 0xFFFF;                            // 脉冲数
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           // 输出极性
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // 互补通道输出极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  
  /* 配置CH1为比较输出模式 */
  HAL_TIM_OC_ConfigChannel(&hAdvtimy, &sConfigOC, TIM_CHANNEL_1);
  /* 初始化定时器相关IO */
  HAL_TIM_OC_PostMspInit();
}




/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef* htim)
{
  if(htim == &hAdvtimy)
  {
    /* 定时器外设时钟禁用 */
    ADVANCED_TIMy_RCC_CLK_DISABLE(); 
    HAL_NVIC_DisableIRQ(ADVANCED_TIMy_OC_IRQn);
  }

} 


/**
  * 外部中断服务函数
  * 实现判断是否到达极限
  */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

		if(GPIO_Pin == LIMPOS_PIN)	//上极限引脚  E2
		{
			__HAL_GPIO_EXTI_CLEAR_IT(LIMPOS_PIN);
			if(HAL_GPIO_ReadPin(LIMPOS_PORT,LIMPOS_PIN) == LIM_POS_LEVEL)
			{	
				LimPosi	= TRUE;  
//				LED0_TOGGLE();
				MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
				move_flag = 0;
				BBB=3;BRB_A=999;
				CCC=999;AAA=999;
			
			}
		}
		if(GPIO_Pin == LIMNEG_PIN)	//下极限引脚   E3
		{
			__HAL_GPIO_EXTI_CLEAR_IT(LIMNEG_PIN);
			if(HAL_GPIO_ReadPin(LIMNEG_PORT,LIMNEG_PIN) == LIM_NEG_LEVEL)
			{
				LimNega = TRUE;   
//				LED1_TOGGLE();
				MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
				move_flag = 0;
				BBB=3;BRB_A=999;
				CCC=999;AAA=999;		
			}
		}

		
//		srd.run_state = STOP;		//碰到上下极限都要停下来
//		srd.Step_count=srd.Step_num;	
	
}
