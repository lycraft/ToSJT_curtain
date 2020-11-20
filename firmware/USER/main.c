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

int CountMotor = 0;
uint8_t mode = 0, MotorDirFlag = 0;//
uint8_t flag = 1;
uint16_t LightLine = 2000;

void meau()//mode0��ˢ��
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
�����ת
******************************************************************/
void Motor_Forward()
{
	switch(CountMotor % 8)//��ʼ�����ת
			{
				case 0:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //��������					
				}

					;break;
				case 1:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //��������		
				}
					;break;
				case 2:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //��������		
				}
					;break;
				case 3:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //��������		
				}
					;break;
				case 4:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //��������		
				}
					;break;
				case 5:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //��������		
				}
					;break;
				case 6:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //��������		
				}
					;break;
				case 7:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //��������		
				}
					;break;
			}
}

/******************************************************************
�����ת
******************************************************************/
void Motor_Back()
{
	switch(CountMotor % 8)//��ʼ�����ת
			{
				case 7:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //��������					
				}

					;break;
				case 6:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4); //��������		
				}
					;break;
				case 5:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //��������		
				}
					;break;
				case 4:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_5); //��������		
				}
					;break;
				case 3:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_6); //��������		
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
				case 1:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //��������		
				}
					;break;
				case 0:
				{
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
					GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_4);//��������
					GPIO_SetBits(GPIOA,GPIO_Pin_7); //��������		
				}
					;break;
			}
}

void Motor_Stop()
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//��������
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);//��������
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);//��������
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//��������		
}

int main(void)
 {	
		vu8 key=0;
	   u16 adcx;
//		float tempadc;
	 
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	
		OLED_Init();			//��ʼ��OLED  
	  HDC1080_Init();
//		KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
		EXTIX_Init();					//������ʼ��
	  Adc_Init();		  		//ADC��ʼ��
		LED_Init();
		TIM3_Int_Init(10 - 1,7200 - 1);	 //new 72000000/7200 = 10khzƵ��		������10����1ms�жϷ���������һ��
	 
	 meau();
	while(1) 
	{		
		//���ADC��ֵ(����ǿ��)����ʾ����Ļ��
		if(CountMotor == 100)
		{
			adcx=Get_Adc_Average(ADC_Channel_2,10);//PA2��ADC��ADֵ
				
			OLED_ShowNum(65,2,adcx,4,16);	//��ʾADC��ֵ
		}


/*******************************************************************
�Զ�ģʽ���ֶ�ģʽ		
*******************************************************************/
			if(mode % 2 == 0)				//�Զ�ģʽ
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				OLED_ShowString(65,0,"Auto");
				flag--;				
			}
			if(adcx >= LightLine)//����ֵ����2000�����Ϳ�ʼ����
			{
				Motor_Forward();				//�����ת
			}
			else if(adcx < LightLine)
			{
				Motor_Stop();
			}
			if(CountMotor == 100)
			{
				OLED_ShowNum(65,4,LightLine,4,16);	//��ʾADC��ֵ
			}
			
		}
/******************************************************************/		
			if(mode % 2 == 1)				//�ֶ�ģʽ
		{
			if(flag == 1)//ִֻ��һ�ε�
			{
				OLED_ShowString(65,0,"Pres");
				flag--;				
			}
			if(MotorDirFlag == 0)
			{
				Motor_Forward();			//�����ת
			}
			if(MotorDirFlag == 1)
			{
				Motor_Back();					//�����ת
			}
			if(MotorDirFlag == 2)
			{
				Motor_Stop();					//���ͣת
			}
				
		}
/*******************************************************************/		
	}	  
}

