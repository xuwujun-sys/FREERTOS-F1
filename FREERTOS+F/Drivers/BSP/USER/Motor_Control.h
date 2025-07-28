#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_
#include "stm32f4xx_hal.h"
#include "math.h"
#include	"stdint.h"


//手动上，手动下，返回，点动上，点动下
#define Manual_up                   1   //手动上
#define Manual_Down                 2   //手动下
#define Manual_return               3   //手动返回
#define Jog_Up                      4   //点动上
#define Jog_Down                    5   //点动下
#define Manual_stop                 6   //手动停止
#define Distance_Zero		            7   //距离零
#define All_Zero		                8   //全部零
#define weight_Zero                 9   //力值清零
#define Start_testing               10  //启动测试
#define Stop_testing                11  //停止测试
#define Save_parameters             12  //确定参数保存

//测试速度关联
#define MOTOR_UP_UP			            13   //测试速度上 Second speed
#define MOTOR_UP_Second		          14   //测试速度上 Second speed
#define MOTOR_UP_third		          15   //测试速度上 third  speed
#define MOTOR_UP_fourth		          16   //保持上     fourth speed
#define MOTOR_DOWN_DOWN			        17   //测试速度下
#define MOTOR_DOWN_Second		        18   //测试速度下 Second speed
#define MOTOR_DOWN_third		        19   //测试速度下 third speed
#define MOTOR_DOWN_fourth		        20   //保持上 
#define MOTOR_Correlation_force		  21    //关联力

#define MOTOR_ORIGIN                22   //原点return
#define MOTOR_RETURN                23   //返回
#define MOTOR_STOPREAL_STOP		      24   //真实停止，打算直接关闭PWM
#define MOTOR_STOP_STOP    		      25   //真实停止，打算直接关闭PWM

#define MOTOR_Plug_in	              26   //插拔上
#define MOTOR_Plug_down   		      27   //插拔下



#define FORCE_CUT     			0x01   //拉断返回
#define FORCE_DISPLACEMENT_LOOP	0x02  //定力定位移返回
#define FORCE_HOLD  			0x03   //保持力
#define KEY_TEST    			0x04

void Motor_Control_draw(unsigned char Test_Mode);
void Motor_Control_press(unsigned char Test_Mode);
unsigned char Limit_Return(void);
void	MotorAction_Control(uint8_t	State);

void specifyDisplacement(void);
void specifyDisplacement_down(void);
void specifyDisplacement_up(void);

void motorReturnZero(void);

void Key1mm_down(void);
void Key1mm_up(void);
void Key01mm_down(void);
void Key01mm_up(void);
void RUN(int CCC);


//void ledled(void);

#endif
