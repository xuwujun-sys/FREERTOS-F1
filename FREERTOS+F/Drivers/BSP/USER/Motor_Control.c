#include "includes.h"





//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



uint8_t Test_change_flag=0;
uint32_t Test_Time_Count = 0;
float force_last=0,force_max=0;
unsigned char Last_Filter_Flag=0,pause_time_flag=0,Force_Hold_Last_Deal_Flag=0;

extern unsigned char MACHINE_TEST_FLAG;
extern uint32_t time_count_1s;

extern float Displacement,Force;
extern float Pause_Time,Hold_Time;
extern unsigned char Loop_Flag,Return_Flag;
extern unsigned int Test_Times_Limit;
extern int keyfag;
extern int keybug;
extern int AAA;
extern int status_flag;
extern float Test_displacemen;
extern int safety_fag;
extern int safety_bug;


////////////////////////////////////////////////电机状态，上下停，设置原点
void	MotorAction_Control(uint8_t	State)
{
//	Encoder_Count=gtim_get_encode();
	//手动上，手动下，手动返回原点，点动上，点动下
	switch(State)
	{
		
		case Manual_up:       //手动上
				SpeedMotorInit(MOTORNUM1);
				SetSpeed(MOTORNUM1,200000);
				MotorAction(MOTORNUM1,MOTOR_UP);
			break;		
		
		case Manual_Down:    //手动下
				SpeedMotorInit(MOTORNUM1);
				SetSpeed(MOTORNUM1,200000);
				MotorAction(MOTORNUM1,MOTOR_DOWN);
			break;		
		
		case Manual_return:  //手动返回原点
			
			break;		
		
		case Jog_Up:        //点动上
			break;

		case Jog_Down:      //
			break;

    case Manual_stop:      //手动停止
				MotorAction(MOTORNUM1,MOTOR_STOP);
			break;

		case Distance_Zero:   //位移清零
				ClearTimer();
			break;		
    case weight_Zero:      //力值清零

				weight_Zero_Data=weight_ad7190_ReadAvg(3);
			break;
		
    case All_Zero:      //全部清零
				
			break;


	
		
	}
}

/////////////////////////
void RUN(int CCC)//Key1mm_up  Key1mm_down
{
//	switch(CCC)
//		{
//			case 1:    //拉力机模式模式
//			{		

//				break;				
//					
//				}		
//		default : break;
//	}
}
	

/////////////////////////////////////////////////////////

