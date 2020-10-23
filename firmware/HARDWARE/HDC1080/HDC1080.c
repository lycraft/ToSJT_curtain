#include <stdint.h>   
#include "HDC1080.h"
#include "delay.h"
#include "iic.h" 
#include "stm32f10x.h"

void HDC1080_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)   
{   
    IIC_Start();   
    IIC_Send_Byte(HDC1080_Device_Adderss);   
    IIC_Wait_Ack();   
    IIC_Send_Byte(addr);   
    IIC_Wait_Ack();   
    while ( len-- )   
    {   
        IIC_Send_Byte(*buffer);   
        IIC_Wait_Ack();   
        buffer++;   
    }   
    IIC_Stop();   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_Buffer between triger and read there is no wait.  
  * @param  uint8_t addr is point register  
  * @param  uint8_t *buffer is the need to read data point  
  * @param  uint8_t len is the read data length  
  * @retval void  
 *******************************************************************************/   
void HDC1080_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)   
{   
    uint8_t temp = 0;   
       
    IIC_Start();   
    IIC_Send_Byte(HDC1080_Device_Adderss);   
   
    IIC_Wait_Ack();   
    IIC_Send_Byte(addr);   
    IIC_Wait_Ack();   
    IIC_Stop();   
       
    while(IIC_SCL)   
    {
        delay_us(20);   
    temp++;   
    if ( temp >= 10 )  //Ô­À´ÊÇ100 
    {   
        break;   
    }   
    }   
    IIC_Start();   
    IIC_Send_Byte(HDC1080_Device_Adderss|0x01);    //read   
    IIC_Wait_Ack();   
    while ( len-- )   
    {   
        *buffer = IIC_Read_Byte(0);   
        buffer++;   
        if ( len == 0 )   
            IIC_NAck();   
        else   
            IIC_Ack();        
           
    }   
    IIC_Stop();   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_MBuffer becareful between triger and read there is a wait.  
  * @param  uint8_t addr is point register  
  * @param  uint8_t *buffer is the need to read data point  
  * @param  uint8_t len is the read data length  
  * @retval void  
 *******************************************************************************/   
void HDC1080_Read_MBuffer(uint8_t addr, uint8_t *buffer, uint8_t len)   
{   
    uint8_t temp = 0;   
       
    IIC_Start();   
    IIC_Send_Byte(HDC1080_Device_Adderss);   
   
    IIC_Wait_Ack();   
    IIC_Send_Byte(addr);   
    IIC_Wait_Ack();   
    IIC_Stop();   
       
    while(IIC_SCL)
    {
        delay_ms(20);   
				temp++;   
				if ( temp >= 10 )  //Ô­À´ÊÇ100 
				{   
//						break; 
					IIC_SCL = 0;
				}   
    }   
    delay_ms(10);      // after triger should wait at least 6.5ms   
    IIC_Start();   
    IIC_Send_Byte(HDC1080_Device_Adderss|0x01);    //read   
    IIC_Wait_Ack();   
    while ( len-- )   
    {   
           *buffer = IIC_Read_Byte(1);   
        buffer++;   
        if ( len == 0 )   
            IIC_NAck();   
        else   
            IIC_Ack();      
           
    }   
    IIC_Stop();   
}   
/*******************************************************************************  
  * @brief  HDC1080_Soft_Reset   
  * @param  void  
  * @retval void  
 *******************************************************************************/   
void HDC1080_Soft_Reset(void)   
{   
    uint8_t temp[2];   
    temp[0] = 0x80;    
    temp[1] = 0x00;   
    HDC1080_Write_Buffer(HDC1080_Read_Config, temp, 2);   
    delay_ms(20);//there should be waiting for more than 15 ms.   
}   
/*******************************************************************************  
  * @brief  HDC1080_Setting   
  * @param  void  
  * @retval void  
 *******************************************************************************/   
void HDC1080_Setting(void)   
{   
    uint16_t tempcom = 0;   
    uint8_t temp[2];   
       
    tempcom |= 1<<HDC1080_Mode ;//???????????'??   
    // ?¶????¶???14bit   
    temp[0] = (uint8_t)(tempcom >> 8);    
    temp[1] = (uint8_t)tempcom;   
    HDC1080_Write_Buffer(0x02, temp, 2);   
}   
/*******************************************************************************  
  * @brief  HDC1080_Init   
  * @param  void  
  * @retval void  
 *******************************************************************************/   
void HDC1080_Init(void)   
{   
    uint8_t temp = 0;   
    IIC_Init();   
    HDC1080_Soft_Reset();   
    HDC1080_Setting();   
     
    while( IIC_SCL )  
    {  
        delay_us(20);;  
    temp++;  
    if ( temp >= 100 )  
    {  
            break;  
        }  
    }
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_Temper   
  * @param  void  
  * @retval int16_t tempr value  
 *******************************************************************************/   
int16_t HDC1080_Read_Temper(void)   
{   
    uint8_t buffer[2];
   
    HDC1080_Read_MBuffer(HDC1080_Read_Temperature, buffer, 2);   
//    return ((buffer[0]<<8)|buffer[1]);   
//		return (int16_t)((((buffer[0]<<8)|buffer[1])/65536.0)*165-40);   
    return (int16_t)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_Humidity   
  * @param  void  
  * @retval int16_t Humidity value  
 *******************************************************************************/   
int16_t HDC1080_Read_Humidi(void)   
{   
    uint8_t buffer[2];   
       
    HDC1080_Read_MBuffer(HDC1080_Read_Humidity, buffer, 2);   
   
//    return (buffer[0]<<8)|buffer[1];   
    return (int16_t)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_Configuration   
  * @param  void  
  * @retval Config value  
 *******************************************************************************/   
uint16_t HDC1080_Read_Conf(void)   
{   
    uint8_t buffer[2];   
       
    HDC1080_Read_Buffer(HDC1080_Read_Config, buffer, 2);   
   
    return ((buffer[0]<<8)|buffer[1]);   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_ManufacturerID   
  * @param  void  
  * @retval ManufacturerID   
 *******************************************************************************/   
uint16_t HDC1080_Read_ManufacturerID(void)   
{   
    uint8_t buffer[2];   
       
    HDC1080_Read_Buffer(0xfe, buffer, 2);   
   
    return ((buffer[0]<<8)|buffer[1]);   
}   
/*******************************************************************************  
  * @brief  HDC1080_Read_DeviceID  
  * @param  void  
  * @retval DeviceID  
 *******************************************************************************/   
uint16_t HDC1080_Read_DeviceID(void)   
{   
    uint8_t buffer[2];   
       
    HDC1080_Read_Buffer(0xff, buffer, 2);   
   
    return ((buffer[0]<<8)|buffer[1]);   
}

