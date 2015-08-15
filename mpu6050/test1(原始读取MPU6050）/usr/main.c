

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
void USART_Configuration(void);
void READ_MPU6050(void);
void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
void Delayms(vu32 m);
void USART1_Puts(unsigned char * str);
void delay5ms(void);
void  USART1_SendData(unsigned char SendData);

//#define GPIO_Pin_0 0;
//#define GPIO_Pin_1 1;
//#define GPIO_Pin_2 2;
//#define GPIO_Pin_3 3;
// 定义MPU6050内部地址
//****************************************

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1b	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x0c   	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)

extern short T_X,A_X,T_Y,A_Y,T_Z,A_Z,T_T;
int i;
unsigned char data[4]={'a','b','c','d'};
void DATA_printf(unsigned char *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;	  //将数字0～9转换为字符0～9
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}

 

int main(void)
{
  /* 配置系统时钟为72M */      
  SystemInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO  , ENABLE);  
  GPIO_Configuration();//init the iic gpio
  USART_Configuration();
	USART1_Puts("now i will put the data from IMU\n");
	Delayms(10);
	Init_MPU6050();
  
  while (1)
  {
      	READ_MPU6050();	         //读取MPU6050数据
        DATA_printf(data,T_X);//转换X轴数据到数组
	      USART1_Puts(data);			 //发送X轴数
		    delay5ms();
	      DATA_printf(data,T_Y);//转换Y轴数据到数组
	      USART1_Puts(data);			 //发送Y轴数
		    delay5ms();
	      DATA_printf(data,T_Z);//转换Z轴数据到数组
	      USART1_Puts(data);			 //发送Z轴数
		    delay5ms();
		
	      DATA_printf(data,A_X);//转换X轴数据到数组
	      USART1_Puts(data);			 //发送X轴数
		    delay5ms();
	      DATA_printf(data,A_Y);//转换Y轴数据到数组
	      USART1_Puts(data);			 //发送Y轴数
		    delay5ms();
	      DATA_printf(data,A_Z);//转换Z轴数据到数组
	      USART1_Puts(data);			 //发送Z轴数
		    delay5ms();
		    USART1_SendData(0X0D);	 //换行
	      USART1_SendData(0X0A);	 //回车
		    delay5ms();
	      
	}
}


void Delay(__IO uint32_t nCount)
{
for(; nCount != 0; nCount--);
}
 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}
void  USART1_SendData(unsigned char SendData)
{
  USART_SendData(USART1, SendData);
  Delayms(1);
}