void Key1mm_up(void)//Key1mm_up  Key1mm_down
{
    SpeedMotorInit(MOTORNUM1);
    SetSpeed(MOTORNUM1, 250000);
    MotorAction(MOTORNUM1, MOTOR_UP);
    last_direction = 1;

    if (current_displacement <= 0.0f)
    {
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.050f))
        {
            SetSpeed(MOTORNUM1, 42000);
        }
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 >= (current_displacement - 0.050f))
        {
            SetSpeed(MOTORNUM1, 42000);
        }
        if (displacement1 >= (current_displacement - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
}


void Key1mm_down(void)
{
    SpeedMotorInit(MOTORNUM1);          // 初始化电机
    SetSpeed(MOTORNUM1, 250000);        // 设置初始速度
    MotorAction(MOTORNUM1, MOTOR_DOWN); // 电机下行
    last_direction = 0;

    if (current_displacement <= 0.0f)
    {
        if ((displacement1) <= ((current_displacement) + 0.050f))
        {
            SetSpeed(MOTORNUM1, 42000);
        }
        if ((displacement1) <= ((current_displacement) + 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 <= (current_displacement + 0.050f))
        {
            SetSpeed(MOTORNUM1, 42000);
        }
        if (displacement1 <= (current_displacement + 0.001f))
        {
            move_flag = 0;
           
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
}


void Key01mm_down(void)
{
    SpeedMotorInit(MOTORNUM1);          // 初始化电机
    SetSpeed(MOTORNUM1,50000);        // 设置初始速度
    MotorAction(MOTORNUM1, MOTOR_DOWN); // 电机下行
    last_direction = 0;

    if (current_displacement <= 0.0f)
    {
        if ((displacement1) <= ((current_displacement) + 0.005f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if ((displacement1) <= ((current_displacement) + 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 <= (current_displacement + 0.005f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (displacement1 <= (current_displacement + 0.001f))
        {
            move_flag = 0;
           
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
}


void Key01mm_up(void)
{
    SpeedMotorInit(MOTORNUM1);
    SetSpeed(MOTORNUM1, 50000);
    MotorAction(MOTORNUM1, MOTOR_UP);
    last_direction = 1;

    if (current_displacement <= 0.0f)
    {
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.005f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 >= (current_displacement - 0.005f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (displacement1 >= (current_displacement - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
					keyfag = 0;
					keybug = 0;
        }
    }
}

///////////////////////四个按键上下1Mm,上下0.1mm，


void specifyDisplacement_down(void)
{
    SpeedMotorInit(MOTORNUM1);          // 初始化电机
    SetSpeed(MOTORNUM1, 50000);        // 设置初始速度
    MotorAction(MOTORNUM1, MOTOR_DOWN); // 电机下行
    last_direction = 0;

    if (current_displacement < 0.0f)
    {
        if ((displacement1) <= ((current_displacement) + 0.1f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if ((displacement1) <= ((current_displacement) + 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 <= (current_displacement + 0.1f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (displacement1 <= (current_displacement + 0.001f))
        {
            move_flag = 0;
           
            MotorAction(MOTORNUM1, MOTOR_STOP);
        }
    }
}


void specifyDisplacement_up(void)
{
    SpeedMotorInit(MOTORNUM1);
    SetSpeed(MOTORNUM1, 50000);
    MotorAction(MOTORNUM1, MOTOR_UP);
    last_direction = 1;

    if (current_displacement < 0.0f)
    {
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.1f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (fabs(displacement1) <= (fabs(current_displacement) - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
        }
    }
    if (current_displacement > 0.0f)
    {
        if (displacement1 >= (current_displacement - 0.1f))
        {
            SetSpeed(MOTORNUM1, 20000);
        }
        if (displacement1 >= (current_displacement - 0.001f))
        {
            move_flag = 0;
            
            MotorAction(MOTORNUM1, MOTOR_STOP);
        }
    }
}

/***********************************************************************************************************/

void motorReturnZero(void)
{
   switch(return_direction)
   {
	   case 0: // 电机下行回位
	   {
		   SpeedMotorInit(MOTORNUM1);
			SetSpeed(MOTORNUM1, 100000);
		   if(displacement1 >= 0.1f)
		   {
			   MotorAction(MOTORNUM1, MOTOR_DOWN);
		   }
		   if(displacement1<0.1f)
		   {
			   SetSpeed(MOTORNUM1, 40000);
			   MotorAction(MOTORNUM1, MOTOR_DOWN);
		   }
		   if(displacement1<0.01f)
		   {
			   SetSpeed(MOTORNUM1, 20000);
			   MotorAction(MOTORNUM1, MOTOR_DOWN);
		   }
		   if(displacement1<0.0001f)
		   {
			   move_flag = 0;
			   MotorAction(MOTORNUM1,MOTOR_STOP);
		   }
		   
		   break;
	   }
	   case 1: // 电机上行回位
	   {
			SpeedMotorInit(MOTORNUM1);
			SetSpeed(MOTORNUM1, 100000);
			if(displacement1 <= -0.1f)
			{
			   MotorAction(MOTORNUM1, MOTOR_UP);
			}
			if(displacement1>-0.1f)
			{
			   SetSpeed(MOTORNUM1, 40000);
				MotorAction(MOTORNUM1, MOTOR_UP);
			}
			if(displacement1>-0.01f)
			{
			   SetSpeed(MOTORNUM1, 20000);
				MotorAction(MOTORNUM1, MOTOR_UP);
			}
			if(displacement1>-0.0001f)
			{
			   move_flag = 0;
			   MotorAction(MOTORNUM1,MOTOR_STOP);
			}
		   break;
	   }
	   
	   default:
		   break;
		
	}	   

   


}























