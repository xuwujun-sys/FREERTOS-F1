
#include "includes.h"


//可能存在重复定义，需要删除
__IO float weight;
__IO int32_t weight_Zero_Data;        //荷重零点
__IO uint8_t State=0;

long double Tx_Buffer[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//暂定数组最小26葛float型数据
long double Rx_Buffer[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//内部保存31个数据10+6+15个
	
//////////////////////////////W25Q128定义///////////////////////////////////////////////////////////////
//////////////////////////////改为内部存储//////////////////////////////////////////////////////////////////////////
#define TEXT_LENTH sizeof(Tx_Buffer) /* 数组长度 */

/*SIZE表示半字长(4字节), 大小必须是4的整数倍, 如果不是的话, 强制对齐到2的整数倍 */
#define SIZE TEXT_LENTH / 4 + ((TEXT_LENTH % 4) ? 1 : 0)

#define FLASH_SAVE_ADDR 0x080C0000 /* 设置FLASH 保存地址(必须为4的整数倍，且大于本代码所占用FLASH的大小 + 0x08000000) */



/////////////////////////////////////////////////////

////////////////////////////////////////////////////

uint8_t cal_flag = 0;
uint8_t cal_f = 0;




/////////////////////////////////////////////////////////////////////////////////////////////////////////
//之后在加，有些用不到，暂不用
//重量校准
__IO int32_t weight_proportion = 9941;  // 电压值与重量变换比例，这个需要实际测试计算才能得到9941
float Tensile_coefficient=0;//拉断系数
float Fracture_coefficient=0;//压断系数
uint32_t speed=0;//速度
uint32_t Electric_distance=0;//点动距离
uint32_t Delay_constant=0;//延时时间
int frequency_constant=0;//频率常数
uint32_t Force_retention_time=0;//力保持时间
uint32_t Force_retention_value=0;//力保持力值
uint32_t Fixed_time=0;//定力时间
uint32_t Constant_force_value=0;//定力力值

//十二点力值校准，原始AD值和校准值，力保持时间，保持力值，点动距离默认的值
extern uint32_t Calibration_weight[4];              //AD值，计算校准/////做标记，之后删掉
extern int32_t Calibration_load[4];                //校准荷重



///////////////////////////////////////////////////////////////////////////////////////////////////////


void readingflash(void)
{
	
					stmflash_read(FLASH_SAVE_ADDR, (uint32_t *)Rx_Buffer, SIZE);

					Weight_calibration[0] = Rx_Buffer[0];      //计算校准值AD1
					Weight_calibration[1] = Rx_Buffer[1];      //计算校准值AD2
					Weight_calibration[2] = Rx_Buffer[2];      //计算校准值AD3
					Weight_calibration[3] = Rx_Buffer[3];      //计算校准值AD4
					Distance_value = Rx_Buffer[4];             //位移系数
					Zero_weight = Rx_Buffer[5];                //零点重量	
					Decreasing_Retention_coefficient = Rx_Buffer[6];       // BRB减速距离
					Decreasing_Speed = Rx_Buffer[7];                       // BRB减速速度
					Weight_Speed_coefficient = Rx_Buffer[8];               // BRB第二阶段速度调整系数
					upper_limit_force = Rx_Buffer[9];                      // BRB力保持上限
					lower_limit_force = Rx_Buffer[10];                      // BRB力保持下限
					Retention_force = Rx_Buffer[11];                        // BRB力保持力值
					First_pushing_time = Rx_Buffer[12];                     // BUTTON第一次下压时间
					First_uplift_time = Rx_Buffer[13];                      // BUTTON第一次抬起时间
					First_wait_time = Rx_Buffer[14];                        // BUTTON第一次等待时间
					Second_pushing_time = Rx_Buffer[15];                    // BUTTON第二次下压时间
					Second_uplift_time = Rx_Buffer[16];                     // BUTTON第二次抬起时间
					Second_wait_time = Rx_Buffer[17];                       // BUTTON第二次等待时间
					Third_pushing_time = Rx_Buffer[18];                     // BUTTON第三次下压时间
					Third_uplift_time = Rx_Buffer[19];                      // BUTTON第三次抬起时间
					Third_wait_time = Rx_Buffer[20];                        // BUTTON第三次等待时间，与下次992间隔时间
					Displacement_deceleration = Rx_Buffer[21];              // BUTTON位移减速距离
					Press_stop_deceleration = Rx_Buffer[22];                // BUTTON 减速距离BUTTON里面
					Slow_down_deceleration = Rx_Buffer[23];                 // BUTTON压停距离BUTTON里面
					Return_stop_deceleration = Rx_Buffer[24];               // BUTTON回停距离BUTTON里面
					Goto_slow_speed = Rx_Buffer[25];                        // BUTTON去程减速速度BUTTON里面回程减速速度BUTTON里面
					Return_slow_speed = Rx_Buffer[26];                      // BUTTON 回程减速速度BUTTON里面
					Zero_up_deceleration = Rx_Buffer[27];                   //零点上方距离


//				printf("计算校准值1 %0.0LF \n",Rx_Buffer[0]);     //计算校准值1
//				printf("计算校准值2 %0.0LF \n",Rx_Buffer[1]);     //计算校准值2
//				printf("计算校准值3 %0.0LF \n",Rx_Buffer[2]);     //计算校准值3
//				printf("计算校准值4 %0.0LF \n",Rx_Buffer[3]);     //计算校准值4
//				printf("位移系数  %0.0LF \n",Rx_Buffer[4]);     //位移系数
//				printf("零点重量  %0.0LF \n",Rx_Buffer[5]);     //零点重量


	
}





void writingflash(void)
{
	
					Tx_Buffer[0] = Weight_calibration[0];      //计算校准值AD1
					Tx_Buffer[1] = Weight_calibration[1];      //计算校准值AD2
					Tx_Buffer[2] = Weight_calibration[2];      //计算校准值AD3 
					Tx_Buffer[3] = Weight_calibration[3];      //计算校准值AD4
					Tx_Buffer[4] = Distance_value;      //位移系数
					Tx_Buffer[5] = Zero_weight;      //零点重量
					Tx_Buffer[6] = Decreasing_Retention_coefficient;       // BRB减速距离
					Tx_Buffer[7] = Decreasing_Speed;       // BRB减速速度
					Tx_Buffer[8] = Weight_Speed_coefficient;       // BRB第二阶段速度调整系数
					Tx_Buffer[9] = upper_limit_force;       // BRB力保持上限
					Tx_Buffer[10] = lower_limit_force;      // BRB力保持下限
					Tx_Buffer[11] = Retention_force;      // BRB力保持力值
					Tx_Buffer[12] = First_pushing_time;      // BUTTON第一次下压时间
					Tx_Buffer[13] = First_uplift_time;      // BUTTON第一次抬起时间
					Tx_Buffer[14] = First_wait_time;      // BUTTON第一次等待时间
					Tx_Buffer[15] = Second_pushing_time;      // BUTTON第二次下压时间
					Tx_Buffer[16] = Second_uplift_time;      // BUTTON第二次抬起时间
					Tx_Buffer[17] = Second_wait_time;      // BUTTON第二次等待时间
					Tx_Buffer[18] = Third_pushing_time;      // BUTTON第三次下压时间
					Tx_Buffer[19] = Third_uplift_time;      // BUTTON第三次抬起时间
					Tx_Buffer[20] = Third_wait_time;      // BUTTON第三次等待时间，与下次992间隔时间
					Tx_Buffer[21] = Displacement_deceleration;      // BUTTON位移减速距离
					Tx_Buffer[22] = Press_stop_deceleration;      // BUTTON 减速距离BUTTON里面
					Tx_Buffer[23] = Slow_down_deceleration;      // BUTTON压停距离BUTTON里面
					Tx_Buffer[24] = Return_stop_deceleration;      // BUTTON回停距离BUTTON里面
					Tx_Buffer[25] = Goto_slow_speed;      // BUTTON去程减速速度BUTTON里面回程减速速度BUTTON里面
					Tx_Buffer[26] = Return_slow_speed;      // BUTTON 回程减速速度BUTTON里面
          Tx_Buffer[26] = Zero_up_deceleration;    //零点上方距离
	



//					Tx_Buffer[0] = 1900;      //计算校准值AD1
//					Tx_Buffer[1] = 1995;      //计算校准值AD2
//					Tx_Buffer[2] = 2034;      //计算校准值AD3 
//					Tx_Buffer[3] = 2034;      //计算校准值AD4
//					Tx_Buffer[4] = 2000;      //位移系数
//					Tx_Buffer[5] = 20;      //零点重量
//					Tx_Buffer[6] = 0;       // BRB减速距离
//					Tx_Buffer[7] = 0;       // BRB减速速度
//					Tx_Buffer[8] = 0;       // BRB第二阶段速度调整系数
//					Tx_Buffer[9] = 0;       // BRB力保持上限
//					Tx_Buffer[10] = 0;      // BRB力保持下限
//					Tx_Buffer[11] = 0;      // BRB力保持力值
//					Tx_Buffer[12] = 200;      // BUTTON第一次下压时间
//					Tx_Buffer[13] = 1900;      // BUTTON第一次抬起时间
//					Tx_Buffer[14] = 200;      // BUTTON第一次等待时间
//					Tx_Buffer[15] = 170;      // BUTTON第二次下压时间
//					Tx_Buffer[16] = 200;      // BUTTON第二次抬起时间
//					Tx_Buffer[17] = 200;      // BUTTON第二次等待时间
//					Tx_Buffer[18] = 2000;      // BUTTON第三次下压时间
//					Tx_Buffer[19] = 2000;      // BUTTON第三次抬起时间
//					Tx_Buffer[20] = 2000;      // BUTTON第三次等待时间，与下次992间隔时间
//					Tx_Buffer[21] = 0;      // BUTTON位移减速距离
//					Tx_Buffer[22] = 0;      // BUTTON 减速距离BUTTON里面
//					Tx_Buffer[23] = 0;      // BUTTON压停距离BUTTON里面
//					Tx_Buffer[24] = 0;      // BUTTON回停距离BUTTON里面
//					Tx_Buffer[25] = 0;      // BUTTON去程减速速度BUTTON里面回程减速速度BUTTON里面
//					Tx_Buffer[26] = 0;      // BUTTON 回程减速速度BUTTON里面
//          Tx_Buffer[26] = 0.5;    //零点上方距离
	



//					Tx_Buffer[0] = 1900;      //计算校准值AD1
//					Tx_Buffer[1] = 1900;      //计算校准值AD2
//					Tx_Buffer[2] = 1900;      //计算校准值AD3 
//					Tx_Buffer[3] = 1900;      //计算校准值AD4
//					Tx_Buffer[4] = 0;      //计算校准值砝码
//					Tx_Buffer[5] = 0;      //计算校准值砝码
//					Tx_Buffer[6] = 0;      //计算校准值砝码 
//					Tx_Buffer[7] = 0;      //计算校准值砝码
//					Tx_Buffer[8] = 2000;             //位移系数
//					Tx_Buffer[9] = 10;                //零点重量

					vTaskDelay(10);
					stmflash_write(FLASH_SAVE_ADDR, (uint32_t *)Tx_Buffer, SIZE);
					

}


















