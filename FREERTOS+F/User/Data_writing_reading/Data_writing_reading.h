
#ifndef  DATA_WRITING_READING
#define  DATA_WRITING_READING
#include "stm32f4xx_hal.h"
#include "math.h"
#include	"stdint.h"




extern __IO int32_t weight_Zero_Data;  //力值初始零点记录值
extern __IO float weight;

extern __IO int32_t weight_proportion;  //校准值

extern long double Tx_Buffer[31];//暂定数组最小26葛float型数据
extern long double Rx_Buffer[31];

//extern long double Tx_Buffer1[2];//暂定数组最小2葛float型数据，与力值和AD数组分开
//extern long double Rx_Buffer1[2];


void readingflash(void);
void writingflash(void);

//void readingflash1(void);
//void writingflash1(void);


#endif
