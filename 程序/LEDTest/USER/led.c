/**************************** led.c ******************************************
//============================================================================//
//��˾:������Ѱ��������Ƽ����޹�˾
//����:STM32+nRF24L01+(����) ��ǩ�� 
//����:2013.01.31
//����:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//�޸�:2011.12.29--*��������Ϊ1���ӷ�һ�ο���Ϣ*  
******************************************************************************/
#include "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOA������ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIOA����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOA*/
  	GPIO_Init(GPIOC, &GPIO_InitStructure);


	 
}

/***************************************majianghua*************************************************/

