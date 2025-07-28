
#include "loadGet.h"

/*
使用单精度浮点计算，精确到小数点后6位，即单位为kg时能精确到0.01g

*/

float Coefficient[CHANNELNUM][LOADCALINUM-1];//斜率计算
int32_t	AdValuedata[CHANNELNUM][LOADCALINUM];//校准AD值
int32_t NominalData[CHANNELNUM][LOADCALINUM];//校准标称砝码值
uint8_t CalPointNumbuf[CHANNELNUM];//校准点数
float	ZeroAccess[CHANNELNUM];//归零偏差值
int32_t LoadRange[CHANNELNUM];//力量程
int32_t LoadZero[CHANNELNUM];//力量零点
float LoadNow[CHANNELNUM]={0};//当前力

uint8_t	GetLoadAdCoe(int32_t	*AdValue,int32_t	*NominalValue,float	*coefficient,uint8_t *CalPointNum)
{
	uint8_t	i=0;
	for(i=0,CalPointNum[0]=0;i<12;i++)
	{
		if(AdValue[i])
			CalPointNum[0]++;
		else break;
	}
	if(CalPointNum[0] < 2)
		return 1;
	for(i=0;i<CalPointNum[0]-1;i++)
	{
		//coefficient[i]=fabs((float)NominalValue[i]);
		coefficient[i]=fabs((float)(NominalValue[i+1]-NominalValue[i])/(AdValue[i+1]-AdValue[i]));//
		if(coefficient[i]==0)
			return 2;//斜率为0出错
	}
	return 0;
}

float	GetLoadData(int32_t Adin,uint8_t channel)
{
	
	uint8_t i=0;
	
	for(i=0;i<CalPointNumbuf[channel]-1;i++)
	{
		
		if(abs(Adin-AdValuedata[channel][0])<abs(AdValuedata[channel][0]-AdValuedata[channel][i+1]))
		{
			return (float)((Adin-AdValuedata[channel][i])*Coefficient[channel][0]+NominalData[channel][i])/10000.0f;
			
		}
		else if(i==CalPointNumbuf[channel]-2)
		{
			return (float)((Adin-AdValuedata[channel][i])*Coefficient[channel][0]+NominalData[channel][i])/10000.0f;
			
		}
	}
	return 0;
}












