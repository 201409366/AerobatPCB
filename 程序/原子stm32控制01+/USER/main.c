/**************************** main.c ******************************************
//============================================================================//
//��˾:������Ѱ��������Ƽ����޹�˾
//����:STM32+nRF24L01+(����) ��ǩ�� 
//����:2013.01.31
//����:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//�޸�:2011.12.29--*��������Ϊ1���ӷ�һ�ο���Ϣ*  
******************************************************************************/
#include "stm32f10x.h"
#include "nRF24L01.h"
#include "usart1.h"
#include "spi.h"
#include "rtc.h"
#include "config.h"
#include "SysTick.h"
#include "led.h"



u8 status;	//�����жϽ���/����״̬
u8 txbuf[TX_PLOAD_WIDTH]="01234567890123456789012345678901";	 //���ͻ���
u8 rxbuf[RX_PLOAD_WIDTH+1];			 //���ջ���
int i=0;


void Delay(__IO u32 nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
} 

uint32_t ticktime;




	/* ���������� */
int main(void)
{
	/* RTC Configuration */
	RTC_Configuration();


	/* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();
   
    SPI_RF24L01_Init();

	RF24L01_Init();
   
    /* ����1��ʼ�� */
	USART1_Config();

		
	/* ����SysTick Ϊ10us�ж�һ�� */
    SysTick_Init();

	Delay_us(5000);    	// 50000 * 10us = 500ms

	printf("\r\n ����һ�� RF24L0124L01 ���ߴ���ʵ�� \r\n");
   	printf("\r\n �������ߴ��� ������ �ķ�����Ϣ\r\n");
	printf("\r\n   ���ڼ��RF24L01��MCU�Ƿ��������ӡ�����\r\n");

	 /*���RF24L01ģ����MCU������*/
   	status = RF24L01_Check(); 

	/*�ж�����״̬*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      RF24L01��MCU���ӳɹ���\r\n");  

   else	  
   	     printf("\r\n  RF24L01��MCU����ʧ�ܣ������¼����ߡ�\r\n");

	LED1( OFF );		  // ��
	 
	while(1)
	{
		RF24L01_TX_Mode();

		/*��ʼ��������*/	
		status = RF24L01_Tx_Dat(txbuf);	  

		/*�жϷ���״̬*/
		switch(status)
		{
			case MAX_RT:
			printf("\r\n ������ û���յ�Ӧ���źţ����ʹ��������޶�ֵ������ʧ�ܡ� \r\n");
			break;

			case ERROR:
			printf("\r\n δ֪ԭ���·���ʧ�ܡ� \r\n");
			break;

			case TX_DS:
			printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	 		
			break;  								
		}			   	       

		LED1( ON );			  // ��
		Delay_us(50000);    	// 50000 * 10us = 500ms
		LED1( OFF );		  // ��

		LED2( ON );			  // ��
		Delay_us(50000);    	// 50000 * 10us = 500ms
		LED2( OFF );		  // ��
		Delay_us(50000);    	// 50000 * 10us = 500ms
	} 


//	while(1)
//	{
	
//		RF24L01_RX_Mode();

//		/*��ʼ��������*/	
//		status = RF24l01_Rx_Dat(rxbuf);	  

//		/*�жϷ���״̬*/
//		switch(status)
//		{
//			case ERROR:
//				printf("\r\n δ֪ԭ���½���ʧ�ܡ� \r\n");
//				break;

//			case RX_DR:
//				//printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	
//				rxbuf[32] = 0;
//				printf("\r\n Receive data is:%s \r\n",rxbuf);//��ӡ���յ�������	
//				LED1( ON );			  // ��			
//				break;  								
//		}			   	       


//		LED2( ON );			  // ��
//		Delay_us(50000);    	// 50000 * 10us = 500ms
//		LED2( OFF );		  // ��
//		Delay_us(50000);    	// 50000 * 10us = 500ms
//	} 


}


/***************************************majianghua*************************************************/


