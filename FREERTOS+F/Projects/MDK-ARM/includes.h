#include "freertos_demo.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LCD/lcd.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT


#include "stm32f4xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////控制相关
#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/DMA/dma.h"
#include "./BSP/KEY/key.h"
#include "./BSP/AD7190/AD.h"
#include "./BSP/USER/MOTOR.h"
#include "./BSP/USER/Motor_Control.h"
#include "Data_writing_reading.h"
#include "./BSP/spiflash/spiflash.h"
#include "./BSP/EncoderTIM/EncoderTIM.h"
#include "./BSP/TIMER/btim.h"
#include "Message_processing.h"
#include "force_treatment.h"
#include "./BSP/TTIM/TTIM.h"
#include "./BSP/AdvancedTIM/AdvancedTIM.h" 
#include "strokeGet.h"
#include "loadGet.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////关键字相关
#include "string.h"
#include  "math.h"
#include  "stdio.h"
#include  "ctype.h"
#include  "stdlib.h"
#include  "stdarg.h"
/*FreeRTOS*********************************************************************************************///系统相关
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "./MALLOC/malloc.h"
#include "freertos_demo.h"
#include "queue.h"




#endif


