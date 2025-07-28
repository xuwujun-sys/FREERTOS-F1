
/* 包含头文件 ----------------------------------------------------------------*/

#include "strokeGet.h"
#include "./BSP/EncoderTIM/EncoderTIM.h"

STROKEDATA	Strokedata;

//int32_t GetEncoderCount(uint8_t Channel)
//{
//	switch(Channel)
//	{
//		case 0:	
//			return  OverflowCount[Channel]*65536 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);
////		case 1:			
////			return  OverflowCount[Channel]*65536 + __HAL_TIM_GET_COUNTER(&htimy_Encoder);
////		case 2:			
////			return  OverflowCount[Channel]*65536 + __HAL_TIM_GET_COUNTER(&htimz_Encoder);
//	}
//	return 0;
//}

//float GetStroke(uint8_t Channel)
//{
//	return	GetEncoderCount(Channel)/1000.0f*(Strokedata.StrokeCoe[Channel]/10000.0f);
//}










