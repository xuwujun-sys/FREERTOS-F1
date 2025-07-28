
#ifndef  LOADGETH
#define  LOADGETH

#include "includes.h"

#define	LOADCALINUM	12
#define	CHANNELNUM	5
#define	LOADCHANNEL	0


float	GetLoadData(int32_t Adin,uint8_t channel);
uint8_t	GetLoadAdCoe(int32_t	*AdValue,int32_t	*NominalValue,float	*coefficient,uint8_t *CalPointNum);


extern	float Coefficient[CHANNELNUM][LOADCALINUM-1];//斜率计算
extern	int32_t	AdValuedata[CHANNELNUM][LOADCALINUM];//校准AD值
extern	int32_t NominalData[CHANNELNUM][LOADCALINUM];//校准标称砝码值
extern	uint8_t CalPointNumbuf[CHANNELNUM];//校准点数
extern	float	ZeroAccess[CHANNELNUM];//归零偏差值
extern	int32_t LoadRange[CHANNELNUM];//力量程
extern	int32_t LoadZero[CHANNELNUM];//力量程
extern	float LoadNow[CHANNELNUM];//当前力值















#endif

