#include "includes.h"


//g_rx_buffer[100],已定义，最大100字节
//////////////////////////////////////////////////////////暂且定义，之后有些要改为全局变量，还未改，注意与之前需不重复，且需要把之前删掉

float Jog_distance_1=0;         //点动距离
float Jog_distance=0;         //点动距离
int Pulling_force=0;          //拉升力标志
int Compressive_force=0;      //压缩力标志
int two_way=0;                //拉升压缩力标志
float Test_force_range=0;     //测试力值范围
float Test_distance_range=0;  //测试距离范围
int Initial_Position=0;       //初始位置
int Zero_load=1;              //荷重零点标志
int Origin_detection=0;       //零点检出
uint32_t test_speed=0;        //测试速度
uint32_t return_speed=0;      //回程速度
uint32_t Waiting_location=0;  //等候位置
uint32_t Waiting_Time=0;      //等候时间
int Test_Mode1=0;              //测试模式
float Holdtime=0;             //保持时间、停留时间
uint32_t stop_position=0;     //停止位置
float Testquantity=0;         //测试次数




uint8_t move_flag = 0;       //位移标志位
int return_direction = 0;
uint8_t last_direction = 0;			// 上次位移方向

uint32_t Maintain_speed=2;        //保持速度速度
//float Weight_zero_point=0;        //零点荷重

float weight_coefficient = 1;    //力值校准系数
float Zero_weight=0;                //零点荷重
float Zero_up_deceleration=0.5;                //零点上方距离
float Decreasing_coefficient=0;     //去减速速度系数
float Decreasing_Speed=0;     //去速度
float Decreasing_coefficient_2=0;     //去减速速度系数
float Decreasing_Speed_2=0;     //去速度
float Decreasing_coefficient_weight2=0;     //去减速速度系数
int Weight_Speed_coefficient=0;   //第二阶段减速的系数，乘以当前力值，去调整速度
float Return_coefficient=0;         //回系数
float Retention_coefficient=0;      //保持力系数  1000
float upper_limit_coefficient=0;      //上限系数   1000
float lower_limit_coefficient=0;      //下限系数   1000
float Decreasing_Retention_coefficient=0;      //去时减速重量 
float Retention_force=0;      //保持力g
float upper_limit_force=0;      //上限g
float lower_limit_force=0;      //下限g

int32_t coefficient_A[4]={0,0,0,0};    //系数锁A
int32_t coefficient_B[4]={0,0,0,0};    //系数锁B

float Testtimes=0;                  //测试次数
uint32_t Manual_Speed  = 50000;     //手动速度
float chabajuli=0;

int odd_change_fast=0;    //找零快速
int odd_change_middle=0;  //找零中速
int odd_change_slow=0;    //找零慢速
float Displacement_deceleration_coefficient=0; //位移减速系数，button模式中使用
int First_uplift_time=0;   //第一次抬起时间BUTTON里面
int First_pushing_time=0;  //第一次下压时间BUTTON里面
int First_wait_time=0;     //第一次等待时间BUTTON里面
int Second_uplift_time=0;  //第二次抬起时间BUTTON里面
int Second_pushing_time=0; //第二次下压时间BUTTON里面
int Second_wait_time=0;    //第二次等待时间BUTTON里面
int Third_uplift_time=0;   //第三次抬起时间BUTTON里面
int Third_pushing_time=0;  //第三次下压时间BUTTON里面
int Third_wait_time=0;     //第三次等待时间，与下次992间隔时间  BUTTON里面
float Displacement_deceleration=0; //位移减速距离
float Press_stop_coefficient=0;  //压停系数 BUTTON里面
float Press_stop_deceleration=0;  //压停距离BUTTON里面
float Slow_down_coefficient=0;   //减速系数BUTTON里面
float Slow_down_deceleration=0;   //减速距离BUTTON里面
float Return_stop_coefficient=0;  //回停系数 BUTTON里面
float Return_stop_deceleration=0;  //回停距离BUTTON里面
float Goto_slow_speed=0;      //去程减速速度BUTTON里面
float Return_slow_speed=0;      //回程减速速度BUTTON里面


uint32_t Calibration_weight[4]={0,0,0,0};              //AD值，计算校准
int32_t Calibration_load[4]={0,0,0,0};                //校准荷重
int32_t Calibration_mark[4]={0,0,0,0};                //校准标志

int32_t Weight_calibration[4]={0,0,0,0};                //校准xishu 
int coefficient = 0;
int32_t Weight_calibration_old[4]={0,0,0,0};     //旧的值
int32_t Weight_A[4]={1,1,1,1}; //校准系数锁值

long double displacement_speed[2]={1,1};


float coefficient_Distance=0;             //位移系数
float coefficient_speed=0;                //速度系数
float Distance_value=2000;                //计算距离除以的系数

float current_displacement = 0.0f;   // 

int Distance_coefficient = 0;       //位移标志
int speed_coefficient = 0;          //速度标志


extern int BBB;

extern int AAA;
extern int DDD;
extern int E;
extern int CCC;
extern int status_flag;
extern int BRB_A;
extern int BRB_B;

