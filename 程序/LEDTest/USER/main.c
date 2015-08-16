/**************************** main.c ******************************************
//============================================================================//
//公司:苏州易寻传感网络科技有限公司
//功能:STM32+nRF24L01+(发送) 标签卡 
//日期:2013.01.31
//作者:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//修改:2011.12.29--*发射间隔改为1秒钟发一次卡信息*  
******************************************************************************/
#include "stm32f10x.h"
#include "nRF24L01.h"
#include "usart1.h"
#include "spi.h"
#include "rtc.h"
#include "config.h"
#include "SysTick.h"
#include "led.h"



u8 status;	//用于判断接收/发送状态
u8 txbuf[TX_PLOAD_WIDTH]="01234567890123456789012345678901";	 //发送缓冲
u8 rxbuf[RX_PLOAD_WIDTH+1];			 //接收缓冲
int i=0;


void Delay(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
} 

uint32_t ticktime;




	/* 主函数进口 */
int main(void)
{

	/* LED 端口初始化 */
    LED_GPIO_Config();
 		
	/* 配置SysTick 为10us中断一次 */
    SysTick_Init();

	Delay_us(5000);    	// 50000 * 10us = 500ms



	while(1)
	{
		LED2( ON );		  // 灭
		Delay_us(50000);    	// 50000 * 10us = 500ms		
		LED2( OFF );			  // 亮
		Delay_us(50000);    	// 50000 * 10us = 500ms
	} 


}


/***************************************majianghua*************************************************/


