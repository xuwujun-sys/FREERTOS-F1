

#include "includes.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
////////////////////////////////////////////////////////////////串口DMA发送，除非使用DMA发送和DMA接收一起，否则会有BUG，单独使用发送，时间最低为40ms，单独接收会与通用发送有一定冲突，导致突然卡死
////////////////////////////////////////////////////////////////暂定不用此方法
#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) ) * 1    /* 发送数据长度, 等于sizeof(TEXT_TO_SEND) + 2的200倍. */
uint8_t TEXT_TO_SEND[20];

__IO int32_t CaptureNumber = 0;     // 输入捕获数
__IO int32_t LastCapNum = 0;     // 上一次输入捕获数
__IO int32_t Speed = 0;     // 上一次输入捕获数


extern __IO float position1 ; 


extern int Test_Mode1;


extern float displacement_Zero;//位移清零，原有冒充有的，这些只是在这个位置，记录当前计数与位移，计算更新之后与之相减，

 void FloatToByte(float floatNum,unsigned char* byteArry);
 
int status_flag=0;
int ABR=0;
int DDD=0;
int AAA=999;
int BBB=0;
int CCC=0;
extern uint8_t Test_change_flag;
float Test_displacemen=0;
int32_t displacemen_1=0;
int BRB_A=0;
int keyfag=0;
int keybug=0;
int BRB_B=0;
int safety_fag=0;
int safety_bug=0;

float Test_displacemen1=0;

int ceshi=0;


/******************************************************************************************************/
/*FreeRTOS配置*/
volatile int case_counter = 0;     // 已完成的 case 计数器
SemaphoreHandle_t xMutex = NULL;   // 互斥信号量保护 AA
QueueHandle_t xQueue = NULL;      // 任务间通信队列


////////////////////////////////////////////////////////////////////////////////////////////
//////////////////滤波

#define N_B 200
float value_buff_B[N_B+1]={0};
int i_B=0;
float filter_B()
{
	int count;
	float sum=0;
	value_buff_B[i_B]=weight;
	for(count=0;count < N_B;count++)
	{
		value_buff_B[count]=value_buff_B[count+1];
		sum += value_buff_B[count];
	}
	return (float)(sum/N_B);
}

////////////////////////////////////////////////////////////////////////////////////////////
void Testtimes_FloatToByte(float floatNum,unsigned char* byteArry)
{
    char* pchar=(char*)&floatNum;
    int i = 0;
    for( i=0;i<sizeof(float);i++)
    {
			*byteArry=*pchar;
			pchar++;
			byteArry++;
 
    }
}



void weight_FloatToByte(float floatNum,unsigned char* byteArry)
{
    char* pchar=(char*)&floatNum;
    int i = 0;
    for( i=0;i<sizeof(float);i++)
    {
			*byteArry=*pchar;
			pchar++;
			byteArry++;
 
    }
}

void AD_FloatToByte(float floatNum,unsigned char* byteArry)
{
    char* pchar=(char*)&floatNum;
    int i = 0;
    for( i=0;i<sizeof(float);i++)
    {
			*byteArry=*pchar;
			pchar++;
			byteArry++;
 
    }
}

void displacement_FloatToByte(float floatNum,unsigned char* byteArry)
{
    char* pchar=(char*)&floatNum;
    int i = 0;
    for( i=0;i<sizeof(float);i++)
    {
			*byteArry=*pchar;
			pchar++;
			byteArry++;
 
    }
}


int DectoHex(int dec, unsigned char *hex, int length) 
{ 
  for(int i=length-1; i>=0; i--) 
  { 
    hex[i] = (dec%256)&0xFF; 
    dec /= 256; 
  } 

  return 0; 
}



/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      3                   /* 任务优先级 */
#define TASK1_STK_SIZE  512                 /* 任务堆栈大小512 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      6                  /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小512 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      5                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
void task3(void *pvParameters);             /* 任务函数 */

/* TASK4 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK4_PRIO      4                  /* 任务优先级 */
#define TASK4_STK_SIZE  512                /* 任务堆栈大小 */
TaskHandle_t            Task4Task_Handler;  /* 任务句柄 */
void task4(void *pvParameters);             /* 任务函数 */

/* TASK5 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK5_PRIO      2                  /* 任务优先级 */
#define TASK5_STK_SIZE  128                /* 任务堆栈大小 */
TaskHandle_t            Task5Task_Handler;  /* 任务句柄 */
void task5(void *pvParameters);             /* 任务函数 */

/* TASK6 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK6_PRIO      7                  /* 任务优先级 */
#define TASK6_STK_SIZE  128                /* 任务堆栈大小 */
TaskHandle_t            Task6Task_Handler;  /* 任务句柄 */
void task6(void *pvParameters);             /* 任务函数 */

#define TASK7_PRIO      8                  /* 任务优先级 */
#define TASK7_STK_SIZE  128                /* 任务堆栈大小 */
TaskHandle_t            Task7Task_Handler;  /* 任务句柄 */
void task7(void *pvParameters);             /* 任务函数 */

