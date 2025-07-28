
#include "includes.h"



SpeedIndexDef	SpeedMotor[4];
int32_t TempStepSpeed[4];//加速步距



/*
	初始化电机参数
*/
void	SpeedMotorInit(uint8_t MotorChannel)
{
	SpeedMotor[MotorChannel].FreoutMax=200000;
	SpeedMotor[MotorChannel].SpeedMax=5000000;
	SpeedMotor[MotorChannel].SpeedMin=400;  //最小值原来为5000 //分频83，对应最小为400
	SpeedMotor[MotorChannel].UpStartSpeed=50000;
	SpeedMotor[MotorChannel].CurrentSpeed=1000000;//1kk 5s 10mm 60s 120mm
	SpeedMotor[MotorChannel].PreSpeed = 3000000;
	SpeedMotor[MotorChannel].Order = 0;
	SpeedMotor[MotorChannel].Test_State = 0;	
	SpeedMotor[MotorChannel].SpeedReturn=3000000;
	SpeedMotor[MotorChannel].SpeedUpTime=5000;
	SpeedMotor[MotorChannel].SpeedDownTime=5000;
	SpeedMotor[MotorChannel].SpeedMode=0;
}
/*
	设置速度函数
	MotorChannel 	电机号		1、2、3、4
	SpeedV0  速度
	无返回
*/
void	SetSpeed(uint8_t MotorChannel,uint32_t	SpeedV0)
{
	float tempSpeedV0;

	if(SpeedV0>SpeedMotor[MOTORNUM1].SpeedMax)SpeedV0 = SpeedMotor[MOTORNUM1].SpeedMax;
	else if(SpeedV0<SpeedMotor[MOTORNUM1].SpeedMin)SpeedV0 = SpeedMotor[MOTORNUM1].SpeedMin;
	tempSpeedV0=(float)SpeedV0/SpeedMotor[MOTORNUM1].SpeedMax*SpeedMotor[MOTORNUM1].FreoutMax;//1S的脉冲数量
	if(tempSpeedV0<16)
		tempSpeedV0=16;
	tempSpeedV0=(uint32_t)(MOTORCLK/tempSpeedV0/2);//实际比较值计算  越大速度越小

		/*
				2000khz  100  	MOTORCLK 2000  MOTORCLK/tempSpeedV0    21000000 /200000/2=
		*/
	switch(MotorChannel)
	{
		case MOTORNUM1:
			if(tempSpeedV0!=SpeedMotor[MOTORNUM1].SpeedSetIndex[1])//||SpeedMotor[MOTORNUM1].MotorState[1]==MOTOR_STOP)//SpeedSetIndex
			{
				SpeedMotor[MOTORNUM1].SpeedSetIndex[0]=SpeedMotor[MOTORNUM1].SpeedSetIndex[1];
				SpeedMotor[MOTORNUM1].SpeedSetIndex[1]=tempSpeedV0;
				SpeedUpDowncal(MOTORNUM1,tempSpeedV0,tempSpeedV0,&SpeedMotor[MOTORNUM1].UpDownTimesIndex);
				__HAL_TIM_SET_AUTORELOAD(&hAdvtimy,SpeedMotor[MOTORNUM1].SpeedCompareIndex[0]);
				__HAL_TIM_SET_COMPARE(&hAdvtimy,TIM_CHANNEL_1,SpeedMotor[MOTORNUM1].SpeedCompareIndex[0]);

			}
			break;

	}
}

void	SpeedUpDowncal(uint8_t Motornum,int32_t StartSpeed,int32_t TargetSpeed,uint8_t	*UpDownTimesIndex)
{
	int8_t i;
		TempStepSpeed[Motornum]=(TargetSpeed-StartSpeed)/(UPDOWNSPEEDTIMES-3);//UPDOWNSPEEDTIMES暂定100次加速至最大速度
	for(i=0;i<UPDOWNSPEEDTIMES;i++)
	{
			SpeedMotor[Motornum].SpeedCompareIndex[i]=StartSpeed;
			if(StartSpeed>TargetSpeed)
			{
				StartSpeed+=TempStepSpeed[Motornum];
				if(StartSpeed<=TargetSpeed)
				{
					SpeedMotor[Motornum].SpeedCompareIndex[i+1]=TargetSpeed;
					UpDownTimesIndex[0]=i;
					break;
				}
			}
			else if(StartSpeed<TargetSpeed)
			{
				StartSpeed+=TempStepSpeed[Motornum];
				if(StartSpeed>=TargetSpeed)
				{
					SpeedMotor[Motornum].SpeedCompareIndex[i+1]=TargetSpeed;
					UpDownTimesIndex[0]=i;
					break;
				}
			}
			else if(StartSpeed==TargetSpeed)
			{
				
			}
	}
}


