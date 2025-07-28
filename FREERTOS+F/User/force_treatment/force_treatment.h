

#ifndef __FORCE_TREATMENT_H__
#define __FORCE_TREATMENT_H__

#include "stm32f4xx_hal.h"
#include "math.h"
//#include	"stdint.h



//void Testtimes_FloatToByte(float floatNum,unsigned char* byteArry);
//void weight_FloatToByte(float floatNum,unsigned char* byteArry);
//void AD_FloatToByte(float floatNum,unsigned char* byteArry);
//void displacement_FloatToByte(float floatNum,unsigned char* byteArry);
//int DectoHex(int dec, unsigned char *hex, int length);




extern float displacement1;          //计算位移值
extern __IO int32_t weight_count;   //重量相关，原始值，尚未计算
extern __IO float data_temp;        //记录处理之后的原始值，从开始，绝对值
extern __IO float position1;        //

//extern __IO int32_t Weight_calibration;   //重量校准系数
extern __IO float weight_data_temp;

extern uint8_t g_sendbuf[20];  

extern uint8_t sb_sendbuf[88]; //测试用


extern unsigned char Testtimes_floattohex[4];
extern unsigned char weight_floattohex[4];
extern unsigned char AD_floattohex[4];
extern unsigned char displacement1_floattohex[4];
extern unsigned char State_condition_DectoHex[1];  


void Force_value_data(void);

#endif	