#define TASK8_PRIO      9                  /* 任务优先级 */
#define TASK8_STK_SIZE  128                /* 任务堆栈大小 */
TaskHandle_t            Task8Task_Handler;  /* 任务句柄 */
void task8(void *pvParameters);             /* 任务函数 */


QueueHandle_t           xQueue;             /* 定义队列 */
#define QUEUE_LENGTH    1                   /* 队列支持的消息个数 */
#define QUEUE_ITEM_SIZE sizeof(uint8_t)     /* 队列中每条消息的大小 */



SemaphoreHandle_t MutexSemaphore;           /* 互斥信号量  */

SemaphoreHandle_t BinarySemaphore;          /* 二值信号量句柄 */


/******************************************************************************************************/


/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{  
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建队列 */
    xQueue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);	
    /* 创建互斥信号量 */
    MutexSemaphore = xSemaphoreCreateMutex();
    /* 创建二值信号量 */
    BinarySemaphore = xSemaphoreCreateBinary();
	
    /* 创建任务1,DMA发 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 ，按键*/
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
		/* 创建任务3 ，串口收*/						
    xTaskCreate((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务4 ，电机*/
    xTaskCreate((TaskFunction_t )task4,
                (const char*    )"task4",
                (uint16_t       )TASK4_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK4_PRIO,
                (TaskHandle_t*  )&Task4Task_Handler);								
    /* 创建任务5 ，AD采集*/
    xTaskCreate((TaskFunction_t )task5,
                (const char*    )"task5",
                (uint16_t       )TASK5_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK5_PRIO,
                (TaskHandle_t*  )&Task5Task_Handler);			
    /* 创建任务6，回位和点动上下 */
    xTaskCreate((TaskFunction_t )task6,
                (const char*    )"task6",
                (uint16_t       )TASK6_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK6_PRIO,
                (TaskHandle_t*  )&Task6Task_Handler);
/* 创建任务7 *///找零点
    xTaskCreate((TaskFunction_t )task7,
                (const char*    )"task7",
                (uint16_t       )TASK7_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK7_PRIO,
                (TaskHandle_t*  )&Task7Task_Handler);
/* 创建任务8 *///量程限位和位移限位
    xTaskCreate((TaskFunction_t )task8,
                (const char*    )"task8",
                (uint16_t       )TASK8_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK8_PRIO,
                (TaskHandle_t*  )&Task8Task_Handler);								
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task1(void *pvParameters)
{
	
    while(1)
    {		
			if(g_usart_rx_buf[0]==0XAA)
			{
				switch(g_usart_rx_buf[1])
				{
					case 0X00:            //手动控制
					{
//						LED2_TOGGLE();
					
						switch(g_usart_rx_buf[2])
						{
							case 0X0C:         //启动测试
							{
								if(g_usart_rx_buf[3]==0X00)
								{
									if((Pulling_force==1)&&(Compressive_force==0)&&(two_way==0))//BRB
									{
										BRB_A=1;ABR=0;BRB_B=0;
										DDD=999;
										safety_fag=1;
										/* 向消息队列发数据，如果消息队列满了，等待10个时钟节拍 */
										if( xQueueSend(xQueue,(void *) &BRB_A,(TickType_t)10) != pdPASS )
										{
											/* 发送失败，即使等待了10个时钟节拍 */
											printf("mistake\n");//发送失败，发送错误
										}
										else
										{
											/* 发送成功 */
											printf("success\n");//发送成功，发送正确						
										} 									
									}
								}
							if(g_usart_rx_buf[3]==0X00)
							{
								if((Pulling_force==0)&&(Compressive_force==1)&&(two_way==0))//BUTTON
								{
									CCC=1;AAA=0;
									BRB_A=999;
									DDD=999;
									safety_bug=1;
									/* 向消息队列发数据，如果消息队列满了，等待10个时钟节拍 */
									if( xQueueSend(xQueue,(void *) &CCC,(TickType_t)10) != pdPASS )
									{
										/* 发送失败，即使等待了10个时钟节拍 */
										printf("mistake\n");//发送失败，发送错误
									}
									else
									{
										/* 发送成功 */
										printf("success\n");//发送成功，发送正确						
									} 										
								}
							}
								if(g_usart_rx_buf[3]==0XAA)
								{
									DDD=1;
									BRB_A=999;
									/* 向消息队列发数据，如果消息队列满了，等待10个时钟节拍 */
									if( xQueueSend(xQueue,(void *) &DDD,(TickType_t)10) != pdPASS )
									{
										/* 发送失败，即使等待了10个时钟节拍 */
										printf("mistake\n");//发送失败，发送错误
									}
									else
									{
										/* 发送成功 */
										printf("success\n");//发送成功，发送正确						
									}
								}
								break;
							}

						}
						break;
					}
					 g_usart_rx_sta = 0;
				}
			}
		Serial_message_processing();
		

		memset(g_usart_rx_buf, 0, sizeof(g_usart_rx_buf));//清空
			
			
		printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",g_sendbuf[0],g_sendbuf[1],g_sendbuf[2],g_sendbuf[3],g_sendbuf[4],g_sendbuf[5],g_sendbuf[6],g_sendbuf[7],g_sendbuf[8],g_sendbuf[9],g_sendbuf[10],g_sendbuf[11],g_sendbuf[12],g_sendbuf[13],g_sendbuf[14],g_sendbuf[15],g_sendbuf[16],g_sendbuf[17]);
		printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",g_sendbuf[6],g_sendbuf[7],g_sendbuf[8],g_sendbuf[9],g_sendbuf[10],g_sendbuf[11],g_sendbuf[12],g_sendbuf[13],g_sendbuf[14],g_sendbuf[15],g_sendbuf[16],g_sendbuf[17]);
		printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",g_sendbuf[6],g_sendbuf[7],g_sendbuf[8],g_sendbuf[9],g_sendbuf[10],g_sendbuf[11],g_sendbuf[12],g_sendbuf[13],g_sendbuf[14],g_sendbuf[15],g_sendbuf[16],g_sendbuf[17]);
		printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X",g_sendbuf[6],g_sendbuf[7],g_sendbuf[8],g_sendbuf[9],g_sendbuf[10],g_sendbuf[11],g_sendbuf[12],g_sendbuf[13],g_sendbuf[14],g_sendbuf[15],g_sendbuf[16],g_sendbuf[17]);
		printf("%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X\n",g_sendbuf[6],g_sendbuf[7],g_sendbuf[8],g_sendbuf[9],g_sendbuf[10],g_sendbuf[11],g_sendbuf[12],g_sendbuf[13],g_sendbuf[14],g_sendbuf[15],g_sendbuf[16],g_sendbuf[17],g_sendbuf[18],g_sendbuf[19]);


//    printf("  重量%.3f  AA%.3f  BB%.3f\n",weight,weight_A,weight_B);

        vTaskDelay(1);                                               /* 延时1000ticks */
    }
}


/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
////////////////相关定义见其他
void task2(void *pvParameters)
{
	 uint8_t key = 0;
	int LEDBUG=0;

    while(1)
    {
		 key = key_scan(0);                  /* 得到键值 */
			LEDBUG++;
      if (key)
        {
            switch (key)
            {
                case KEY0_PRES:            //上升
									{							
											current_displacement = 0.100f + displacement1;	
											move_flag = 4;									
											LED0(1);
											break;
									}

                case KEY1_PRES:             /* 下降 */
								{
											current_displacement =displacement1 - 0.100f;
											move_flag = 5;

									LED0(0);
                    break;
								}

                case KEY2_PRES:             /* 同时控制LED0, LED1翻转 */
                 {
												current_displacement = 0.010f + displacement1;
												move_flag = 6;			
											LED1(1);
											break;
									}
								
								case KEY3_PRES:             /* 控制LED0(RED)翻转 */
									{
											current_displacement = displacement1 - 0.010f ;
											move_flag = 7;			
										LED2(0);
											break;
									}


								
                default : break;
            }    
        }
				if(LEDBUG==5)
				{
					LED0_TOGGLE();
					LEDBUG=0;
				}			
        vTaskDelay(200);                           /* 延时1000ticks */
				
    }

}



/**
 * @brief       task3
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task3(void *pvParameters)
{
//	int BRB_B=0;
	int E=0;
    while(1)
    {			

			
if(BRB_A==1)
{
			switch(BRB_B)
				{
					case 0:
					{
						if((weight < Decreasing_Retention_coefficient)&&((fabs(Test_displacemen))<Test_distance_range)) //Test_force_range/5
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //62500，对应速度是75mm每分钟，电机驱动器是10000，走1mm是2000脉冲
							MotorAction(MOTORNUM1,MOTOR_DOWN);//
							status_flag=2;
//							BRB_B = 1;ABR=1;
						}
						else
						{
							status_flag=2;
							BRB_B = 1;ABR=1;
						}
						break;
					}
//					case 1:
//					{
//						if((weight >= Decreasing_Retention_coefficient)&&((fabs(Test_displacemen))<Test_distance_range))   //Decreasing_Retention_coefficient 减速重量  //18000
//						{
//							SpeedMotorInit(MOTORNUM1);
//							SetSpeed(MOTORNUM1,Decreasing_Speed);//30000
//							MotorAction(MOTORNUM1,MOTOR_DOWN);
//							status_flag=2;
//							BRB_B = 2;ABR=2;
//						}
//						else
//						{
//							status_flag=2;
//							BRB_B = 4;ABR=4;
//						}							
////						else     //Decreasing_Retention_coefficient      //18000
////						{
////							status_flag=2;
////							BRB_B = 2;ABR=2;
////						}
//						break;
//					}
//					case 2:
//					{
//						if((weight >= Decreasing_coefficient_weight2)&&((fabs(Test_displacemen))<Test_distance_range))
//						{
//							SpeedMotorInit(MOTORNUM1);
//							SetSpeed(MOTORNUM1,Decreasing_Speed_2);//30000
//							MotorAction(MOTORNUM1,MOTOR_DOWN);
//							status_flag=2;
//							BRB_B = 3;ABR=3;
//						}
//						else
//						{
//							status_flag=2;
//							BRB_B = 5;ABR=5;
//						}
//						break;
//					}
					case 1:
					{
						SetSpeed(MOTORNUM1,(test_speed - Decreasing_Speed * weight));  //原来是60400、、、原来是50 * weight
						status_flag=2;
//						if((weight > Test_force_range*0.9f)&&(weight < Retention_force))
//						{
//							SetSpeed(MOTORNUM1,(Decreasing_Speed - weight));  //原来是60400、、、原来是50 * weight
//							status_flag=2;
//						}
						if(weight > Retention_force)               //进入保持 Retention_force   19950
						{
							HAL_NVIC_EnableIRQ(TIM7_IRQn);	// HAL_NVIC_EnableIRQ   HAL_NVIC_DisableIRQ
							BRB_B = 2;ABR=2;
						}
						if((fabs(Test_displacemen)) >= Test_distance_range)
						{
							status_flag=2;
							BRB_B = 3;ABR=3;
						}
						break;
					}
					case 2:
					{
						if(Test_mark==1)
						{
							HAL_NVIC_DisableIRQ(TIM7_IRQn);	// HAL_NVIC_EnableIRQ   HAL_NVIC_DisableIRQ
							BRB_B = 3;ABR=3;
							time_count_1s=0;
							time_count_100ms=0;
							Test_mark=0;
						}
						if((fabs(Test_displacemen)) >= Test_distance_range)
								{
									HAL_NVIC_DisableIRQ(TIM7_IRQn);	// HAL_NVIC_EnableIRQ   HAL_NVIC_DisableIRQ
									BRB_B = 3;ABR=3;
									time_count_1s=0;
									time_count_100ms=0;
									Test_mark=0;
								}
//						if(Test_mark_1==1)
//						{				
							if(weight > upper_limit_force)
								{
										status_flag=2;
										SetSpeed(MOTORNUM1,Weight_Speed_coefficient*400);
										MotorAction(MOTORNUM1,MOTOR_UP); //MOTOR_DOWN  
								}
							if(weight < lower_limit_force)   //下限  lower_limit_force  19980
								{
									
										status_flag=2;
										SetSpeed(MOTORNUM1,Weight_Speed_coefficient*400);
										MotorAction(MOTORNUM1,MOTOR_DOWN);
								}
							else
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);
									time_count_100ms=0;
								}
//						}
//						if(Test_mark_1==2)
//							{
//								MotorAction(MOTORNUM1,MOTOR_STOP);
//								time_count_100ms=0;
//							}
					
						break;
					}					
					case 3://停止
					{
						if((Test_displacemen) < (-0.050f))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);	
							status_flag=3;		
						}	
						else
						{
							BRB_B=4;ABR=4;
							status_flag=3;
						}
						break;
					}	
					case 4:
					{
						if(((Test_displacemen) >= (-0.02f))&&((Test_displacemen) < (-0.005f)))//执行速度与位移关联
						{
							status_flag=3;
							SetSpeed(MOTORNUM1,20000);
						}
						else
						{
							BRB_B=5;	ABR=5;	
						}							
						break;
					}	
					case 5:
					{
						
						if((Test_displacemen) > (-0.005f))//到达指定力和位移停止并返回
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(BRB_time);
							Testtimes++;
							status_flag=0;
							safety_fag=0;
							safety_bug=0;
							BRB_B=6;		ABR=6;
							BRB_A=2;							
								
						}	
																	
						break;
					}	

					default:
            {
                break;
            }		
					
				}
			}

		if(DDD==1)
		{
					switch(E)
						{
						case 0:
							{
								if(weight <= 2.00f)
								{
									SpeedMotorInit(MOTORNUM1);          // 初始化电机
									SetSpeed(MOTORNUM1, 1000);        // 设置初始速度
									MotorAction(MOTORNUM1, MOTOR_DOWN); // 电机下行							
									E=1;
								}
								break;
							}
							case 1:
							{
								if( weight <= (Zero_weight-1.00f))
								{
									SetSpeed(MOTORNUM1,500);
									E=2;
								}
								break;
							}
							case 2:
							{
								if( weight >= (Zero_weight-1.00f))
								{
									MotorAction(MOTORNUM1,MOTOR_STOP);
								  vTaskDelay(100);
								  ClearTimer();
								  vTaskDelay(100);
									E=3;
								}
								break;
							}
							case 3:
							{
								if( weight >= 1.000f)
								{
									SpeedMotorInit(MOTORNUM1);
									SetSpeed(MOTORNUM1,2000);
									MotorAction(MOTORNUM1,MOTOR_UP);
									E=4;
								}
								
									
									break;
							}
							case 4:
							{
								SetSpeed(MOTORNUM1,1000);									
								if(fabs(Test_displacemen) >= (Zero_up_deceleration-0.003f))
								{
									MotorAction(MOTORNUM1, MOTOR_STOP);
									E=5;
								}
									break;
							}
							case 5:
							{
									MotorAction(MOTORNUM1, MOTOR_STOP);
									vTaskDelay(100);
								  ClearTimer();
								  vTaskDelay(100);
									DDD=99;
									E=6;
									break;
							}
						}
		}  
/////////////////////////////////////////////////////////////////////////////////////////			
//				if(((BRB_B==7)&&(BRB_A==2))||(BRB_A==999))
//				{
//					BRB_B=0;BRB_A=2;
//				}
				if((E==6)&&(DDD==99))
				{
					E=0;DDD=99;
				}	
		
        vTaskDelay(1);                           /* 延时1000ticks */ 
    }
}




