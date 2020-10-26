//////////////////////////////////////////////////////////////////////////////////	 
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PA8��SCL��
//              D1   ��PB15��SDA��
//              RES  ��PB14
//              DC   ��PB13
//              CS   ��PA3               
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

int sa=0,sb=0,sc=0,sd=0;
int wendu,shidu;
int CountMotor = 0;
uint8_t modetemp = 6,mode = 4;//modetemp��ָʾλ�� ָ��ѡ�е�mode��==============mode0-4
uint8_t flag = 1;

void measure1()//��Ļ��ˢ�µĶ��� 
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
	
		OLED_ShowCHinese(0,2,7);//�¶�
		OLED_ShowCHinese(18,2,8);//
		OLED_ShowCHinese(0,4,9);//ʪ��
		OLED_ShowCHinese(18,4,8);//
		OLED_ShowChar(37,2,':');
		OLED_ShowChar(37,4,':');
		OLED_ShowChar(71,2,'.');
		OLED_ShowChar(71,4,'.');
		OLED_ShowChar(90,2,'C');
		OLED_ShowChar(90,4,'%');
	 
	 	OLED_ShowCHinese(0,6,0);
		OLED_ShowCHinese(18,6,1);
		OLED_ShowCHinese(36,6,2);//��
		OLED_ShowCHinese(54,6,3);//��
		OLED_ShowCHinese(72,6,4);//��
		OLED_ShowCHinese(90,6,5);//��
		OLED_ShowCHinese(108,6,6);//Ļ
	
		OLED_ShowNum(0,0,mode,2,16);
}
void measure()//��Ļ��ʱ��ˢ�µĶ���
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
void meau()//mode0��ˢ��
{
		OLED_Clear();
		OLED_DrawBMP(96,0,128,2,BMP2);	
		OLED_ShowString(0,6,"Meau");
		OLED_ShowString(40,0,"Measure");
		OLED_ShowString(40,2,"Voltage");
		OLED_ShowString(40,4,"Animation");
		OLED_ShowString(72,6,"Lycraft");
}
void voltage()//������ѹ����ˢ�µ�
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
void voltage1()//������ѹ������ˢ�µ�
{
		OLED_Clear();
		OLED_ShowCHinese(0,2,10);//��
		OLED_ShowCHinese(18,2,11);//ѹ
		OLED_ShowCHinese(0,4,10);//��
		OLED_ShowCHinese(18,4,12);//��
		
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
	 
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
		OLED_Init();			//��ʼ��OLED  
	  HDC1080_Init();
//		KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
		EXTIX_Init();					//������ʼ��
	  Adc_Init();		  		//ADC��ʼ��
		LED_Init();
		TIM3_Int_Init(100 - 1,7200 - 1);	 //new 72000000/7200 = 10khzƵ��		������100����10ms�жϷ���������һ��
	 
	while(1) 
	{		
		if(modetemp == 3||modetemp == 9)//======��ͷ��־λ
		{
			modetemp = 6;
		}
//========================================================================Meau
		if(mode % 4 == 0)
		{
			if(flag == 1)//ִֻ��һ�ε�
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
			if(flag == 1)//ִֻ��һ�ε�
			{
				measure1();
				flag--;
			}
			measure();
			delay_ms(200);
		}	
//========================================================================��ѹ��������
		if(mode % 4 == 2)
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				voltage1();
				flag--;
			}
		adcx=Get_Adc_Average(ADC_Channel_2,10);//PA2��ADC��ADֵ
		adca=Get_Adc_Average(ADC_Channel_3,10);//PA3��ADC��ADֵ
			
		OLED_ShowNum(37,0,adcx,4,16);	//��ʾADC��ֵ
		tempadc=(float)adcx*(3.3/4096);
		adcx=tempadc;
		OLED_ShowNum(65,2,adcx,1,16);	//��ʾ��ѹֵ
		tempadc-=adcx;
		tempadc*=1000;
		OLED_ShowNum(75,2,tempadc,3,16);		

		OLED_ShowNum(37,6,adca,4,16);	//��ʾADC��ֵ
		tempadc=(float)adca*(3.3/4096);
		adca=tempadc;
		OLED_ShowNum(65,4,adca,1,16);	//��ʾ��ѹֵ
		tempadc-=adca;
		tempadc*=1000;
		OLED_ShowNum(75,4,tempadc,3,16);
			
//			voltage();
			delay_ms(100);
		}	
//=========================================================================��������
			if(mode % 4 == 3)
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				OLED_Clear();
				flag--;				
			}
			OLED_ShowNum(65,2,CountMotor,3,16);	//��ʾ����ֵ
			switch(CountMotor % 4)//��ʼ�����ת
			{
				case 0:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //��������					
				}

					;break;
				case 1:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //��������		
				}
					;break;
				case 2:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //��������		
				}
					;break;
				case 3:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //��������		
				}
					;break;
			}

			
		}
	}	  
}

