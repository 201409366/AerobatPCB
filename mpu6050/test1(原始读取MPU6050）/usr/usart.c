#include "usart.h"
#include "stm32f10x.h"

void USART_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	USART_ClockInitTypeDef  USART_ClockInitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 |RCC_APB2Periph_USART1, ENABLE  );
	/* A9 USART1_Tx */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/* A10 USART1_Rx */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
  
  USART_ClockInit(USART1, &USART_ClockInitStructure);					// 时钟参数初始化设置
	*/
	
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	/* Enable the USARTx */
	  /* Enable USART1 */
//  USART_ClearFlag(USART1, USART_IT_RXNE); 			//清中断，以免一启用中断后立即产生中断//
//  USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//使能USART1中断源
  USART_Cmd(USART1, ENABLE);
	
}

void USART1_Puts(unsigned char * str)
{
while(*str)
{
  USART_SendData(USART1, *str++);
  /* Loop until the end of transmission */
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
}
