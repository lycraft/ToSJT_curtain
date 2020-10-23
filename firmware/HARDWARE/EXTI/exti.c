#include "exti.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"

extern int sa,sb,sc,sd;
extern uint8_t mode;
extern uint8_t flag;
extern uint8_t modetemp;
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //GPIOB7 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOA1	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOA0	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOB8	  中断线以及中断初始化配置 下降沿触发   WK_UP
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line9;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

//外部中断0服务程序 
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(WK_UP==1&&KEY2 == 0)	 	 //按键←
	{		
		mode = 4;
		flag = 1;
	}
		if(KEY2==1&&WK_UP == 0)	 	 //按键↑
	{		
		sb++;
		modetemp--;
	}
		EXTI_ClearITPendingBit(EXTI_Line7); //清除LINE0上的中断标志位  
		EXTI_ClearITPendingBit(EXTI_Line9); //清除LINE0上的中断标志位 
}
 
//外部中断3服务程序			→
void EXTI1_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1  
	{				 
		mode = modetemp-5;
		flag = 1;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除LINE3上的中断标志位  
}
//↓
void EXTI0_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0   
	{
		sd++;
		modetemp++;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE4上的中断标志位  
}
 
