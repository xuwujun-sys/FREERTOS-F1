
#ifndef  MOTOR
#define  MOTOR
#include "stm32f4xx_hal.h"
#include "math.h"
#include	"stdint.h"





#define	MOTORCLK	2000000 //实际时钟频率

#define	MOTORNUM1	0
#define	MOTORNUM2	1
#define	MOTORNUM3	2
#define	MOTORNUM4	3

#define	MOTOR1ON()		HAL_TIM_OC_Start_IT(&hAdvtimy,TIM_CHANNEL_1)
#define	MOTOR1OFF()		HAL_TIM_OC_Stop_IT(&hAdvtimy,TIM_CHANNEL_1)
#define	MOTOR2ON()		HAL_TIM_OC_Start_IT(&hAdvtimx,TIM_CHANNEL_1)
#define	MOTOR2OFF()		HAL_TIM_OC_Stop_IT(&hAdvtimx,TIM_CHANNEL_1)
#define	MOTOR3ON()		HAL_TIM_OC_Start_IT(&hGentimx,TIM_CHANNEL_1)
#define	MOTOR3OFF()		HAL_TIM_OC_Stop_IT(&hGentimx,TIM_CHANNEL_1)
#define	MOTOR4ON()		HAL_TIM_OC_Start_IT(&hGentimy,TIM_CHANNEL_1)
#define	MOTOR4OFF()		HAL_TIM_OC_Stop_IT(&hGentimy,TIM_CHANNEL_1)

#define	MOTOR1CLEAR()		__HAL_TIM_SET_COUNTER(&hAdvtimy, 0);							
#define	MOTOR2CLEAR()		__HAL_TIM_SET_COUNTER(&hAdvtimx, 0);							
#define	MOTOR3CLEAR()		__HAL_TIM_SET_COUNTER(&hGentimx, 0);							
#define	MOTOR4CLEAR()		__HAL_TIM_SET_COUNTER(&hGentimy, 0);							



#define MOTOR_STOP		0
#define MOTOR_UP			1
#define MOTOR_DOWN		2
#define MOTOR_STOPREAL		3


#define DIR1_CCW()					HAL_GPIO_WritePin(ADVANCED_TIMy_DIR1_PORT, ADVANCED_TIMy_DIR1_PIN, GPIO_PIN_RESET)
#define DIR1_CW()						HAL_GPIO_WritePin(ADVANCED_TIMy_DIR1_PORT, ADVANCED_TIMy_DIR1_PIN, GPIO_PIN_SET)
#define DIR2_CCW()					HAL_GPIO_WritePin(ADVANCED_TIMx_DIR2_PORT, ADVANCED_TIMx_DIR2_PIN, GPIO_PIN_RESET)
#define DIR2_CW()						HAL_GPIO_WritePin(ADVANCED_TIMx_DIR2_PORT, ADVANCED_TIMx_DIR2_PIN, GPIO_PIN_SET)
#define DIR3_CCW()					HAL_GPIO_WritePin(GENERAL_TIMx_DIR3_PORT, GENERAL_TIMx_DIR3_PIN, GPIO_PIN_RESET)
#define DIR3_CW()						HAL_GPIO_WritePin(GENERAL_TIMx_DIR3_PORT, GENERAL_TIMx_DIR3_PIN, GPIO_PIN_SET)
#define DIR4_CCW()					HAL_GPIO_WritePin(GENERAL_TIMy_DIR4_PORT, GENERAL_TIMy_DIR4_PIN, GPIO_PIN_RESET)
#define DIR4_CW()						HAL_GPIO_WritePin(GENERAL_TIMy_DIR4_PORT, GENERAL_TIMy_DIR4_PIN, GPIO_PIN_SET)


#ifndef  ON
#define  ON	1
#endif
#ifndef  OFF
#define  OFF	0
#endif

#ifndef  CCW
#define  CCW	1
#endif
#ifndef  CW
#define  CW	0
#endif

#define		UPDOWNSPEEDTIMES 100
typedef struct
{
	uint32_t	SpeedMax;					//最大速度
	uint32_t	SpeedMin;					//最小速度
	uint32_t 	UpStartSpeed;			//加速启动速度
	uint32_t	SpeedReturn;			//回位速度
	/*最大脉冲100000*/
	uint32_t	FreoutMax;				//最大频率当前最大100000 100khz
	
	uint32_t  PreSpeed;
	uint32_t 	CurrentSpeed;			//当前设置速度
	uint32_t	SpeedSetIndex[2];	//实际设置比较值
	uint8_t 	MotorState[2];		//电机状态
	uint32_t 	SpeedCompareIndex[UPDOWNSPEEDTIMES];//加减速数组 暂定100次加速至最大速度
	uint8_t 	UpDownTimesIndex;			//加减速实际需要
	uint8_t 	CurrentSpeedCompare;	//当前设定比较值
	uint8_t 	FlashNewCompareFlag;	//更新设定比较值标志
	uint8_t		Order;
	uint8_t   Test_State;//测试状态
	uint8_t 	SpeedMode;						//加减速模式 0为正常加减速 1为快速模式，测试的时候需要紧急停止时使用
	uint32_t 	SpeedUpTime;					//加速时间
	uint32_t 	SpeedDownTime;				//减速时间
	int32_t 	CurrentCountTimhard;	//中断当前计数  用以比较与编码器获取计数的比例关系
	uint8_t 	StopFlag;							//停止标志	此标志
	uint8_t		LimitFlag;						//限位标志
}SpeedIndexDef;

extern	SpeedIndexDef	SpeedMotor[4];




void 	Motor_OC_IT(uint8_t MotorNum,uint8_t	State,uint8_t Dir);
void	SetSpeed(uint8_t MotorChannel,uint32_t	SpeedV0);
void	MotorAction(uint8_t MotorNum,uint8_t	State);
void	SpeedMotorInit(uint8_t MotorChannel);
void	SpeedUpDowncal(uint8_t Motornum,int32_t StartSpeed,int32_t TargetSpeed,uint8_t	*UpDownTimesIndex);











#endif

