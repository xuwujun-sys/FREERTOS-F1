
#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 引脚 定义 */

#define LED0_GPIO_PORT                  GPIOE
#define LED0_GPIO_PIN                   GPIO_PIN_0
#define LED0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */

#define LED1_GPIO_PORT                  GPIOE
#define LED1_GPIO_PIN                   GPIO_PIN_1
#define LED1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */

#define LED2_GPIO_PORT                  GPIOI
#define LED2_GPIO_PIN                   GPIO_PIN_4
#define LED2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */

#define LED3_GPIO_PORT                  GPIOI
#define LED3_GPIO_PIN                   GPIO_PIN_6
#define LED3_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */

#define LED4_GPIO_PORT                  GPIOI
#define LED4_GPIO_PIN                   GPIO_PIN_7
#define LED4_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */


//#define REST_GPIO_PORT                  GPIOE
//#define REST_GPIO_PIN                   GPIO_PIN_1
//#define REST_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PF口时钟使能 */
/******************************************************************************************/

/* LED端口定义 */
#define LED0(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */

#define LED1(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

#define LED2(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */
	
#define LED3(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

#define LED4(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

//#define REST(x)   do{ x ? \
//                      HAL_GPIO_WritePin(REST_GPIO_PORT, REST_GPIO_PIN, GPIO_PIN_SET) : \
//                      HAL_GPIO_WritePin(REST_GPIO_PORT, REST_GPIO_PIN, GPIO_PIN_RESET); \
//                  }while(0)       /* LED1 = GREEN */									
									
/* LED取反定义 */
#define LED0_TOGGLE()    do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)       /* LED0 = !LED0 */
#define LED1_TOGGLE()    do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)       /* LED1 = !LED1 */
#define LED2_TOGGLE()    do{ HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_GPIO_PIN); }while(0)       /* LED1 = !LED1 */
#define LED3_TOGGLE()    do{ HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_GPIO_PIN); }while(0)       /* LED1 = !LED1 */
#define LED4_TOGGLE()    do{ HAL_GPIO_TogglePin(LED4_GPIO_PORT, LED4_GPIO_PIN); }while(0)       /* LED1 = !LED1 */
//#define REST_TOGGLE()    do{ HAL_GPIO_TogglePin(REST_GPIO_PORT, REST_GPIO_PIN); }while(0)       /* LED1 = !LED1 */
/******************************************************************************************/
/* 外部接口函数*/
void led_init(void);                                                                            /* 初始化 */

#endif
