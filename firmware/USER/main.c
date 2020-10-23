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
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "HDC1080.h"
#include "key.h"
#include "exti.h" 
#include "adc.h"

int sa=0,sb=0,sc=0,sd=0;
int wendu,shidu;
uint8_t modetemp = 6,mode = 4;//modetemp是指示位置 指向选中的mode，==============mode0-4
uint8_t flag = 1;

void loading()//开机进度条
{
	uint8_t loadingcount;
	OLED_ShowString(24,0,"Loading...");
	for(loadingcount = 0;loadingcount < 16;loadingcount++)
	{
			OLED_ShowChar(loadingcount * 8,5,'>');
			delay_ms(loadingcount % 8 * 10 + 10);
	}
}

void measure1()//屏幕不刷新的东西 
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
	
		OLED_ShowCHinese(0,2,7);//温度
		OLED_ShowCHinese(18,2,8);//
		OLED_ShowCHinese(0,4,9);//湿度
		OLED_ShowCHinese(18,4,8);//
		OLED_ShowChar(37,2,':');
		OLED_ShowChar(37,4,':');
		OLED_ShowChar(71,2,'.');
		OLED_ShowChar(71,4,'.');
		OLED_ShowChar(90,2,'C');
		OLED_ShowChar(90,4,'%');
	 
	 	OLED_ShowCHinese(0,6,0);
		OLED_ShowCHinese(18,6,1);
		OLED_ShowCHinese(36,6,2);//的
		OLED_ShowCHinese(54,6,3);//迷
		OLED_ShowCHinese(72,6,4);//你
		OLED_ShowCHinese(90,6,5);//屏
		OLED_ShowCHinese(108,6,6);//幕
	
		OLED_ShowNum(0,0,mode,2,16);
}
void measure()//屏幕上时刻刷新的东西
{
		wendu = HDC1080_Read_Temper();
		shidu = HDC1080_Read_Humidi();
		
		OLED_ShowNum(55,2,wendu/100,2,16); 
		OLED_ShowNum(72,2,wendu/10%10,1,16);
		OLED_ShowNum(80,2,wendu%10,1,16);

		OLED_ShowNum(55,4,shidu/100,2,16);
		OLED_ShowNum(72,4,shidu/10%10,1,16);
		OLED_ShowNum(80,4,shidu%10,1,16);
		
}
void meau()//mode0不刷新
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"Measure");
		OLED_ShowString(40,2,"Voltage");
		OLED_ShowString(40,4,"Animation");
		OLED_ShowString(72,6,"Lycraft");
}
void voltage()//电流电压测量刷新的
{
		OLED_ShowNum(55,2,11,2,16); 
		OLED_ShowNum(72,2,1,1,16);
		OLED_ShowNum(80,2,1,1,16);
		OLED_ShowNum(88,2,1,1,16);
		OLED_ShowNum(96,2,1,1,16);
		OLED_ShowNum(104,2,1,1,16);

		OLED_ShowNum(55,4,11,2,16);
		OLED_ShowNum(72,4,1,1,16);
		OLED_ShowNum(80,4,1,1,16);
		OLED_ShowNum(88,4,1,1,16);
		OLED_ShowNum(96,4,1,1,16);
		OLED_ShowNum(104,4,1,1,16);	
	
}
void voltage1()//电流电压测量不刷新的
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,10);//电
		OLED_ShowCHinese(18,2,11);//压
		OLED_ShowCHinese(0,4,10);//电
		OLED_ShowCHinese(18,4,12);//流
		
		OLED_ShowChar(37,2,':');
		OLED_ShowChar(37,4,':');
		OLED_ShowChar(71,2,'.');
		OLED_ShowChar(71,4,'.');
		OLED_ShowChar(112,2,'V');
		OLED_ShowChar(112,4,'A');	
}
int main(void)
 {	
		vu8 key=0;
	   u16 adcx,adca;
		float tempadc;
	 
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	
		OLED_Init();			//初始化OLED  
	  HDC1080_Init();
//		KEY_Init();          //初始化与按键连接的硬件接口
		EXTIX_Init();
	  Adc_Init();		  		//ADC初始化
		
	 loading();
	 
	while(1) 
	{		
		if(modetemp == 3||modetemp == 9)//======箭头标志位
		{
			modetemp = 6;
		}
//========================================================================Meau
		if(mode % 4 == 0)
		{
			if(flag == 1)//只执行一次的
			{
				meau();	
				flag--;				
			}
			OLED_ShowChar(32,(modetemp % 3) * 2,'>');
			OLED_ShowChar(32,(modetemp - 1) % 3 * 2,' ');
			OLED_ShowChar(32,(modetemp + 1) % 3 * 2,' ');
		}
//========================================================================Measure
		if(mode % 4 == 1)
		{
			if(flag == 1)//只执行一次的
			{
				measure1();
				flag--;
			}
			measure();
			delay_ms(200);
		}	
//========================================================================电压电流测量
		if(mode % 4 == 2)
		{
			if(flag == 1)//只执行一次的
			{
				voltage1();
				flag--;
			}
		adcx=Get_Adc_Average(ADC_Channel_2,10);//PA2的ADC的AD值
		adca=Get_Adc_Average(ADC_Channel_3,10);//PA3的ADC的AD值
			
		OLED_ShowNum(37,0,adcx,4,16);	//显示ADC的值
		tempadc=(float)adcx*(3.3/4096);
		adcx=tempadc;
		OLED_ShowNum(65,2,adcx,1,16);	//显示电压值
		tempadc-=adcx;
		tempadc*=1000;
		OLED_ShowNum(75,2,tempadc,3,16);		

		OLED_ShowNum(37,6,adca,4,16);	//显示ADC的值
		tempadc=(float)adca*(3.3/4096);
		adca=tempadc;
		OLED_ShowNum(65,4,adca,1,16);	//显示电压值
		tempadc-=adca;
		tempadc*=1000;
		OLED_ShowNum(75,4,tempadc,3,16);
			
//			voltage();
			delay_ms(100);
		}	
//=========================================================================方波发生
			if(mode % 4 == 3)
		{
			if(flag == 1)//只执行一次的
			{
				OLED_Clear();
				flag--;				
			}
			OLED_DrawBMP(0,0,128,8,BMP1);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,BMP3);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF3);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF4);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF5);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF6);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF7);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF8);
			delay_ms(100);
			OLED_DrawBMP(0,0,128,8,GIF9);
			delay_ms(100);

		}
	}	  
}