int times_fug=0;
int times_bug=0;
int BRB_time=1;
///////////////////////////////////////////////////////////////////串行消息接收处理
void Serial_message_processing(void)
{

	
			if(g_usart_rx_buf[0]==0XAA)
			{

				switch(g_usart_rx_buf[1])
				{
					case 0X00:            //手动控制
					{

					
						switch(g_usart_rx_buf[2])
						{
							case 0X01:         //手动上
							{
								g_usart_rx_buf[2]=0X00;
								vTaskDelay(2);
								SpeedMotorInit(MOTORNUM1);
								vTaskDelay(2);
								SetSpeed(MOTORNUM1,200000);
								MotorAction(MOTORNUM1,MOTOR_UP);
								vTaskDelay(2);
								last_direction = 1;
								if(LimNega == TRUE)
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
									move_flag = 0;
									BBB=3;BRB_A=999;BRB_B=999;
									CCC=999;AAA=999;	
								}
								break;
							}
							case 0X02:         //手动下
							{
								g_usart_rx_buf[2]=0X00;
								vTaskDelay(2);
								SpeedMotorInit(MOTORNUM1);
								vTaskDelay(2);
								SetSpeed(MOTORNUM1,200000);//Manual_Speed
								MotorAction(MOTORNUM1,MOTOR_DOWN);
								vTaskDelay(2);
								last_direction = 0;
								if(LimNega == TRUE)
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
									move_flag = 0;
									BBB=3;BRB_A=999;BRB_B=999;
									CCC=999;AAA=999;	
								}
								break;
							}
							case 0X03:         //手动停止
							{
								g_usart_rx_buf[2]=0X00;
								MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP		
								HAL_NVIC_DisableIRQ(TIM7_IRQn);
								time_count_1s=0;
								time_count_100ms=0;
								move_flag = 0;
								status_flag = 0;
								times_bug=99;
								BBB=3;BRB_A=999;BRB_B=999;
								DDD=999;
								CCC=999;AAA=999;
								MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP						
								break;
							}
							case 0X04:         //回原点
							{
								g_usart_rx_buf[2]=0X00;
								if(displacement1 > 0.000f)
								{
									return_direction = 0;
								}
								if(displacement1 < 0.000f)
								{
									return_direction = 1;
								}
								move_flag = 3;
								break;
							}
							case 0X05:         //重量清零
							{
								
								g_usart_rx_buf[2]=0X00;
								weight_Zero_Data=weight_ad7190_ReadAvg(3);//filter()
								weight_Zero_Data=weight_ad7190_ReadAvg(3);
//								weight_Zero_Data=weight_ad7190_ReadAvg(3);//filter()
								break;
							}
							case 0X06:         //位移清零
							{
								g_usart_rx_buf[2]=0X00;
								MotorAction(MOTORNUM1,MOTOR_STOP);
								ClearTimer();
								break;
							}
							case 0X07:         //全部清零
							{
								g_usart_rx_buf[2]=0X00;
//								MotorAction(All_Zero);
								weight_Zero_Data=weight_ad7190_ReadAvg(3);
								MotorAction(MOTORNUM1,MOTOR_STOP);
								ClearTimer();
								break;
							}
					 		case 0X08:         //设置手动速度，手动速度在电机控制方式中定义，有个默认值20，但还是要设置
							{
								g_usart_rx_buf[2]=0X00;
								vTaskDelay(2); 
								Manual_Speed=g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收到的速度
								Manual_Speed=(Manual_Speed/10)*835;//速度系数，应该是833，3循坏
//								if(Manual_Speed<=400)
//								{
//									Manual_Speed=400;
//								}
								break;
							}
							case 0X09:         //设置点动距离
							{
								g_usart_rx_buf[2]=0X00;
								vTaskDelay(2); 
								Jog_distance=g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收的是mm
								Jog_distance=(Jog_distance/1000);//接收的mm乘以每个毫米所走的脉冲数
								break;
							}
							case 0X0A:         //点动上
							{
								g_usart_rx_buf[2]=0X00;
								if(last_direction == 0)
								{
									current_displacement = Jog_distance + displacement1;
								}
								else if(last_direction == 1)
								{
									current_displacement = Jog_distance + displacement1;
								}
								if(Jog_distance>0)
								{
									move_flag = 1;
								}
								if(LimNega == TRUE)
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
									move_flag = 0;
									BBB=3;BRB_A=999;
									CCC=999;AAA=999;	
								}					
								break;
							}
							case 0X0B:         //点动下
							{
								g_usart_rx_buf[2]=0X00;
								if(last_direction == 0)
								{
									current_displacement = displacement1 - Jog_distance;
								
								}else if(last_direction == 1)
								{
									current_displacement =displacement1 - Jog_distance;
								
								}
								
								if(Jog_distance>0)
								{
									move_flag = 2;
								}
								if(LimNega == TRUE)
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
									move_flag = 0;
									BBB=3;BRB_A=999;
									CCC=999;AAA=999;	
								}

								
								break;
							}
//							case 0X0C:         //启动测试,见主程序，主任务里面
//							{`
//						  
//								break;
//							}
//							case 0X0D:        //启动测试,见主程序，主任务里面
//							{
//								g_usart_rx_buf[2]=0X00;
////							  MotorAction(Stop_testing);
//								break;
//							}
							case 0X0E:         //确定参数保存
							{
								g_usart_rx_buf[2]=0X00;
//								MotorAction(Save_parameters);
								break;
							}
							case 0X0F:        //频率
							{
								g_usart_rx_buf[2]=0X00;
								
//								Rate_variable = g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收的是数据发送的频率快慢的一个大概系数，必须为整数
//								
								break;
							}

					
							case 0X10:        //BUTTON内部参数
							{
								g_usart_rx_buf[2]=0X00;	
								if((g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0)||(g_usart_rx_buf[6]!=0))////BUTTON模式下的位移减速的系数是%，乘距离得到减速距离
								{////BUTTON模式下的位移减速的系数是%，乘距离得到减速距离
									vTaskDelay(1); 
									Displacement_deceleration_coefficient = g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1; 
									Displacement_deceleration_coefficient = Displacement_deceleration_coefficient /1000;
									Displacement_deceleration = Test_distance_range * Displacement_deceleration_coefficient;
								}
								
								if((g_usart_rx_buf[7]!=0)||(g_usart_rx_buf[8]!=0)||(g_usart_rx_buf[9]!=0)||(g_usart_rx_buf[10]!=0))////第一个9抬起时间
								{////第一个9抬起时间
									vTaskDelay(1); 
									First_uplift_time=g_usart_rx_buf[7]*16777216+g_usart_rx_buf[8]*65536+g_usart_rx_buf[9]*256+g_usart_rx_buf[10]*1; 						
							
								}
								
								if((g_usart_rx_buf[11]!=0)||(g_usart_rx_buf[12]!=0)||(g_usart_rx_buf[13]!=0)||(g_usart_rx_buf[14]!=0))//////第一个9下压时间
								{//////第一个9下压时间
									vTaskDelay(1); 
									First_pushing_time=g_usart_rx_buf[11]*16777216+g_usart_rx_buf[12]*65536+g_usart_rx_buf[13]*256+g_usart_rx_buf[14]*1; 	
								}			
								
							if((g_usart_rx_buf[15]!=0)||(g_usart_rx_buf[16]!=0)||(g_usart_rx_buf[17]!=0)||(g_usart_rx_buf[18]!=0))////第一个9与第二个9的间隔时间
								{////第一个9与第二个9的间隔时间
									vTaskDelay(1); 
									First_wait_time=g_usart_rx_buf[15]*16777216+g_usart_rx_buf[16]*65536+g_usart_rx_buf[17]*256+g_usart_rx_buf[18]*1; 
								}
							if((g_usart_rx_buf[19]!=0)||(g_usart_rx_buf[20]!=0)||(g_usart_rx_buf[21]!=0)||(g_usart_rx_buf[22]!=0))////第二个9抬起时间
								{////第二个9抬起时间
									vTaskDelay(1); 
									Second_uplift_time=g_usart_rx_buf[19]*16777216+g_usart_rx_buf[20]*65536+g_usart_rx_buf[21]*256+g_usart_rx_buf[22]*1; 								
								}					
								if((g_usart_rx_buf[23]!=0)||(g_usart_rx_buf[24]!=0)||(g_usart_rx_buf[25]!=0)||(g_usart_rx_buf[26]!=0))////第二个9下压时间
								{////第二个9下压时间
									vTaskDelay(1); 
									Second_pushing_time=g_usart_rx_buf[23]*16777216+g_usart_rx_buf[24]*65536+g_usart_rx_buf[25]*256+g_usart_rx_buf[26]*1; 
					
								}		
								if((g_usart_rx_buf[27]!=0)||(g_usart_rx_buf[28]!=0)||(g_usart_rx_buf[29]!=0)||(g_usart_rx_buf[30]!=0))////第二个9与第三个2的间隔时间
								{////第二个9与第三个2的间隔时间
									vTaskDelay(1); 
									Second_wait_time=g_usart_rx_buf[27]*16777216+g_usart_rx_buf[28]*65536+g_usart_rx_buf[29]*256+g_usart_rx_buf[30]*1; 
								}								
								if((g_usart_rx_buf[31]!=0)||(g_usart_rx_buf[32]!=0)||(g_usart_rx_buf[33]!=0)||(g_usart_rx_buf[34]!=0))////第三个2抬起时间
								{////第三个2抬起时间
									vTaskDelay(1); 
									Third_uplift_time=g_usart_rx_buf[31]*16777216+g_usart_rx_buf[32]*65536+g_usart_rx_buf[33]*256+g_usart_rx_buf[34]*1; 
								}					
								if((g_usart_rx_buf[35]!=0)||(g_usart_rx_buf[36]!=0)||(g_usart_rx_buf[37]!=0)||(g_usart_rx_buf[38]!=0))////第三个2下压时间
								{////第三个2下压时间
									vTaskDelay(1); 
									Third_pushing_time=g_usart_rx_buf[35]*16777216+g_usart_rx_buf[36]*65536+g_usart_rx_buf[37]*256+g_usart_rx_buf[38]*1; 
								
								}		
								if((g_usart_rx_buf[39]!=0)||(g_usart_rx_buf[40]!=0)||(g_usart_rx_buf[41]!=0)||(g_usart_rx_buf[42]!=0))////与下一个992的循环等待时间
								{////与下一个992的循环等待时间
									vTaskDelay(1); 
									Third_wait_time=g_usart_rx_buf[39]*16777216+g_usart_rx_buf[40]*65536+g_usart_rx_buf[41]*256+g_usart_rx_buf[42]*1; 
								}	
//	Press_stop_deceleration							
								if((g_usart_rx_buf[43]!=0)||(g_usart_rx_buf[44]!=0)||(g_usart_rx_buf[45]!=0)||(g_usart_rx_buf[46]!=0))////压停系数 BUTTON里面    压停距离BUTTON里面
								{////压停系数 BUTTON里面    压停距离BUTTON里面
									vTaskDelay(1); 
									Press_stop_coefficient=g_usart_rx_buf[43]*16777216+g_usart_rx_buf[44]*65536+g_usart_rx_buf[45]*256+g_usart_rx_buf[46]*1; 
									Press_stop_coefficient = Press_stop_coefficient / 1000;
									Press_stop_deceleration = Press_stop_coefficient * Test_distance_range;
								}
								if((g_usart_rx_buf[47]!=0)||(g_usart_rx_buf[48]!=0)||(g_usart_rx_buf[49]!=0)||(g_usart_rx_buf[50]!=0))////减速系数BUTTON里面   减速距离BUTTON里面
								{////减速系数BUTTON里面   减速距离BUTTON里面
									vTaskDelay(1); 
									Slow_down_coefficient=g_usart_rx_buf[47]*16777216+g_usart_rx_buf[48]*65536+g_usart_rx_buf[49]*256+g_usart_rx_buf[50]*1; 
									Slow_down_coefficient = Slow_down_coefficient / 100;
									Slow_down_deceleration = Slow_down_coefficient * Test_distance_range;
								}
								if((g_usart_rx_buf[51]!=0)||(g_usart_rx_buf[52]!=0)||(g_usart_rx_buf[53]!=0)||(g_usart_rx_buf[54]!=0))////回停系数 BUTTON里面    回停距离BUTTON里面
								{////回停系数 BUTTON里面    回停距离BUTTON里面
									vTaskDelay(1); 
									Return_stop_coefficient=g_usart_rx_buf[51]*16777216+g_usart_rx_buf[52]*65536+g_usart_rx_buf[53]*256+g_usart_rx_buf[54]*1; 
									Return_stop_coefficient = Return_stop_coefficient /1000;
									Return_stop_deceleration = Return_stop_coefficient * Test_distance_range;
								}
								if((g_usart_rx_buf[55]!=0)||(g_usart_rx_buf[56]!=0)||(g_usart_rx_buf[57]!=0)||(g_usart_rx_buf[58]!=0))////去程减速速度BUTTON里面
								{////去程减速速度BUTTON里面
									vTaskDelay(1); 
									Goto_slow_speed=g_usart_rx_buf[55]*16777216+g_usart_rx_buf[56]*65536+g_usart_rx_buf[57]*256+g_usart_rx_buf[58]*1; 
									Goto_slow_speed = Goto_slow_speed*835;
								}
								if((g_usart_rx_buf[59]!=0)||(g_usart_rx_buf[60]!=0)||(g_usart_rx_buf[61]!=0)||(g_usart_rx_buf[62]!=0))////////回程减速速度BUTTON里面
								{////回程减速速度BUTTON里面
									vTaskDelay(1); 
									Return_slow_speed=g_usart_rx_buf[59]*16777216+g_usart_rx_buf[60]*65536+g_usart_rx_buf[61]*256+g_usart_rx_buf[62]*1; 
									Return_slow_speed = Return_slow_speed*835;
								}	
								if(Displacement_deceleration!=0)
								{
									vTaskDelay(1); 
									writingflash();
									vTaskDelay(1); 
								}	
								break;
							}
							case 0X11:        //
							{
								g_usart_rx_buf[2]=0X00;		
								if((g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0)||(g_usart_rx_buf[6]!=0))////零点重量
								{////零点重量
									vTaskDelay(1); 
									Zero_weight = g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1; 		
								}								
								if(Zero_weight!=0)
								{
									writingflash();
								}	
								break;
							}
							case 0X12:        //
							{
								g_usart_rx_buf[2]=0X00;		
								if((g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0)||(g_usart_rx_buf[6]!=0))////零点位置
								{////零点位置
									vTaskDelay(1); 
									Zero_up_deceleration = g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1; 	
									Zero_up_deceleration = Zero_up_deceleration/1000;			
								}		
							if(Zero_up_deceleration!=0)
								{
									writingflash();
								}	
								break;
							}
							case 0X13:        //力值校准系数，默认1，发多少除1000，1000即是1
							{
								g_usart_rx_buf[2]=0X00;					
				        weight_coefficient = g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收的是测试次数必须为整数
								weight_coefficient = weight_coefficient/1000;
								break;
							}
							case 0X14:        //
							{
								g_usart_rx_buf[2]=0X00;					
				
								break;
							}
							case 0X15:        //
							{
								g_usart_rx_buf[2]=0X00;					
				
								break;
							}
							
							case 0X1A:        //测试次数
							{
								g_usart_rx_buf[2]=0X00;
								Testtimes =  g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收的是测试次数必须为整数
							  times_fug=99;			
								break;
							}
							case 0X1B:        //更改保持力的速度
							{
								g_usart_rx_buf[2]=0X00;
                Maintain_speed=g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1;//接收到的速度
								Maintain_speed=Maintain_speed/10;
								if(Maintain_speed <= 2)
								{
									Maintain_speed=2;
								}
								break;
							}
							case 0X1C:        //测试时停止，只是停止
							{
								g_usart_rx_buf[2]=0X00;
								LED2_TOGGLE();
//								STEPMOTOR_AxisMoveRel(6000*CCW,40,40,2);	
								break;
							}
							case 0X1D:        //测试时停止，勾选返回原点
							{
								g_usart_rx_buf[2]=0X00;
								LED3_TOGGLE();
//							  STEPMOTOR_AxisMoveRel(6000*CW,40,40,2);	

								break;
							}		
							case 0X1E:        //测试用，空
							{
								g_usart_rx_buf[2]=0X00;
								LED4_TOGGLE();


								break;
							}
							
						
						}
						
						
					 break;		
					}
	//////////////////////////////////////////////////////////////////////////				
					case 0X01: //运行模式
					{
						switch(g_usart_rx_buf[2])////直接判断第三位是什么值，给予相应的变量
						{
							case 0X00:
							{
								Pulling_force=1;//BRB机模式
								Compressive_force=0;//BUTTON模式
								two_way=0;//插拔模式
//								Distance_value = 20000;
								break;
							}
							case 0X01:
							{

								Pulling_force=0;//BRB模式
								Compressive_force=1;//BUTTON模式
								two_way=0;//插拔模式
//								Distance_value = 2000;
								break;
							}
							
						}
						if((g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0)||(g_usart_rx_buf[6]!=0))//测试力值范围
						{
							vTaskDelay(1); 
							Test_force_range=g_usart_rx_buf[3]*16777216+g_usart_rx_buf[4]*65536+g_usart_rx_buf[5]*256+g_usart_rx_buf[6]*1; 
							
						}
		
						if((g_usart_rx_buf[7]!=0)||(g_usart_rx_buf[8]!=0)||(g_usart_rx_buf[9]!=0)||(g_usart_rx_buf[10]!=0))//测试距离范围
						{
							vTaskDelay(1); 
							Test_distance_range = g_usart_rx_buf[7]*16777216+g_usart_rx_buf[8]*65536+g_usart_rx_buf[9]*256+g_usart_rx_buf[10]*1; 
							if((Pulling_force==0)&&(Compressive_force==1)&&(two_way==0))//如果是992模式，距离加上零点距离
							{
								Test_distance_range = (Test_distance_range/1000);
								Test_distance_range = Test_distance_range + Zero_up_deceleration;
							}
							else  //不是，就是发多少就是多少
							{
								Test_distance_range = (Test_distance_range/1000);
							}
								
						}
						
						switch(g_usart_rx_buf[11])////直接判断第11位是什么值，给予相应的变量
						{
							case 0X00:
							{
								Initial_Position=1;//初始位置
								Zero_load=0;//荷重零点位置
								break;
							}
							case 0X01:
							{
								Initial_Position=0;//初始位置
								Zero_load=1;//荷重零点位置
								break;
							}
						}

           switch(g_usart_rx_buf[12])////直接判断第12位是什么值，给予相应的变量
						{
							case 0X00:
							{
							  Origin_detection=1;       //零点检出，只检测一次
								break;
							}
							case 0X01:
							{
								Origin_detection=2;       //零点检出，每次检测
								break;
							}
						}	

        	 if((g_usart_rx_buf[13]!=0)||(g_usart_rx_buf[14]!=0)||(g_usart_rx_buf[15]!=0)||(g_usart_rx_buf[16]!=0))//测试速度
						{
							vTaskDelay(1); 
							test_speed=g_usart_rx_buf[13]*16777216+g_usart_rx_buf[14]*65536+g_usart_rx_buf[15]*256+g_usart_rx_buf[16]*1; 
							test_speed=test_speed*835;//835为速度系数，准确来说是833.3,3循坏
//							if(test_speed<7)
//							{
//								test_speed=7;
//							}
						}
						
						if((g_usart_rx_buf[17]!=0)||(g_usart_rx_buf[18]!=0)||(g_usart_rx_buf[19]!=0)||(g_usart_rx_buf[20]!=0))//回程速度
						{
							vTaskDelay(1); 
							return_speed=g_usart_rx_buf[17]*16777216+g_usart_rx_buf[18]*65536+g_usart_rx_buf[19]*256+g_usart_rx_buf[20]*1; 
							return_speed=return_speed*835;//835为速度系数，准确来说是833.3,3循坏
//							if(return_speed<7)
//							{
//								return_speed=7;
//							}
						}

						
						if((g_usart_rx_buf[21]!=0)||(g_usart_rx_buf[22]!=0)||(g_usart_rx_buf[23]!=0)||(g_usart_rx_buf[24]!=0))//每次等候位置
						{
							vTaskDelay(1); 
							Waiting_location=g_usart_rx_buf[21]*16777216+g_usart_rx_buf[22]*65536+g_usart_rx_buf[23]*256+g_usart_rx_buf[24]*1; 
							Waiting_location=(Waiting_location/1000)*Distance_value;	
						}
						
            if((g_usart_rx_buf[25]!=0)||(g_usart_rx_buf[26]!=0)||(g_usart_rx_buf[27]!=0)||(g_usart_rx_buf[28]!=0))//每次等候时间
						{
							vTaskDelay(1); 
							Waiting_Time=g_usart_rx_buf[25]*16777216+g_usart_rx_buf[26]*65536+g_usart_rx_buf[27]*256+g_usart_rx_buf[28]*1; 
						}

						switch(g_usart_rx_buf[29])////直接判断第29位是什么值，给予相应的变量，模式
						{
							case 0X00:
							{
							  Test_Mode1=1;       //直接返回
								break;
							}
							case 0X01:
							{
								Test_Mode1=2;       //力保持模式
								break;
							}
						}											
						
						if((g_usart_rx_buf[30]!=0)||(g_usart_rx_buf[31]!=0)||(g_usart_rx_buf[32]!=0)||(g_usart_rx_buf[33]!=0))//保持时间，停留时间
						{
							vTaskDelay(1); 
							Holdtime=g_usart_rx_buf[30]*16777216+g_usart_rx_buf[31]*65536+g_usart_rx_buf[32]*256+g_usart_rx_buf[33]*1; 
							Holdtime=(Holdtime/10)*10;
						}
						
						if((g_usart_rx_buf[34]!=0)||(g_usart_rx_buf[35]!=0)||(g_usart_rx_buf[36]!=0)||(g_usart_rx_buf[37]!=0))//停止位置，零点上方或者下方
						{
							vTaskDelay(1); 
							stop_position=g_usart_rx_buf[34]*16777216+g_usart_rx_buf[35]*65536+g_usart_rx_buf[36]*256+g_usart_rx_buf[37]*1; 
							stop_position=(stop_position/1000)*Distance_value;
						}

						if((g_usart_rx_buf[38]!=0)||(g_usart_rx_buf[39]!=0)||(g_usart_rx_buf[40]!=0)||(g_usart_rx_buf[41]!=0))//测试次数
						{
							vTaskDelay(1); 
							Testquantity=g_usart_rx_buf[38]*16777216+g_usart_rx_buf[39]*65536+g_usart_rx_buf[40]*256+g_usart_rx_buf[41]*1; 
						
						}

						
						break;
					}		
					

				 case 0X02:   // 均没有开始计算，去回保持系数要除以100，在进行计算 
					{
						if((g_usart_rx_buf[2]!=0)||(g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0))////第一阶段减速系数*100发我，是百分比
						{
							vTaskDelay(1); 
							Decreasing_coefficient = g_usart_rx_buf[2]*16777216+g_usart_rx_buf[3]*65536+g_usart_rx_buf[4]*256+g_usart_rx_buf[5]*1; 
							Decreasing_coefficient = Decreasing_coefficient /1000;
							Decreasing_Retention_coefficient = Test_force_range*Decreasing_coefficient;
//							if(Decreasing_Retention_coefficient <= Test_force_range/5)
//							{
//								Decreasing_Retention_coefficient = Test_force_range/5;
//							}							
						}
						
						if((g_usart_rx_buf[6]!=0)||(g_usart_rx_buf[7]!=0)||(g_usart_rx_buf[8]!=0)||(g_usart_rx_buf[9]!=0))////第一阶段减速速度，没校准，之后再改这个值
						{
							vTaskDelay(1); 
							Decreasing_Speed=g_usart_rx_buf[6]*16777216+g_usart_rx_buf[7]*65536+g_usart_rx_buf[8]*256+g_usart_rx_buf[9]*1; 						
//							Decreasing_Speed = Decreasing_Speed*835;
							Decreasing_Speed = Decreasing_Speed;
						}
						
						if((g_usart_rx_buf[10]!=0)||(g_usart_rx_buf[11]!=0)||(g_usart_rx_buf[12]!=0)||(g_usart_rx_buf[13]!=0))//////第二阶段减速的系数，乘以当前力值，去调整速度
						{
							vTaskDelay(1); 
							Weight_Speed_coefficient=g_usart_rx_buf[10]*16777216+g_usart_rx_buf[11]*65536+g_usart_rx_buf[12]*256+g_usart_rx_buf[13]*1; 
							Weight_Speed_coefficient = Weight_Speed_coefficient/10;					
						}			
						
					if((g_usart_rx_buf[14]!=0)||(g_usart_rx_buf[15]!=0)||(g_usart_rx_buf[16]!=0)||(g_usart_rx_buf[17]!=0))////保持力上限系数，系数是*1000发过来，我这边要除以1000
						{
							vTaskDelay(1); 
							upper_limit_coefficient=g_usart_rx_buf[14]*16777216+g_usart_rx_buf[15]*65536+g_usart_rx_buf[16]*256+g_usart_rx_buf[17]*1; 
							upper_limit_coefficient = upper_limit_coefficient/1000;
							upper_limit_force = Test_force_range*upper_limit_coefficient;
							
						}
					if((g_usart_rx_buf[18]!=0)||(g_usart_rx_buf[19]!=0)||(g_usart_rx_buf[20]!=0)||(g_usart_rx_buf[21]!=0))////保持力下限系数，系数是*1000发过来，我这边要除以1000
						{
							vTaskDelay(1); 
							lower_limit_coefficient=g_usart_rx_buf[18]*16777216+g_usart_rx_buf[19]*65536+g_usart_rx_buf[20]*256+g_usart_rx_buf[21]*1; 
							lower_limit_coefficient = lower_limit_coefficient/1000;
							lower_limit_force = Test_force_range*lower_limit_coefficient;
							
						}					
						if((g_usart_rx_buf[22]!=0)||(g_usart_rx_buf[23]!=0)||(g_usart_rx_buf[24]!=0)||(g_usart_rx_buf[25]!=0))////保持力系数，系数是*1000发过来，我这边要除以1000
						{
							vTaskDelay(1); 
							Retention_coefficient=g_usart_rx_buf[22]*16777216+g_usart_rx_buf[23]*65536+g_usart_rx_buf[24]*256+g_usart_rx_buf[25]*1; 
							Retention_coefficient = Retention_coefficient/1000;
							Retention_force = Test_force_range*Retention_coefficient;   //Test_force_range
							
						}
						if((g_usart_rx_buf[26]!=0)||(g_usart_rx_buf[27]!=0)||(g_usart_rx_buf[28]!=0)||(g_usart_rx_buf[29]!=0))////保持力系数，系数是*1000发过来，我这边要除以1000
						{//BRB间隔等待时间
							vTaskDelay(1); 
							BRB_time=g_usart_rx_buf[26]*16777216+g_usart_rx_buf[27]*65536+g_usart_rx_buf[28]*256+g_usart_rx_buf[29]*1; 
							BRB_time = BRB_time/10;				
						}

					if((g_usart_rx_buf[30]!=0)||(g_usart_rx_buf[31]!=0)||(g_usart_rx_buf[32]!=0)||(g_usart_rx_buf[33]!=0))////第二阶段减速百分比
						{
							vTaskDelay(1); 
							Decreasing_coefficient_2=g_usart_rx_buf[30]*16777216+g_usart_rx_buf[31]*65536+g_usart_rx_buf[32]*256+g_usart_rx_buf[33]*1; 
							Decreasing_coefficient_2 = Decreasing_coefficient_2/1000;
							Decreasing_coefficient_weight2 = Test_force_range*Decreasing_coefficient_2;   //Test_force_range
							
						}
						if((g_usart_rx_buf[34]!=0)||(g_usart_rx_buf[35]!=0)||(g_usart_rx_buf[36]!=0)||(g_usart_rx_buf[37]!=0))////第二阶段速度
						{//
							vTaskDelay(1); 
							Decreasing_Speed_2=g_usart_rx_buf[34]*16777216+g_usart_rx_buf[35]*65536+g_usart_rx_buf[36]*256+g_usart_rx_buf[37]*1; 
							Decreasing_Speed_2 = Decreasing_Speed_2*835;				
						}						
						
						
						if(Decreasing_Retention_coefficient!=0)
						{
							writingflash();
						}
						break;
					}

					
					case 0X03:// 荷重校准
					{

						
						if((g_usart_rx_buf[50]!=0)||(g_usart_rx_buf[51]!=0)||(g_usart_rx_buf[52]!=0)||(g_usart_rx_buf[53]!=0))////荷重校准1
						{
							vTaskDelay(1); 
							Calibration_load[0]=g_usart_rx_buf[50]*16777216+g_usart_rx_buf[51]*65536+g_usart_rx_buf[52]*256+g_usart_rx_buf[53]*1; //重量校准值
							Calibration_weight[0]=g_usart_rx_buf[2]*16777216+g_usart_rx_buf[3]*65536+g_usart_rx_buf[4]*256+g_usart_rx_buf[5]*1;//AD值,取绝对值，进行进入分段函数
							Calibration_mark[0]=1;	//标志Weight_calibration_old
							Tx_Buffer[0] = Weight_calibration[0];
							if(Calibration_load[0] != Weight_calibration_old[0])
							{
								  Weight_calibration_old[0] = Calibration_load[0];
								  Tx_Buffer[0] = Weight_calibration[0];
								  Weight_A[0]=1;
							}
						}
						else
						{
							Calibration_mark[0]=0;
						}
						
						if((g_usart_rx_buf[54]!=0)||(g_usart_rx_buf[55]!=0)||(g_usart_rx_buf[56]!=0)||(g_usart_rx_buf[57]!=0))////荷重校准2
						{
							vTaskDelay(1); 
							Calibration_load[1]=g_usart_rx_buf[54]*16777216+g_usart_rx_buf[55]*65536+g_usart_rx_buf[56]*256+g_usart_rx_buf[57]*1; 
							Calibration_weight[1]=g_usart_rx_buf[6]*16777216+g_usart_rx_buf[7]*65536+g_usart_rx_buf[8]*256+g_usart_rx_buf[9]*1;//AD值
							Calibration_mark[1]=1;	
							Tx_Buffer[1] = Weight_calibration[1];
							if(Calibration_load[1] != Weight_calibration_old[1])
							{
								  Weight_calibration_old[1] = Calibration_load[1];
								  Tx_Buffer[1] = Weight_calibration[1];
								  Weight_A[1]=1;
							}
							
						}
						else
						{
							Calibration_mark[1]=0;
						}
						
						if((g_usart_rx_buf[58]!=0)||(g_usart_rx_buf[59]!=0)||(g_usart_rx_buf[60]!=0)||(g_usart_rx_buf[61]!=0))////荷重校准3
						{
							vTaskDelay(1); 
							Calibration_load[2]=g_usart_rx_buf[58]*16777216+g_usart_rx_buf[59]*65536+g_usart_rx_buf[60]*256+g_usart_rx_buf[61]*1; 
							Calibration_weight[2]=g_usart_rx_buf[10]*16777216+g_usart_rx_buf[11]*65536+g_usart_rx_buf[12]*256+g_usart_rx_buf[13]*1;//AD值
							Calibration_mark[2]=1;	
							Tx_Buffer[2] = Weight_calibration[2];
							if(Calibration_load[2] != Weight_calibration_old[2])
							{
								  Weight_calibration_old[2] = Calibration_load[2];
								  Tx_Buffer[2] = Weight_calibration[2];
									Weight_A[2]=1;
							}
						}
						else
						{
							Calibration_mark[2]=0;
						}
						
						if((g_usart_rx_buf[62]!=0)||(g_usart_rx_buf[63]!=0)||(g_usart_rx_buf[64]!=0)||(g_usart_rx_buf[65]!=0))////荷重校准4
						{
							vTaskDelay(1); 
							Calibration_load[3]=g_usart_rx_buf[62]*16777216+g_usart_rx_buf[63]*65536+g_usart_rx_buf[64]*256+g_usart_rx_buf[65]*1; 
							Calibration_weight[3]=g_usart_rx_buf[14]*16777216+g_usart_rx_buf[15]*65536+g_usart_rx_buf[16]*256+g_usart_rx_buf[17]*1;//AD值
							Calibration_mark[3]=1;	
							Tx_Buffer[3] = Weight_calibration[3];
							if(Calibration_load[3] != Weight_calibration_old[3])
							{
								  Weight_calibration_old[3] = Calibration_load[3];
								  Tx_Buffer[3] = Weight_calibration[3];
								  Weight_A[3]=1;
							}
							
						}
						else
						{
							Calibration_mark[3]=0;
						}
					
						
						if((Tx_Buffer[0]!=0)||(Tx_Buffer[1]!=0))
						{
							writingflash();
						}
						
						break;
					}
								

					case 0X04:    //位移速度校准，之前有位移系数定义过，需要改，但还没改，接收到的*10000,数据需要处理/10000
					{
						if((g_usart_rx_buf[2]!=0)||(g_usart_rx_buf[3]!=0)||(g_usart_rx_buf[4]!=0)||(g_usart_rx_buf[5]!=0))////位移系数
						{
							vTaskDelay(2); 
							coefficient_Distance = g_usart_rx_buf[2]*16777216+g_usart_rx_buf[3]*65536+g_usart_rx_buf[4]*256+g_usart_rx_buf[5]*1; 
							displacement_speed[0] = coefficient_Distance/10000;
							Distance_value=Distance_value/displacement_speed[0];
							Tx_Buffer[8] = Distance_value;//Distance_value
						  Distance_coefficient = 1;       //位移标志
						}
						else
						{			
						  	Tx_Buffer[8] = Distance_value;
							  Distance_coefficient = 1;       //位移标志
						}
						
						if((g_usart_rx_buf[6]!=0)||(g_usart_rx_buf[7]!=0)||(g_usart_rx_buf[8]!=0)||(g_usart_rx_buf[9]!=0))////速度系数
						{
							vTaskDelay(2); 
							coefficient_speed = g_usart_rx_buf[6]*16777216+g_usart_rx_buf[7]*65536+g_usart_rx_buf[8]*256+g_usart_rx_buf[9]*1; 
							displacement_speed[1] = coefficient_speed/10000;																	
						}
						
						if(Distance_coefficient == 1) //写的有点繁琐，可以缩减，之后优化在缩减
						{
							Tx_Buffer[8] = Distance_value;
							writingflash();
							Distance_coefficient = 0;
							speed_coefficient = 0;
						}
						
						
						break;
					}						
								
			
				}

//					printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X\r\n",g_usart_rx_buf[0],g_usart_rx_buf[1],g_usart_rx_buf[2],g_usart_rx_buf[3],g_usart_rx_buf[4],g_usart_rx_buf[5],g_usart_rx_buf[6]);			
//					
//				vTaskDelay(1); 
//				memset(g_usart_rx_buf, 0, 100);//清空缓存区
			}

 g_usart_rx_sta = 0;

//			  memset(g_rx_buffer, 0, 100);//清空缓存区
	
			
	
					
}















