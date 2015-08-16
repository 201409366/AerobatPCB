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

	/* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();
 		
	/* ����SysTick Ϊ10us�ж�һ�� */
    SysTick_Init();

	Delay_us(5000);    	// 50000 * 10us = 500ms



	while(1)
	{
		LED2( ON );		  // ��
		Delay_us(50000);    	// 50000 * 10us = 500ms		
		LED2( OFF );			  // ��
		Delay_us(50000);    	// 50000 * 10us = 500ms
	} 


}


/***************************************majianghua*************************************************/