/**
 * @brief       task4
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */


void task4(void *pvParameters)
{
	volatile int BD=0;
	volatile int A_992=0;
	
    while(1)
    {
			if(((times_fug==99)&&(times_bug==99))||(times_bug==99))
			{
				A_992=0;BD=0;CCC=2;times_fug=999;times_bug=10;ceshi=2;
			}
			if(CCC==1)
			{
				switch(A_992)
				{
					case 0:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(BD==0))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							BD=1;ceshi=1;
							A_992 = 1;
						}
						break;
					}
					case 1:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =2;
						}
						break;
					}
					case 2:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 3;
						}
						break;
					}
					case 3:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 4;
						}
						break;
					}
					case 4:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=5;
						}
						break;
					}
					case 5:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=6;
						}
						break;
					}
					case 6:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=7;
						}	
													
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-2				
				case 7:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==7))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 8;
						}
						break;
					}
					case 8:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =9;
						}
						break;
					}
					case 9:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 10;
						}
						break;
					}
					case 10:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 11;
						}
						break;
					}
					case 11: 
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=12;
						}
						break;
					}
					case 12:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=13;
						}
						break;
					}
					case 13:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=14;
						}	
													
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-3				
				case 14:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==14))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 15;
						}
						break;
					}
					case 15:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =16;
						}
						break;
					}
					case 16:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 17;
						}
						break;
					}
					case 17:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 18;
						}
						break;
					}
					case 18:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=19;
						}
						break;
					}
					case 19:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=20;
						}
						break;
					}
					case 20:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=21;
						}	
													
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-4					
				case 21:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==21))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 22;
						}
						break;
					}
					case 22:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =23;
						}
						break;
					}
					case 23:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 24;
						}
						break;
					}
					case 24:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 25;
						}
						break;
					}
					case 25:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=26;
						}
						break;
					}
					case 26:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=27;
						}
						break;
					}
					case 27:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=28;
						}											
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-5					
				case 28:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==28))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 29;
						}
						break;
					}
					case 29:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 = 30;
						}
						break;
					}
					case 30:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 31;
						}
						break;
					}
					case 31:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 32;
						}
						break;
					}
					case 32:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=33;
						}
						break;
					}
					case 33:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=34;
						}
						break;
					}
					case 34:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=35;
						}	
													
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-6					
				case 35:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==35))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 36;
						}
						break;
					}
					case 36:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =37;
						}
						break;
					}
					case 37:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 38;
						}
						break;
					}
					case 38:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 39;
						}
						break;
					}
					case 39:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=40;
						}
						break;
					}
					case 40:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=41;
						}
						break;
					}
					case 41:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=42;
						}	
													
						break;
					}
