#ifndef __MESSAGE_PROCESSING_H__
#define __MESSAGE_PROCESSING_H__

#include "stm32f4xx_hal.h"
#include "math.h"
#include	"stdint.h"






extern uint32_t Calibration_weight[4];              //AD值，计算校准
extern int32_t Calibration_load[4];                //校准荷重
extern int32_t Calibration_mark[4];                //校准标志
extern int32_t Weight_calibration_old[4];          //旧的值
extern int32_t Weight_calibration[4];              
extern int32_t Weight_A[4];              
extern long double displacement_speed[2];  
extern uint32_t Manual_Speed;
	
	
extern int coefficient;

extern float Jog_distance;         //点动距离extern 
extern int Pulling_force;          //拉升力标志
extern int Compressive_force;      //压缩力标志
extern int two_way;                //拉升压缩力标志
extern float Test_force_range;     //测试力值范围
extern float Test_distance_range;  //测试距离范围
extern int Initial_Position;       //初始位置
extern int Zero_load;              //荷重零点标志
extern int Origin_detection;       //零点检出
extern uint32_t test_speed;        //测试速度
extern uint32_t return_speed;      //回程速度
extern uint32_t Waiting_location;  //等候位置
extern uint32_t Waiting_Time;      //等候时间
extern int Test_Mode1;              //测试模式
extern float Holdtime;             //保持时间、停留时间
extern uint32_t stop_position;     //停止位置
extern float Testquantity;         //测试次数
extern uint32_t Maintain_speed;        //保持速度速度

extern float Zero_weight;                //零点荷重

extern float Decreasing_coefficient;     //去减速速度系数
extern float Decreasing_Speed;     //去速度
extern float Decreasing_coefficient_2;     //
extern float Decreasing_Speed_2;     //去
extern float Decreasing_coefficient_weight2;     //
extern int Weight_Speed_coefficient;  //第二阶段减速的系数，乘以当前力值，去调整速度
extern float Return_coefficient;         //回系数
extern float Retention_coefficient;      //保持力系数
extern float upper_limit_coefficient;      //上限系数
extern float lower_limit_coefficient;      //下限系数
extern float Decreasing_Retention_coefficient;      //去时减速重量 
extern float Retention_force;      //保持力g
extern float upper_limit_force;      //上限g
extern float lower_limit_force;      //下限g
extern int32_t coefficient_A[4];    //系数锁A
extern int32_t coefficienfloat;    //力值校准系数t_B[4];    //系数锁B
extern float Zero_up_deceleration;  //零点上方距离

extern float Testtimes;                  //测试次数

extern  float Distance_value;                //计算距离除以的系数

extern float chabajuli;

extern float current_displacement;


extern uint8_t move_flag;       	//位移标志位

extern uint8_t last_direction;		// 上次位移方向
extern int return_direction;// 回位方向
extern uint8_t move_up_flag;       //位移标志位
extern uint8_t move_down_flag;       //位移标志位
//extern uint8_t direction;			// 位移方向
extern uint8_t last_direction;		// 上次位移方向

extern float weight_coefficient;    //力值校准系数
extern float Displacement_deceleration_coefficient; //位移减速系数，button模式中使用
extern int First_uplift_time;   //第一次抬起时间
extern int First_pushing_time;  //第一次下压时间
extern int First_wait_time;     //第一次等待时间
extern int Second_uplift_time;  //第二次抬起时间
extern int Second_pushing_time; //第二次下压时间
extern int Second_wait_time;    //第二次等待时间
extern int Third_uplift_time;   //第三次抬起时间
extern int Third_pushing_time;  //第三次下压时间
extern int Third_wait_time;     //第三次等待时间，与下次992间隔时间
extern float Displacement_deceleration; //位移减速距离
//  Test_distance_range //位移范围
extern	float Press_stop_coefficient;  //压停系数 BUTTON里面
extern	float Press_stop_deceleration;  //压停距离BUTTON里面
extern	float Slow_down_coefficient;   //减速系数BUTTON里面
extern	float Slow_down_deceleration;   //减速距离BUTTON里面
extern	float Return_stop_coefficient;  //回停系数 BUTTON里面
extern	float Return_stop_deceleration;  //回停距离BUTTON里面
extern	float Goto_slow_speed;      //去程减速速度BUTTON里面
extern	float Return_slow_speed;      //回程减速速度BUTTON里面

extern int BRB_time;//BRB间隔等待时间

extern int times_fug;//992次停标志
extern int times_bug;//992次停标志

void Serial_message_processing(void);


#endif	





