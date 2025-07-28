

#include "includes.h"
 


int main(void)
{
   HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
	
    usart_init(460800);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    key_init();                             /* 初始化按键 */
	
//		MX_SPIFlash_Init();//W25Q128
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */

			/* 初始化定时器配置 */
	
		ADVANCED_TIMy_Init();//TIM8 高级定时器

	

		gtim_timx_encoder_chy_init(0XFFFF, 0);  /* 不分频直接84M的计数频率 */


//writingflash();
	HAL_Delay(10);
	readingflash();

	HAL_Delay(10);				
				
				
	if((Rx_Buffer[0] != 0))
	{
		Weight_calibration[0] = Rx_Buffer[0];
		Weight_calibration[1] = Rx_Buffer[1];
		Weight_calibration[2] = Rx_Buffer[2];
		Weight_calibration[3] = Rx_Buffer[3];


	}

//100ms现在	
		gtim_timx_int_init(1000-1, 8400-1);//定时器7的，也就是保持力的,1S   10000-1
		HAL_NVIC_DisableIRQ(TIM7_IRQn);	// HAL_NVIC_EnableIRQ   HAL_NVIC_DisableIRQ

	
  freertos_demo();                    /* 运行FreeRTOS例程 */
}




















































































//void godBless(void)
//{
////                              _ooOoo_
////                             o8888888o
////                             88" . "88
////                             (| -_- |)
////                              O\ = /O
////                           ____/`---'\____
////                        .   ' \\| |// `.
////                         / \\||| : |||// \
////                        / _||||| -:- |||||- \
////                         | | \\\ - /// | |
////                       | \_| ''\---/'' | |
////                        \ .-\__ `-` ___/-. /
////                    ___`. .' /--.--\ `. . __
////                  ."" '< `.___\_<|>_/___.' >'"".
////                 | | : `- \`.;`\ _ /`;.`/ - ` : | |
////                    \ \ `-. \_ __\ /__ _/ .-` / /
////           ======`-.____`-.___\_____/___.-`____.-'======
////                              `=---='
////
////           .............................................
////                     佛祖保佑             永无BUG
////            佛曰:
////                     写字楼里写字间，写字间里程序员；
////                     程序人员写程序，又拿程序换酒钱。
////                     酒醒只在网上坐，酒醉还来网下眠；
////                     酒醉酒醒日复日，网上网下年复年。
////                     但愿老死电脑间，不愿鞠躬老板前；
////                     奔驰宝马贵者趣，公交自行程序员。
////                     别人笑我忒疯癫，我笑自己命太贱；
////                     不见满街漂亮妹，哪个归得程序员？
////                     这家公司垃圾，老板不行，趁早跑路！！！
//}