////////////////////////////////////////////////////////////////////////////1-9-7					
				case 42:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==42))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 43;
						}
						break;
					}
					case 43:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =44;
						}
						break;
					}
					case 44:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 45;
						}
						break;
					}
					case 45:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 46;
						}
						break;
					}
					case 46:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=47;
						}
						break;
					}
					case 47:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=48;
						}
						break;
					}
					case 48:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=49;
						}	
													
						break;
					}																									
////////////////////////////////////////////////////////////////////////////1-9-8					
				case 49:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==49))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 50;
						}
						break;
					}
					case 50:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =51;
						}
						break;
					}
					case 51:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 52;
						}
						break;
					}
					case 52:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 53;
						}
						break;
					}
					case 53:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=54;
						}
						break;
					}
					case 54:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=55;
						}
						break;
					}
					case 55:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_uplift_time);
							A_992=56;
						}	
													
						break;
					}					
////////////////////////////////////////////////////////////////////////////1-9-9				
				case 56:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==56))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);  //2300000
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
//							AAA = 1;
						}
						else
						{
							status_flag=2;
							A_992 = 57;
						}
						break;
					}
					case 57:
					{
						if((fabs(Test_displacemen)) > Displacement_deceleration)//原来是是1.850，走的是2mm，后面的也是一样，现计算为 Displacement_deceleration
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);//50000
							status_flag=2;
							A_992 =58;
						}
						break;
					}
					case 58:
					{//
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(First_pushing_time);
							status_flag=2;
							A_992 = 59;
						}
						break;
					}
					case 59:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);//3300000
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 60;
						}
						break;
					}
					case 60:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)//0.050f
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);//100000
							status_flag=3;
							A_992=61;
						}
						break;
					}
					case 61:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=62;
						}
						break;
					}
					case 62:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(First_wait_time);
							A_992=63;
						}	
													
						break;
					}					
