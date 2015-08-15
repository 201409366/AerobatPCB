/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：I2C EEPROM(AT24C02)测试，测试信息通过USART1打印在电脑的超级终端。
 *          
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.0.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/	
#include "stm32f10x.h"
#include "I2C_MPU6050.h"
#include "usart1.h"
#include "delay.h"



/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
int main(void)
{	


	/* 串口1初始化 */
	USART1_Config();	 
	printf("This is a MPU6050 demo !");
	/*IIC接口初始化*/
	I2C_MPU6050_Init(); 	 
	/*陀螺仪传感器初始化*/
  InitMPU6050();
	
	/***********************************************************************/
	
	while(1)
	{
		printf("\r\n---------加速度X轴原始数据---------%d \r\n",GetData(ACCEL_XOUT_H));
		printf("\r\n---------加速度Y轴原始数据---------%d \r\n",GetData(ACCEL_YOUT_H));	
		printf("\r\n---------加速度Z轴原始数据---------%d \r\n",GetData(ACCEL_ZOUT_H));	
		printf("\r\n---------陀螺仪X轴原始数据---------%d \r\n",GetData(GYRO_XOUT_H));	
		printf("\r\n---------陀螺仪Y轴原始数据---------%d \r\n",GetData(GYRO_YOUT_H));	
		printf("\r\n---------陀螺仪Z轴原始数据---------%d \r\n",GetData(GYRO_ZOUT_H));
		delay_ms(500);
	}		

 
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
