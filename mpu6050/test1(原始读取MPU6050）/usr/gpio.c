#include "gpio.h"
#include "stm32f10x.h"



void ledon(int x)
{
	switch(x)
	{
		case 0:
			GPIO_ResetBits(GPIOC,GPIO_Pin_0); break;
		case 1:
			GPIO_ResetBits(GPIOC,GPIO_Pin_1); break;
		case 2:
			GPIO_ResetBits(GPIOC,GPIO_Pin_2); break;
		case 3:
			GPIO_ResetBits(GPIOC,GPIO_Pin_3); break;
	}
};

void ledoff(int x)
{
		switch(x)
	{
		case 0:
			GPIO_SetBits(GPIOC,GPIO_Pin_0); break;
		case 1:
			GPIO_SetBits(GPIOC,GPIO_Pin_1); break;
		case 2:
			GPIO_SetBits(GPIOC,GPIO_Pin_2); break;
		case 3:
			GPIO_SetBits(GPIOC,GPIO_Pin_3); break;
	}
};
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE  );
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

