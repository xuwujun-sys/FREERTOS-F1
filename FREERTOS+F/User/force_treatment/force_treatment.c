
#include "includes.h"

////都要改


__IO int32_t weight_count=0;   //重量相关，原始值，尚未计算Weight calibration  weight_data_temp  Calibration_coefficient
__IO float data_temp;
__IO float weight_data_temp;
//__IO int32_t Calibration_coefficient=0;

__IO float position1 = 0; //脉冲
float displacement1=0;     //计算的位移值mm，变量
//float Displacement1_change=0;//位移值mm，变量

unsigned char Testtimes_floattohex[4];
unsigned char weight_floattohex[4];
unsigned char AD_floattohex[4];
unsigned char displacement1_floattohex[4];
unsigned char State_condition_DectoHex[1];  
uint8_t g_sendbuf[20]; 





//////////////////////////////////////////////////////////////////////
//float Displacement = 10,Force = 1000;
//float Pause_Time = 1000,Hold_Time = 2000;
//unsigned char Loop_Flag = 0,Return_Flag = 0;
//unsigned char MACHINE_TEST_FLAG=0;
//unsigned int Test_Times_Limit = 100;
///////////////////////////////////////////////////////////////////

 


//uint8_t sb_sendbuf[88]; //测试用


////int32_t Weight_coefficient(void)
////{
////	if(Calibration_mark[0]==1)
////	{
////		Calibration_coefficient=weight_data_temp/Calibration_load[0];	
////		
////	}
////}







//void Force_value_data(void)
//{
//		AD7190_Init();
//		//AD7190参数配置
//		weight_ad7190_conf();
//		//延时0.5秒
//		weight_Zero_Data=weight_ad7190_ReadAvg(3);
//		filter();	   
//	
//    while(1)
//    {
//	    weight_count=filter();
//			data_temp=(weight_count-weight_Zero_Data);
//		
////			weight=fabs(data_temp*1000.0f/(-360));	//默认的一个系数，这个也是要保存的，下边的是保存一个数组


//			
//			//也可以用标志判断，暂未写，可改，数组已经定义
//			//分段函数计算出的重量是经过取绝对值处理的，后期可改为正负形式的
//			//要保存的不是校准的重量和校准的AD值，是这两个计算之后的一个值，也就是一个12位的数组，串口接收那段需要去掉
//			if((0 <fabs(data_temp))&&(fabs(data_temp) < 3))
//			{
////				weight=fabs(data_temp*1000.0f/(1800.0f));
//				weight=(data_temp*1000.0f/(1800.0f));
//			} 

//		  else if((3 <= fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[0]+1))//第1段分段函数
//			{	
//				if((Calibration_mark[0]==1)&&(Weight_A[0]==1))
//				{
//					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
//					Weight_calibration[0]=weight_data_temp/Calibration_load[0];	
//					Tx_Buffer[0] = Weight_calibration[0];		
//					Calibration_mark[0]=2;Weight_A[0]=2;
//				}
////			  weight=fabs(data_temp*1000.0f/Weight_calibration[0]);	//
//				weight=(data_temp*1000.0f/Weight_calibration[0]);	//
//			}
//			else if((Calibration_weight[0] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[1]+1))//第2段分段函数
//			{
//				if((Calibration_mark[1]==1)&&(Weight_A[1]==1))
//				{
//					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
//				  Weight_calibration[1]=weight_data_temp/Calibration_load[1];	
//					Tx_Buffer[1] = Weight_calibration[1];		
//          Calibration_mark[1]=2;Weight_A[1]=2;					
//				}
////			  weight=fabs(data_temp*1000.0f/Weight_calibration[1]);	//
//				weight=(data_temp*1000.0f/Weight_calibration[1]);	//
//			}
//			else if((Calibration_weight[1] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[2]+1))//第3段分段函数
//			{
//				if((Calibration_mark[2]==1)&&(Weight_A[2]==1))
//				{
//					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
//				  Weight_calibration[2]=weight_data_temp/Calibration_load[2];	
//					Tx_Buffer[2] = Weight_calibration[2];		
//					Calibration_mark[2]=2;Weight_A[2]=2;
//				}					
////			  weight=fabs(data_temp*1000.0f/Weight_calibration[2]);	//
//				weight=(data_temp*1000.0f/Weight_calibration[2]);	//
//			}
//			else if((Calibration_weight[2] < fabs(data_temp)) && (fabs(data_temp) <= Calibration_weight[3]+1))//第4段分段函数
//			{
//				if((Calibration_mark[3]==1)&&(Weight_A[3]==1))
//				{
//					weight_data_temp=(weight_count-weight_Zero_Data)*1000;
//				  Weight_calibration[3]=weight_data_temp/Calibration_load[3];
//					Tx_Buffer[3] = Weight_calibration[3];					
//					Calibration_mark[3]=2;Weight_A[3]=2;
//				}					
////			  weight=fabs(data_temp*1000.0f/Weight_calibration[3]);	//
//				weight=(data_temp*1000.0f/Weight_calibration[3]);	//
//			}
//			else      //计算找出最后一段校准值，超出校准以最后一个为校准系数计算
//			{
////				 weight=fabs(data_temp*1000.0f/(-352.0f));	
//				weight=(data_temp*1000.0f/(-352.0f));	
//			}