/*
	机台动作操作函数
	MotorNum 	电机号		1、2、3、4
	State			状态		0、1、2  MOTOR_UP上升 MOTOR_DOWN下降 MOTOR_STOP停止
	无返回
*/
void	MotorAction(uint8_t MotorNum,uint8_t	State)
{
	switch(State)
	{
		case MOTOR_UP:
				//if(SpeedMotor[MotorNum].MotorState[1]==MOTOR_STOP)
				//{
					SpeedMotor[MotorNum].MotorState[0]=SpeedMotor[MotorNum].MotorState[1];
					SpeedMotor[MotorNum].MotorState[1]=MOTOR_UP;
					DIR1_CCW();
					Motor_OC_IT(MotorNum,ON,CCW);
				//}
			break;
		case MOTOR_DOWN:
				//if(SpeedMotor[MotorNum].MotorState[1]==MOTOR_STOP)
				//{
					SpeedMotor[MotorNum].MotorState[0]=SpeedMotor[MotorNum].MotorState[1];
					SpeedMotor[MotorNum].MotorState[1]=MOTOR_DOWN;
					DIR1_CW();
					Motor_OC_IT(MotorNum,ON,CW);
				//}
			break;
		case MOTOR_STOP:
				if(SpeedMotor[MotorNum].MotorState[1]!=MOTOR_STOP)
				{
					SetSpeed(MotorNum,0);
					SpeedMotor[MotorNum].StopFlag=1;
					Strokedata.ReturnStage[MotorNum]=0;
				}
//				Motor_OC_IT(MotorNum,OFF,CW);
				/*
				真正停止放在中断里面
				*/
			break;
		case MOTOR_STOPREAL:
				if(SpeedMotor[MotorNum].MotorState[1]!=MOTOR_STOP)
				{
					Motor_OC_IT(MOTORNUM1,OFF,CW);
					SpeedMotor[MOTORNUM1].StopFlag=0;
					SpeedMotor[MOTORNUM1].MotorState[0]=SpeedMotor[MOTORNUM1].MotorState[1];
					SpeedMotor[MOTORNUM1].MotorState[1]=MOTOR_STOP;
				}
//				Motor_OC_IT(MotorNum,OFF,CW);
				/*
				真正停止放在中断里面
				*/
			break;
	}	
}

/*
	脉冲发送启动停止操作
	MotorNum 	电机号		1、2、3、4
	State			状态		0、1  0关闭 1开启
	Dir				方向		1 CCW 0 CW
	无返回
*/
void Motor_OC_IT(uint8_t MotorNum,uint8_t	State,uint8_t Dir)
{
	switch(MotorNum)
	{
		case	MOTORNUM1:
			if(!State)
			{
				
				MOTOR1OFF();
				MOTOR1CLEAR();//清零计数值
			}
			else
			{
					MOTOR1ON();
				
			}
			if(!Dir)
				DIR1_CW();
			else
				DIR1_CCW();		
			break;

	}
}




/**
  * 函数功能: 比较中断回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 更改比较值
  */
uint32_t	CompareTimes[100],CompareTimesCount;
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  __IO uint32_t count = 0;
  __IO uint32_t tmp = 0;
  /* 修改定时器通道下一次中断的时间 */
  count = __HAL_TIM_GetCounter(htim);
  if(htim == &hAdvtimy) // 高级定时器x
  {
    switch(htim->Channel)
    {
      case HAL_TIM_ACTIVE_CHANNEL_1:
				
//        tmp = htim->Init.Period & (count + SpeedMotor[MOTORNUM1].SpeedCompareIndex[SpeedMotor[MOTORNUM1].CurrentSpeedCompare]);// 修改定时器CH1的比较值
				if(SpeedMotor[MOTORNUM1].CurrentSpeedCompare<SpeedMotor[MOTORNUM1].UpDownTimesIndex+1)
				{
					__HAL_TIM_SET_AUTORELOAD(htim,SpeedMotor[MOTORNUM1].SpeedCompareIndex[SpeedMotor[MOTORNUM1].CurrentSpeedCompare]);
					__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,SpeedMotor[MOTORNUM1].SpeedCompareIndex[SpeedMotor[MOTORNUM1].CurrentSpeedCompare]);
////					if(SpeedMotor[MOTORNUM1].FlashNewCompareFlag||SpeedMotor[MOTORNUM1].SpeedMode)
					{
						SpeedMotor[MOTORNUM1].FlashNewCompareFlag=0;
						SpeedMotor[MOTORNUM1].CurrentSpeedCompare++;
					}
				}
				else if(SpeedMotor[MOTORNUM1].StopFlag)
				{
					MotorAction(0,MOTOR_STOPREAL);
					
				}
			
        break;
      default:
        break;
    }
  }

}











//  * 函数功能: 基本定时器硬件初始化配置
//  * 输入参数: htim_base：基本定时器句柄类型指针
//  * 返 回 值: 无
//  * 说    明: 该函数被HAL库内部调用

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
//{

//		if(htim_base->Instance==STEPMOTOR_TIMx)
//		{
//			/* 基本定时器外设时钟使能 */
//			STEPMOTOR_TIM_RCC_CLK_ENABLE();
//		}
		
//}


///////////////////////////////////////////////////////////////////////////

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
//{

//  if(htim_base->Instance==STEPMOTOR_TIMx)
//  {
//    /* 基本定时器外设时钟禁用 */
//    STEPMOTOR_TIM_RCC_CLK_DISABLE();
//    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL_PORT,STEPMOTOR_TIM_PUL_PIN);
//    HAL_GPIO_DeInit(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN);
//    HAL_GPIO_DeInit(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN);
//    
//    HAL_NVIC_DisableIRQ(STEPMOTOR_TIMx_IRQn);
//  }
//	

//	if(htim_base->Instance==TIM7)
//	{
//		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM3时钟
//		HAL_NVIC_SetPriority(TIM7_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级2
////		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM3中断  
//		HAL_NVIC_DisableIRQ(TIM7_IRQn);			
//	}	

////	if(htim_base->Instance==TIM4)
////	{
////		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM3时钟
////		HAL_NVIC_SetPriority(TIM4_IRQn,2,3);    //设置中断优先级，抢占优先级1，子优先级2
//////		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断  
////		HAL_NVIC_DisableIRQ(TIM4_IRQn);			
////	}	
//	
//} 






































