////////////////////////////////////////////////////////////////////////////2-9-1				
					case 63:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==63))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 64;
						}
						break;
					}
					case 64:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =65;
						}
						break;
					}
					case 65:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 66;
						}
						break;
					}
					case 66:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 67;
						}
						break;
					}
					case 67:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 68;
						}
						break;
					}
					case 68:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=69;
						}
						break;
					}				
					case 69:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=70;
						}	
													
						break;
					}					
////////////////////////////////////////////////////////////////////////////2-9-2				
					case 70:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==70))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 71;
						}
						break;
					}
					case 71:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =72;
						}
						break;
					}
					case 72:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 73;
						}
						break;
					}
					case 73:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 74;
						}
						break;
					}
					case 74:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 75;
						}
						break;
					}
					case 75:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=76;
						}
						break;
					}				
					case 76:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=77;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-3				
					case 77:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==77))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 78;
						}
						break;
					}
					case 78:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =79;
						}
						break;
					}
					case 79:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 80;
						}
						break;
					}
					case 80:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 81;
						}
						break;
					}
					case 81:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 82;
						}
						break;
					}
					case 82:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=83;
						}
						break;
					}				
					case 83:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=84;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-4			
					case 84:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==84))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 85;
						}
						break;
					}
					case 85:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =86;
						}
						break;
					}
					case 86:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 87;
						}
						break;
					}
					case 87:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 88;
						}
						break;
					}
					case 88:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 89;
						}
						break;
					}
					case 89:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=90;
						}
						break;
					}				
					case 90:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=91;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-5			
					case 91:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==91))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 92;
						}
						break;
					}
					case 92:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =93;
						}
						break;
					}
					case 93:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 94;
						}
						break;
					}
					case 94:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 95;
						}
						break;
					}
					case 95:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 96;
						}
						break;
					}
					case 96:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=97;
						}
						break;
					}				
					case 97:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=98;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-6				
					case 98:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==98))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 99;
						}
						break;
					}
					case 99:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =100;
						}
						break;
					}
					case 100:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 101;
						}
						break;
					}
					case 101:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 102;
						}
						break;
					}
					case 102:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 103;
						}
						break;
					}
					case 103:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=104;
						}
						break;
					}				
					case 104:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=105;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-7				
					case 105:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==105))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 106;
						}
						break;
					}
					case 106:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =107;
						}
						break;
					}
					case 107:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 108;
						}
						break;
					}
					case 108:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 109;
						}
						break;
					}
					case 109:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 110;
						}
						break;
					}
					case 110:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=111;
						}
						break;
					}				
					case 111:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=112;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-8				
					case 112:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==112))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 113;
						}
						break;
					}
					case 113:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =114;
						}
						break;
					}
					case 114:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 115;
						}
						break;
					}
					case 115:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 116;
						}
						break;
					}
					case 116:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 117;
						}
						break;
					}
					case 117:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=118;
						}
						break;
					}				
					case 118:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_uplift_time);
							A_992=119;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////2-9-9			
					case 119:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==119))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 120;
						}
						break;
					}
					case 120:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =121;
						}
						break;
					}
					case 121:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Second_pushing_time);
							status_flag=2;
							A_992 = 122;
						}
						break;
					}
					case 122:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 123;
						}
						break;
					}
					case 123:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 124;
						}
						break;
					}
					case 124:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=125;
						}
						break;
					}				
					case 125:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Second_wait_time);
							A_992=126;
						}	
													
						break;
					}	
