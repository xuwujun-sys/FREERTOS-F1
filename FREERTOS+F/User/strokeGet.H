
#ifndef  STROKEGET
#define  STROKEGET

#include "stdint.h"




//extern	uint32_t StrokeCoe;//位移系数
//extern	float	Stroke[3];//3个通道的位移值
//extern	float ZeroStroke[3];
//extern	float	TempStroke[3];//比较值

typedef struct{	
double StrokeCoe[4];//位移系数 默认*10000
float 		ZeroStroke[4];//位移零值
float			Stroke[4];//3个通道的位移值
float			TempStroke[4];//比较值
uint8_t		StrokeUD_DirFlag[4];//位移升降标志
float			ReturnStroke[4];//回位位置
uint8_t		Returnflag[4];//回位标志
uint8_t		ReturnStage[4];//回位阶段
uint8_t		ReturnDir[4];//回位方向	
	
}STROKEDATA;

extern	STROKEDATA	Strokedata;


int32_t GetEncoderCount(uint8_t Channel);
float GetStroke(uint8_t Channel);






#endif