//			
////		weight=fabs(data_temp*1000.0f/Weight_coefficient);
//		

////			if((Zero_load_bug==1)&&(weight>Zero_weight)) //过不过所谓的荷重零点
////			{
////				weight=weight-Zero_weight;
////			}
////			else     //先下走，下走力值传感器下拉输出的值为正，才使用这个，另一个是把这边删除就行，之后单独分出方向上下，即可，这边就不做处理，，援其/励军/掩虚
////			{
////				weight = 0 - weight;
////			}
//			
//			
//			
//			
//      think=Distance_value;  //
//			
////			displacement=(0-(gtim_get_encode()/think));		//上走，输出的位移的值为正
////			displacement=((gtim_get_encode()/think));		////先下走，下走位移输出的值为正

//			
////      Displacement_change=displacement;
////			if(Displacement_change == (Encoder_Count/Distance_value))
////			{
////				State_condition=0;
////			}
//			
////			State_condition=status_flag;
////			if(fabs(displacement) <= 0.01f)
////			{
//////				status_flag=0;
////				State_condition=0;
////			}

//					


////			DectoHex(State_condition, State_condition_DectoHex, 1); 
//			Testtimes_FloatToByte(Testtimes,Testtimes_floattohex);//转换乘IEEEE754进制（16进制的一种）测试次数转换
//			AD_FloatToByte(data_temp,AD_floattohex);//转换乘IEEEE754进制（16进制的一种）AD值转换
//			weight_FloatToByte(weight,weight_floattohex);//转换乘IEEEE754进制（16进制的一种） 重量转换
//			displacement_FloatToByte(displacement1,displacement1_floattohex);//转换乘IEEEE754进制（16进制的一种） 位移转换
//	 
//	 
//		
//		 g_sendbuf[0]=0XAA;                             //帧头
//		 g_sendbuf[1]=State_condition_DectoHex[0];      //状态
//		 g_sendbuf[2]=Testtimes_floattohex[3];          //测试次数
//		 g_sendbuf[3]=Testtimes_floattohex[2];          //
//		 g_sendbuf[4]=Testtimes_floattohex[1];          //
//		 g_sendbuf[5]=Testtimes_floattohex[0];          //
//		 g_sendbuf[6]=displacement1_floattohex[3];       //行程
//		 g_sendbuf[7]=displacement1_floattohex[2];       //
//		 g_sendbuf[8]=displacement1_floattohex[1];       //
//		 g_sendbuf[9]=displacement1_floattohex[0];       //
//		 g_sendbuf[10]=weight_floattohex[3];            //重量
//		 g_sendbuf[11]=weight_floattohex[2];            //
//		 g_sendbuf[12]=weight_floattohex[1];            //
//		 g_sendbuf[13]=weight_floattohex[0];            //
//		 g_sendbuf[14]=AD_floattohex[3];                //原始AD值
//		 g_sendbuf[15]=AD_floattohex[2];                //
//		 g_sendbuf[16]=AD_floattohex[1];                //
//		 g_sendbuf[17]=AD_floattohex[0];                //
//		 g_sendbuf[18]=0X00;                            //补充
//		 g_sendbuf[19]=0XDD;                            //
//		 




//		 
//			vTaskDelay(1);                           /* 延时1000ticks */

//    }
//	
//}


//void Testtimes_FloatToByte(float floatNum,unsigned char* byteArry)
//{
//    char* pchar=(char*)&floatNum;
//    int i = 0;
//    for( i=0;i<sizeof(float);i++)
//    {
//			*byteArry=*pchar;
//			pchar++;
//			byteArry++;
// 
//    }
//}



//void weight_FloatToByte(float floatNum,unsigned char* byteArry)
//{
//    char* pchar=(char*)&floatNum;
//    int i = 0;
//    for( i=0;i<sizeof(float);i++)
//    {
//			*byteArry=*pchar;
//			pchar++;
//			byteArry++;
// 
//    }
//}

//void AD_FloatToByte(float floatNum,unsigned char* byteArry)
//{
//    char* pchar=(char*)&floatNum;
//    int i = 0;
//    for( i=0;i<sizeof(float);i++)
//    {
//			*byteArry=*pchar;
//			pchar++;
//			byteArry++;
// 
//    }
//}

//void displacement_FloatToByte(float floatNum,unsigned char* byteArry)
//{
//    char* pchar=(char*)&floatNum;
//    int i = 0;
//    for( i=0;i<sizeof(float);i++)
//    {
//			*byteArry=*pchar;
//			pchar++;
//			byteArry++;
// 
//    }
//}


//int DectoHex(int dec, unsigned char *hex, int length) 
//{ 
//  for(int i=length-1; i>=0; i--) 
//  { 
//    hex[i] = (dec%256)&0xFF; 
//    dec /= 256; 
//  } 

//  return 0; 
//}







