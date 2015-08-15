
#include "i2c.h"
#include "stm32f10x.h"
#include  <math.h>    //Keil library  
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			  0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1b	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x0c   	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
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

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)



//#define SCL_H         GPIOB->BSRR = GPIO_Pin_7
//#define SCL_L         GPIOB->BRR  = GPIO_Pin_7 
//   
//#define SDA_H         GPIOB->BSRR = GPIO_Pin_6
//#define SDA_L         GPIOB->BRR  = GPIO_Pin_6

//#define SCL_read      GPIOB->IDR  & GPIO_Pin_7
//#define SDA_read      GPIOB->IDR  & GPIO_Pin_6

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7
#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
void SDA_IOIN(void);
void SDA_IOOUT(void);
void I2C_delay(void);
void delay5ms(void);
u8 I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte) ;
unsigned char I2C_RadeByte(void);
void READ_MPU6050(void);
u8 Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);


unsigned char BUF[15];   //�������ݻ�����
char TX_DATA[4];  	 //��ʾ�ݻ�����
short T_X,A_X,T_Y,A_Y,T_Z,A_Z,T_T;


void SDA_IOOUT(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;		  	// ��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	// ����������50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 	    // ѡ��C�˿�
}


void SDA_IOIN(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;			
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;  	// ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);				     	// ѡ��C�˿�
}

void Init_MPU6050()
{
	Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x80);
	delay5ms();
	Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);
  delay5ms();	//�������״̬
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
	 delay5ms();//��������Ϊ
	Single_Write(MPU6050_Addr,CONFIG, 0x06);
	 delay5ms();
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x08);
	 delay5ms(); //�����Ǽ�ⷶΧ500��/��
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x08);
	 delay5ms();  //��������ⷶΧ4G
};


u8 Single_Write(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data)		     //void
{
  	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//���ø���ʼ��ַ+������ַ 
  //  if(!I2C_WaitAck()){I2C_Stop(); return 0;}
	  I2C_WaitAck();	
    I2C_SendByte(REG_Address);   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return 1;
}

unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	  I2C_Start();
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    I2C_WaitAck();
	  I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
    I2C_WaitAck();
    I2C_Start();
		I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	  REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return 1;
	  return REG_data;

}



/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(void)
{
		
   u8 i=30; //��������Ż��ٶ�	����������͵�5����д��
   while(i) 
   { 
     i--; 
   }  
}

void delay5ms(void)
{
		
   u8 i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
u8 I2C_Start(void)
{
	SDA_IOOUT();
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return 0;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA_L;
	I2C_delay();
	if(SDA_read) return 0;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	SDA_L;
	I2C_delay();
	SCL_L;
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	SDA_IOOUT();
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	SDA_IOIN();
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
		  SDA_IOOUT();
	    I2C_delay();
      return 0;
	}
	SCL_L;
	SDA_IOOUT();
	I2C_delay();
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte( unsigned char SendByte) //���ݴӸ�λ����λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
 unsigned char I2C_RadeByte(void)  //���ݴӸ�λ����λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;
    SDA_IOIN();	
 			
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	    SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 



void READ_MPU6050(void)
{
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //��ȡ����X����ٶ�

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //��ȡ����Y����ٶ�
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //��ȡ����Z����ٶ�

   BUF[6]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
   A_X=	(BUF[7]<<8)|BUF[6];
   A_X/=8192.0; 						   //��ȡ����X����ٶ�
   
   BUF[8]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L); 
   BUF[9]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
   A_Y=	(BUF[9]<<8)|BUF[8];
   A_Y/=8192.0; 						   //��ȡ����Y����ٶ�
   
   BUF[10]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L); 
   BUF[11]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   A_Z=	(BUF[11]<<8)|BUF[10];
   A_Z/=8192.0; 						   //��ȡ����Z����ٶ�

   BUF[12]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[13]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[13]<<8)|BUF[12];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
}