////////////////////////////////////////////////////////////////////////////3-2-1				
					case 126:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==126))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 127;
						}
						break;
					}
					case 127:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =128;
						}
						break;
					}
					case 128:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Third_pushing_time);
							status_flag=2;
							A_992 = 129;
						}
						break;
					}
					case 129:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 130;
						}
						break;
					}
					case 130:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 131;
						}
						break;
					}
					case 131:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=132;
						}
						break;
					}				
					case 132:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							status_flag=2;
							vTaskDelay(Third_uplift_time);
							A_992=133;
						}	
													
						break;
					}	

////////////////////////////////////////////////////////////////////////////3-2-2				
					case 133:
					{
						if(((fabs(Test_displacemen))<(0.50f))&&(A_992==133))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,test_speed);
							MotorAction(MOTORNUM1,MOTOR_DOWN);
							status_flag=2;
						}
						else
						{
							status_flag=2;
							A_992 = 134;
						}
						break;
					}
					case 134:
					{
						if((fabs(Test_displacemen))>Displacement_deceleration)//Displacement_deceleration  Test_distance_range
						{
							SetSpeed(MOTORNUM1,Goto_slow_speed);
							status_flag=2;
							A_992 =135;
						}
						break;
					}
					case 135:
					{
						if(((fabs(Test_displacemen))>(Test_distance_range - Press_stop_deceleration))&&((fabs(Test_displacemen)) < (Test_distance_range -0.003f)))
						{
							SetSpeed(MOTORNUM1,(Goto_slow_speed/2));//50000
							status_flag=2;
						}
						if((fabs(Test_displacemen))>(Test_distance_range -0.003f))//0.020f
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							vTaskDelay(Third_pushing_time);
							status_flag=2;
							A_992 = 136;
						}
						break;
					}
					case 136:
					{
						if((fabs)(Test_displacemen) > (Test_distance_range/2))
						{
							SpeedMotorInit(MOTORNUM1);
							SetSpeed(MOTORNUM1,return_speed);
							MotorAction(MOTORNUM1,MOTOR_UP);
							status_flag=3;							
						}
						else
						{
							status_flag=3;
							A_992 = 137;
						}
						break;
					}
					case 137:
					{
						
						if((fabs)(Test_displacemen) <= Slow_down_deceleration)
						{
							SetSpeed(MOTORNUM1,Return_slow_speed);
							status_flag=3;
							A_992 = 138;
						}
						break;
					}
					case 138:
					{
						if(((fabs)(Test_displacemen) > Return_stop_deceleration)&&((fabs)(Test_displacemen) <= Slow_down_deceleration))//if(Test_displacemen <= (0.00f-Return_stop_deceleration))
						{
							SetSpeed(MOTORNUM1,(Return_slow_speed/5));//100000
							status_flag=3;
							A_992=139;
						}
						break;
					}				
					case 139:
					{
					 if (Test_displacemen >= (-Return_stop_deceleration))
						{
							MotorAction(MOTORNUM1,MOTOR_STOP);
							
							vTaskDelay(Third_wait_time);
							status_flag=0;
							A_992=140;BD=0;
							safety_fag=0;safety_bug=0;		
							CCC=2;Testtimes++;
						}	
													
						break;
					}						
					
					default :  break;
	
				}


		}
			if(((A_992==140)&&(CCC ==2)))
			{
				A_992=0;BD=0;CCC=2;
			}
			
			
			
        vTaskDelay(1);                           /* 延时1000ticks */
  			

    }
}







