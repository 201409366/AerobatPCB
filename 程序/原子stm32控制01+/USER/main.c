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
	/* RTC Configuration */
	RTC_Configuration();


	/* LED 端口初始化 */
    LED_GPIO_Config();
   
    SPI_RF24L01_Init();

	RF24L01_Init();
   
    /* 串口1初始化 */
	USART1_Config();

		
	/* 配置SysTick 为10us中断一次 */
    SysTick_Init();

	Delay_us(5000);    	// 50000 * 10us = 500ms

	printf("\r\n 这是一个 RF24L0124L01 无线传输实验 \r\n");
   	printf("\r\n 这是无线传输 主机端 的反馈信息\r\n");
	printf("\r\n   正在检测RF24L01与MCU是否正常连接。。。\r\n");

	 /*检测RF24L01模块与MCU的连接*/
   	status = RF24L01_Check(); 

	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      RF24L01与MCU连接成功！\r\n");  

   else	  
   	     printf("\r\n  RF24L01与MCU连接失败，请重新检查接线。\r\n");

	LED1( OFF );		  // 灭
	 
	while(1)
	{
		RF24L01_TX_Mode();

		/*开始发送数据*/	
		status = RF24L01_Tx_Dat(txbuf);	  

		/*判断发送状态*/
		switch(status)
		{
			case MAX_RT:
			printf("\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
			break;

			case ERROR:
			printf("\r\n 未知原因导致发送失败。 \r\n");
			break;

			case TX_DS:
			printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	 		
			break;  								
		}			   	       

		LED1( ON );			  // 亮
		Delay_us(50000);    	// 50000 * 10us = 500ms
		LED1( OFF );		  // 灭

		LED2( ON );			  // 亮
		Delay_us(50000);    	// 50000 * 10us = 500ms
		LED2( OFF );		  // 灭
		Delay_us(50000);    	// 50000 * 10us = 500ms
	} 


//	while(1)
//	{
	
//		RF24L01_RX_Mode();

//		/*开始发送数据*/	
//		status = RF24l01_Rx_Dat(rxbuf);	  

//		/*判断发送状态*/
//		switch(status)
//		{
//			case ERROR:
//				printf("\r\n 未知原因导致接收失败。 \r\n");
//				break;

//			case RX_DR:
//				//printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	
//				rxbuf[32] = 0;
//				printf("\r\n Receive data is:%s \r\n",rxbuf);//打印接收到的数据	
//				LED1( ON );			  // 亮			
//				break;  								
//		}			   	       


//		LED2( ON );			  // 亮
//		Delay_us(50000);    	// 50000 * 10us = 500ms
//		LED2( OFF );		  // 灭
//		Delay_us(50000);    	// 50000 * 10us = 500ms
//	} 


}


/***************************************majianghua*************************************************/


