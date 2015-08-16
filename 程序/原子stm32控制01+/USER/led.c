/**************************** led.c ******************************************
//============================================================================//
//公司:苏州易寻传感网络科技有限公司
//功能:STM32+nRF24L01+(发送) 标签卡 
//日期:2013.01.31
//作者:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//修改:2011.12.29--*发射间隔改为1秒钟发一次卡信息*  
******************************************************************************/
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOA的外设时钟*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 

	/*选择要控制的GPIOA引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOA*/
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  




    	/*开启GPIOD的外设时钟*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE); 

	/*选择要控制的GPIOD引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOD*/
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	  

	/* 关闭所有led灯	*/
    GPIO_SetBits(GPIOA, GPIO_Pin_8); 
    GPIO_SetBits(GPIOD, GPIO_Pin_2);

	 
}

/***************************************majianghua*************************************************/