/**
 * @brief       task5
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task5(void *pvParameters)
{

		AD7190_Init();
		//AD7190参数配置
		weight_ad7190_conf();
		//延时0.5秒
	
		weight_Zero_Data=weight_ad7190_ReadAvg(3);
		filter();	 
	
    while(1)
    {			
			
			weight_count=filter();
			data_temp=(weight_count-weight_Zero_Data);

//				weight=fabs(data_temp*1000.0f/(1900));	//默认的一个系数，这个也是要保存的，下边的是保存一个数组 355 360   
			//也可以用标志判断，暂未写，可改，数组已经定义
			//分段函数计算出的重量是经过取绝对值处理的，后期可改为正负形式的
			//要保存的不是校准的重量和校准的AD值，是这两个计算之后的一个值，也就是一个12位的数组，串口接收那段需要去掉
			if((0 <fabs(data_temp))&&(fabs(data_temp) < 3))
			{
				weight=fabs(data_temp*1000.0f/(1900))*weight_coefficient;
			} 

		  else if((3 <= fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[0]+1))//第1段分段函数
			{	
				if((Calibration_mark[0]==1)&&(Weight_A[0]==1))
				{
					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
					Weight_calibration[0]=weight_data_temp/Calibration_load[0];	
					Tx_Buffer[0] = Weight_calibration[0];		
					Calibration_mark[0]=2;Weight_A[0]=2;
				}
//			  weight=fabs(data_temp*1000.0f/Weight_calibration[0]);	//
				weight=fabs(data_temp*1000.0f/Weight_calibration[0])*weight_coefficient;	//
			}
			else if((Calibration_weight[0] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[1]+1))//第2段分段函数
			{
				if((Calibration_mark[1]==1)&&(Weight_A[1]==1))
				{
					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
				  Weight_calibration[1]=weight_data_temp/Calibration_load[1];	
					Tx_Buffer[1] = Weight_calibration[1];		
          Calibration_mark[1]=2;Weight_A[1]=2;					
				}
//			  weight=fabs(data_temp*1000.0f/Weight_calibration[1]);	//
				weight=fabs(data_temp*1000.0f/Weight_calibration[1])*weight_coefficient;	//
			}
			else if((Calibration_weight[1] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[2]+1))//第3段分段函数
			{
				if((Calibration_mark[2]==1)&&(Weight_A[2]==1))
				{
					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
				  Weight_calibration[2]=weight_data_temp/Calibration_load[2];	
					Tx_Buffer[2] = Weight_calibration[2];		
					Calibration_mark[2]=2;Weight_A[2]=2;
				}					
//			  weight=fabs(data_temp*1000.0f/Weight_calibration[2]);	//
				weight=fabs(data_temp*1000.0f/Weight_calibration[2])*weight_coefficient;	//
			}
			else if((Calibration_weight[2] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[3]+1))//第4段分段函数
			{
				if((Calibration_mark[3]==1)&&(Weight_A[3]==1))
				{
					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
				  Weight_calibration[3]=weight_data_temp/Calibration_load[3];
					Tx_Buffer[3] = Weight_calibration[3];					
					Calibration_mark[3]=2;Weight_A[3]=2;
				}					
//			  weight=fabs(data_temp*1000.0f/Weight_calibration[3]);	//
				weight=fabs(data_temp*1000.0f/Weight_calibration[3])*weight_coefficient;	//
			}
			else      //计算找出最后一段校准值，超出校准以最后一个为校准系数计算
			{
				if(Weight_calibration[3] != 0)
				{
					weight=fabs(data_temp*1000.0f/Weight_calibration[3])*weight_coefficient;
				}
				else
				{
					weight=fabs(data_temp*1000.0f/Weight_calibration[0])*weight_coefficient;
				}	
			}

		
			if((Pulling_force==1)&&(Compressive_force==0)&&(two_way==0))//BRB模式
			{
				Test_displacemen=gtim_get_encode()/20000.00f;// Distance_value 位移系数
				displacement1 = Test_displacemen;//一工位需要取反。其余不要
			}
//			else 
			if((Pulling_force==0)&&(Compressive_force==1)&&(two_way==0))//992模式
			{
				Test_displacemen=gtim_get_encode()/2000.00f;// Distance_value 位移系数
				displacement1 = Test_displacemen;//一工位需要取反。其余不要
			}
//			Test_displacemen=gtim_get_encode()/Distance_value;// Distance_value 位移系数
//				displacement1 = Test_displacemen;//一工位需要取反。其余不要	
			
//			Test_displacemen1=gtim_get_encode();
			
//			displacement1 = 0.00f - Test_displacemen;//一工位需要取反。其余不要
			
			
			
			DectoHex(status_flag, State_condition_DectoHex, 1); 
			Testtimes_FloatToByte(Testtimes,Testtimes_floattohex);//转换乘IEEEE754进制（16进制的一种）测试次数转换
			AD_FloatToByte(data_temp,AD_floattohex);//转换乘IEEEE754进制（16进制的一种）AD值转换
			weight_FloatToByte(weight,weight_floattohex);//转换乘IEEEE754进制（16进制的一种） 重量转换
			displacement_FloatToByte(displacement1,displacement1_floattohex);//转换乘IEEEE754进制（16进制的一种） 位移转换
	 
	 
		
		 g_sendbuf[0]=0XAA;                             //帧头
		 g_sendbuf[1]=State_condition_DectoHex[0];      //状态
		 g_sendbuf[2]=Testtimes_floattohex[3];          //测试次数
		 g_sendbuf[3]=Testtimes_floattohex[2];          //
		 g_sendbuf[4]=Testtimes_floattohex[1];          //
		 g_sendbuf[5]=Testtimes_floattohex[0];          //
		 g_sendbuf[6]=displacement1_floattohex[3];       //行程
		 g_sendbuf[7]=displacement1_floattohex[2];       //
		 g_sendbuf[8]=displacement1_floattohex[1];       //
		 g_sendbuf[9]=displacement1_floattohex[0];       //
		 g_sendbuf[10]=weight_floattohex[3];            //重量
		 g_sendbuf[11]=weight_floattohex[2];            //
		 g_sendbuf[12]=weight_floattohex[1];            //
		 g_sendbuf[13]=weight_floattohex[0];            //
		 g_sendbuf[14]=AD_floattohex[3];                //原始AD值
		 g_sendbuf[15]=AD_floattohex[2];                //
		 g_sendbuf[16]=AD_floattohex[1];                //
		 g_sendbuf[17]=AD_floattohex[0];                //
		 g_sendbuf[18]=0X00;                            //补充
		 g_sendbuf[19]=0XDD;                            //		
		 
		 
			vTaskDelay(1);                           /* 延时1000ticks */
    }
    
}




