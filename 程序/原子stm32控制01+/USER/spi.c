/**************************** spi.c ******************************************
//============================================================================//
//公司:苏州易寻传感网络科技有限公司
//功能:STM32+nRF24L01+(发送) 标签卡 
//日期:2013.01.31
//作者:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//修改:2011.12.29--*发射间隔改为1秒钟发一次卡信息*  
******************************************************************************/
#include "spi.h"
/*
 * 函数名：SPI_RF24L01_Init
 * 描述  ：SPI的 I/O配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void SPI_RF24L01_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
 /*使能GPIOA,GPIOC,复用功能时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);

 /*使能SPI1时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

//   /*配置485芯片的控制引脚GPIOA^0，防止干扰RF24L01*/
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);  
//  GPIO_ResetBits(GPIOA, GPIO_Pin_0);               //禁止485的发送模式，接收模式仍开

   /*配置 SPI_RF24L01_SPI的 SCK,MISO,MOSI引脚，GPIOA^5,GPIOA^6,GPIOA^7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  /*配置SPI_RF24L01_SPI的CE引脚，GPIOA^4*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*配置SPI_RF24L01_SPI的 CSN 引脚: NSS GPIOC^4*/
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
		  

   /*配置SPI_RF24L01_SPI的IRQ引脚，GPIOC^5*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  //上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
		  
  /* 这是自定义的宏，用于拉高csn引脚，RF24L01进入空闲状态 */
//  RF24L01_CSN_HIGH(); 
 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 					//主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 				//数据大小8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 				//时钟极性，空闲时为低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//第1个边沿有效，上升沿为采样时刻
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   					//NSS信号由软件产生
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //8分频，9MHz
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  				//高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);

}

/***************************************majianghua*************************************************/

