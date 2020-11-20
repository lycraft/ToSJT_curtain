//////////////////////////////////////////////////////////////////////////////////	 
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PA8（SCL）
//              D1   接PB15（SDA）
//              RES  接PB14
//              DC   接PB13
//              CS   接PA3               
//              ----------------------------------------------------------------
//							ADC-PA2,PA3
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "HDC1080.h"
#include "key.h"
#include "timer.h"
#include "exti.h" 
#include "adc.h"
#include "led.h"

int CountMotor = 0;
uint8_t mode = 0, MotorDirFlag = 0;//
uint8_t flag = 1;
uint16_t LightLine = 2000;

void meau()//mode0不刷新
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,0,"Mode");
		OLED_ShowString(0,2,"Light");
		OLED_ShowString(0,4,"Set");
		OLED_ShowString(0,6,"Animation");
//		OLED_ShowString(72,6,"Lycraft");
}

/******************************************************************
电机正转
******************************************************************/
void Motor_Forward()
{
	switch(CountMotor % 8)//开始电机旋转
			{
				case 0:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //拉高引脚					
				}

					;break;
				case 1:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //拉高引脚		
				}
					;break;
				case 2:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高引脚		
				}
					;break;
				case 3:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高引脚		
				}
					;break;
				case 4:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //拉高引脚		
				}
					;break;
				case 5:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //拉高引脚		
				}
					;break;
				case 6:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //拉高引脚		
				}
					;break;
				case 7:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //拉高引脚		
				}
					;break;
			}
}

/******************************************************************
电机反转
******************************************************************/
void Motor_Back()
{
	switch(CountMotor % 8)//开始电机旋转
			{
				case 7:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //拉高引脚					
				}

					;break;
				case 6:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //拉高引脚		
				}
					;break;
				case 5:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高引脚		
				}
					;break;
				case 4:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //拉高引脚		
				}
					;break;
				case 3:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //拉高引脚		
				}
					;break;
				case 2:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //拉高引脚		
				}
					;break;
				case 1:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //拉高引脚		
				}
					;break;
				case 0:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_4);//拉高引脚
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //拉高引脚		
				}
					;break;
			}
}

void Motor_Stop()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//拉低引脚
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);//拉低引脚
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);//拉低引脚
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//拉低引脚		
}

int main(void)
 {	
		vu8 key=0;
	   u16 adcx;
//		float tempadc;
	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
	  HDC1080_Init();
//		KEY_Init();          //初始化与按键连接的硬件接口
		EXTIX_Init();					//按键初始化
	  Adc_Init();		  		//ADC初始化
		LED_Init();
		TIM3_Int_Init(10 - 1,7200 - 1);	 //new 72000000/7200 = 10khz频率		计数到10就是1ms中断服务函数触发一次
	 
	 meau();
	while(1) 
	{		
		//测出ADC的值(光照强度)并显示在屏幕上
		if(CountMotor == 100)
		{
			adcx=Get_Adc_Average(ADC_Channel_2,10);//PA2的ADC的AD值
				
			OLED_ShowNum(65,2,adcx,4,16);	//显示ADC的值
		}


/*******************************************************************
自动模式与手动模式		
*******************************************************************/
			if(mode % 2 == 0)				//自动模式
		{
			if(flag == 1)//只执行一次的
			{
				OLED_ShowString(65,0,"Auto");
				flag--;				
			}
			if(adcx >= LightLine)//光照值大于2000窗帘就开始工作
			{
				Motor_Forward();				//电机正转
			}
			else if(adcx < LightLine)
			{
				Motor_Stop();
			}
			if(CountMotor == 100)
			{
				OLED_ShowNum(65,4,LightLine,4,16);	//显示ADC的值
			}
			
		}
/******************************************************************/		
			if(mode % 2 == 1)				//手动模式
		{
			if(flag == 1)//只执行一次的
			{
				OLED_ShowString(65,0,"Pres");
				flag--;				
			}
			if(MotorDirFlag == 0)
			{
				Motor_Forward();			//电机正转
			}
			if(MotorDirFlag == 1)
			{
				Motor_Back();					//电机反转
			}
			if(MotorDirFlag == 2)
			{
				Motor_Stop();					//电机停转
			}
				
		}
/*******************************************************************/		
	}	  
}

