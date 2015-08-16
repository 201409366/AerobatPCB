/**************************** spi.c ******************************************
//============================================================================//
//��˾:������Ѱ��������Ƽ����޹�˾
//����:STM32+nRF24L01+(����) ��ǩ�� 
//����:2013.01.31
//����:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//�޸�:2011.12.29--*��������Ϊ1���ӷ�һ�ο���Ϣ*  
******************************************************************************/
#include "spi.h"
/*
 * ��������SPI_RF24L01_Init
 * ����  ��SPI�� I/O����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void SPI_RF24L01_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
 /*ʹ��GPIOA,GPIOC,���ù���ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

 /*ʹ��SPI1ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

//   /*����485оƬ�Ŀ�������GPIOA^0����ֹ����RF24L01*/
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
//  GPIO_Init(GPIOA, &GPIO_InitStructure);  
//  GPIO_ResetBits(GPIOA, GPIO_Pin_0);               //��ֹ485�ķ���ģʽ������ģʽ�Կ�

   /*���� SPI_RF24L01_SPI�� SCK,MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //���ù���
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /*����SPI_RF24L01_SPI��CE���ţ�GPIOB^1*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*����SPI_RF24L01_SPI�� CSN ����: NSS GPIOA^4*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
		  

   /*����SPI_RF24L01_SPI��IRQ���ţ�GPIOB^0*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
		  
  /* �����Զ���ĺ꣬��������csn���ţ�RF24L01�������״̬ */
//  RF24L01_CSN_HIGH(); 
 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//���ݴ�С8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 				//ʱ�Ӽ��ԣ�����ʱΪ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//��1��������Ч��������Ϊ����ʱ��
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS�ź����������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8��Ƶ��9MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//��λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);

}

/***************************************majianghua*************************************************/

