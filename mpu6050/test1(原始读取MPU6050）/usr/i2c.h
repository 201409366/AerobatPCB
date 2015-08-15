#include "stm32f10x.h"

void SDA_IOIN(void);
void SDA_IOOUT(void);
void Init_MPU6050(void);
void READ_MPU6050(void);
void I2C_delay(void);
void delay5ms(void);
u8 I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte) ;
unsigned char I2C_RadeByte(void);

u8 Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);