void task6(void *pvParameters)
{

 while(1)
 {
		if(move_flag==1)
		 {
			 specifyDisplacement_up();
		 }
		if(move_flag==2)
		 {
			 specifyDisplacement_down();
		 }
		if(move_flag == 3)
		 {
			 motorReturnZero();
		 }	
		if(move_flag == 4)
		 {
			 Key1mm_up();
		 }
		if(move_flag == 5)
		 {
			 Key1mm_down();
		 }
		if(move_flag == 6)
		 {
			 Key01mm_up();
		 }
		if(move_flag == 7)
		 {
			 Key01mm_down();
		 }		 
	 vTaskDelay(1);
 }
}


void task7(void *pvParameters) //找零
{

 while(1)
 {
     
	  vTaskDelay(1000);
	}
 }



void task8(void *pvParameters)
{

 while(1)
 {

//      if(weight >= 4300.00f)
//			 {
//				 BRB_A=999;BBB=3;
//				 CCC=999;AAA=999;
//					DDD=999;
//				 MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
//				 move_flag = 0;
//			 }	
//			if(((safety_fag == 1) || (safety_bug == 1))&&((fabs)(Test_displacemen) >= (Test_distance_range+1.00f)))
//			{
//				 BRB_A=999;BBB=3;
//				 CCC=999;AAA=999;
//					DDD=999;
//				 MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
//				 move_flag = 0;
//			}	
//			if((safety_fag == 1)&&((fabs)(weight) >= (4300.00f)))
//			{
//				 BRB_A=999;BBB=3;
//				 CCC=999;AAA=999;
//					DDD=999;
//				 MotorAction(MOTORNUM1,MOTOR_STOP);//MOTOR_STOPREAL   MOTOR_STOP
//				 move_flag = 0;
//			}	
	vTaskDelay(1000);
 }
}











