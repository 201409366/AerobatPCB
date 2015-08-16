/**************************** nRF24L01.c *************************************
//============================================================================//
//��˾:������Ѱ��������Ƽ����޹�˾
//����:STM32+nRF24L01+(����) ��ǩ�� 
//����:2013.01.31
//����:ZYX_majianghua
//E-mail:majianghua735366@yeah.net
//�޸�:2011.12.29--*��������Ϊ1���ӷ�һ�ο���Ϣ*  
******************************************************************************
 * Ӳ�����ӣ�---------------------------------|
 *          |                		    	  |
 *          |  	PC4            : RF24L01- CSN |
			|	PA5-SPI1-SCK   : RF24L01 -SCK |
			|	PA6-SPI1-MISO  : RF24L01 -MISO|
			|	PA7-SPI1-MOSI  : RF24L01 -MOSI|
			|	PC5		   	   : RF24L01-IRQ  |
			|	PA4		   	   : RF24L01-CE   |
 *          |                	     		  |
 *           ----------------------------------

**********************************************************************************/
#include "nRF24L01.h"
#include "usart1.h"
#include "spi.h"
#include "config.h"

 u8 RX_BUF[RX_PLOAD_WIDTH+1];		//�������ݻ���
 u8 TX_BUF[TX_PLOAD_WIDTH+1];		//�������ݻ���
 u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};;  // ����һ����̬���͵�ַ
 u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};;



/*
 * ��������SPI_RF24L01_RW
 * ����  ��������RF24L01��/дһ�ֽ�����
 * ����  ��д�������
 * ���  ����ȡ�õ�����
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_RW(u8 dat)
{  	
   /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  
   /* ͨ�� SPI2����һ�ֽ����� */
  SPI_I2S_SendData(SPI1, dat);		
 
   /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*
 * ��������SPI_RF24L01_WriteReg
 * ����  ��������RF24L01�ض��ļĴ���д������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   dat:��Ҫ��Ĵ���д�������
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_WriteReg(u8 reg,u8 dat)
{
 	u8 status;
	 RF24L01_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
    RF24L01_CSN_LOW();
				
	/*��������Ĵ����� */
	status = SPI_RF24L01_RW(reg);
		 
	 /*��Ĵ���д������*/
    SPI_RF24L01_RW(dat); 
	          
	/*CSN���ߣ����*/	   
  	RF24L01_CSN_HIGH();	
		
	/*����״̬�Ĵ�����ֵ*/
   	return(status);
}


/*
 * ��������SPI_RF24L01_ReadReg
 * ����  �����ڴ�RF24L01�ض��ļĴ�����������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 * ���  ���Ĵ����е�����
 * ����  ���ڲ�����
 */
u8 SPI_RF24L01_ReadReg(u8 reg)
{
 	u8 reg_val;

	RF24L01_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
 	RF24L01_CSN_LOW();
				
  	 /*���ͼĴ�����*/
	SPI_RF24L01_RW(reg); 

	 /*��ȡ�Ĵ�����ֵ */
	reg_val = SPI_RF24L01_RW(NOP);
	            
   	/*CSN���ߣ����*/
	RF24L01_CSN_HIGH();		
   	
	return reg_val;
}	


/*
 * ��������SPI_RF24L01_ReadBuf
 * ����  �����ڴ�RF24L01�ļĴ����ж���һ������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   pBuf�����ڴ洢���������ļĴ������ݵ����飬�ⲿ����
		   bytes: pBuf�����ݳ���	
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_RF24L01_ReadBuf(u8 reg,u8 *pBuf,u8 bytes)
{
 	u8 status, byte_cnt;

	  RF24L01_CE_LOW();
	/*�õ�CSN��ʹ��SPI����*/
	RF24L01_CSN_LOW();
		
	/*���ͼĴ�����*/		
	status = SPI_RF24L01_RW(reg); 

 	/*��ȡ����������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_RF24L01_RW(NOP); //��RF24L01��ȡ����  

	 /*CSN���ߣ����*/
	RF24L01_CSN_HIGH();	
		
 	return status;		//���ؼĴ���״ֵ̬
}



/*
 * ��������SPI_RF24L01_WriteBuf
 * ����  ��������RF24L01�ļĴ�����д��һ������
 * ����  ��reg:RF24L01������+�Ĵ�����ַ��
 		   pBuf���洢�˽�Ҫд��д�Ĵ������ݵ����飬�ⲿ����
		   bytes: pBuf�����ݳ���	
 * ���  ��RF24L01��status�Ĵ�����״̬
 * ����  ���ⲿ����
 */
u8 SPI_RF24L01_WriteBuf(u8 reg ,u8 *pBuf,u8 bytes)
{
	 u8 status,byte_cnt,temp;
	 RF24L01_CE_LOW();
   	 /*�õ�CSN��ʹ��SPI����*/
	 RF24L01_CSN_LOW();			

	 /*���ͼĴ�����*/	
  	 status = SPI_RF24L01_RW(reg); 
 	
  	  /*�򻺳���д������*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		temp = SPI_RF24L01_RW(*pBuf++);	//д���ݵ������� 	 
	  	   
	/*CSN���ߣ����*/
	RF24L01_CSN_HIGH();			
  
  	return (status);	//����RF24L01��״̬ 		
}


void RF24L01_Init(void)
{
   RF24L01_CSN_HIGH();

}
   

/*
 * ��������void RF24L01_Init(void)_RX_Mode
 * ����  �����ò��������ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void RF24L01_RX_Mode(void)

{
	RF24L01_CE_LOW();	

   SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_AA,0x00);    //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ    

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_CH,CHANAL);      //����RFͨ��Ƶ��    

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��      

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_SETUP,0x07); //����TX�������,0db����,1Mbps,���������濪��   

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+CONFIG, 0x0f);  //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 

/*CE���ߣ��������ģʽ*/	
  RF24L01_CE_HIGH();

}    

/*
 * ��������void RF24L01_Init(void)_TX_Mode
 * ����  �����÷���ģʽ
 * ����  ����	
 * ���  ����
 * ����  ���ⲿ����
 */
void RF24L01_TX_Mode(void)
{  
	RF24L01_CE_LOW();		

   SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //дTX�ڵ��ַ 

   SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK   

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_CH,CHANAL);       //����RFͨ��ΪCHANAL

   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+RF_SETUP,0x07);  //����TX�������,0db����,2Mbps,���������濪��   
	
   SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�

/*CE���ߣ����뷢��ģʽ*/	
  RF24L01_CE_HIGH();
    Delay(0xffff); //CEҪ����һ��ʱ��Ž��뷢��ģʽ
}



/*
 * ��������void RF24L01_Init(void)_Check
 * ����  ����Ҫ����void RF24L01_Init(void)��MCU�Ƿ���������
 * ����  ����	
 * ���  ��SUCCESS/ERROR ��������/����ʧ��
 * ����  ���ⲿ����
 */
u8 RF24L01_Check(void)
{
	u8 buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	u8 buf1[5];
	u8 i; 
	 
	/*д��5���ֽڵĵ�ַ.  */  
	SPI_RF24L01_WriteBuf(RF24L01_WRITE_REG+TX_ADDR,buf,5);

	/*����д��ĵ�ַ */
	SPI_RF24L01_ReadBuf(TX_ADDR,buf1,5); 
	 
	/*�Ƚ�*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU��void RF24L01_Init(void)�ɹ����� 
	else
		return ERROR ;        //MCU��void RF24L01_Init(void)����������
}


 /*
 * ��������void RF24L01_Init(void)_Tx_Dat
 * ����  ��������void RF24L01_Init(void)�ķ��ͻ�������д������
 * ����  ��txBuf���洢�˽�Ҫ���͵����ݵ����飬�ⲿ����	
 * ���  �����ͽ�����ɹ�����TXDS,ʧ�ܷ���MAXRT��ERROR
 * ����  ���ⲿ����
 */ 
u8 RF24L01_Tx_Dat(u8 *txbuf)
{
	u8 state;  

	 /*ceΪ�ͣ��������ģʽ1*/
	RF24L01_CE_LOW();

	/*д���ݵ�TX BUF ��� 32���ֽ�*/						
   SPI_RF24L01_WriteBuf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);

      /*CEΪ�ߣ�txbuf�ǿգ��������ݰ� */   
 	 RF24L01_CE_HIGH();
	  	
	  /*�ȴ���������ж� */                            
	while(RF24L01_Read_IRQ()!=0); 	
	
	/*��ȡ״̬�Ĵ�����ֵ */                              
	state = SPI_RF24L01_ReadReg(STATUS);

	 /*���TX_DS��MAX_RT�жϱ�־*/                  
	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+STATUS,state); 	

	SPI_RF24L01_WriteReg(FLUSH_TX,NOP);    //���TX FIFO�Ĵ��� 

	 /*�ж��ж�����*/    
	if(state&MAX_RT)                     //�ﵽ����ط�����
			 return MAX_RT; 

	else if(state&TX_DS)                  //�������
		 	return TX_DS;
	 else						  
			return ERROR;                 //����ԭ����ʧ��
} 


 /*
 * ��������void RF24L01_Init(void)_Rx_Dat
 * ����  �����ڴ�void RF24L01_Init(void)�Ľ��ջ������ж�������
 * ����  ��rxBuf�����ڽ��ո����ݵ����飬�ⲿ����	
 * ���  �����ս����
 * ����  ���ⲿ����
 */ 
u8 RF24l01_Rx_Dat(u8 *rxbuf)
{
	u8 state; 
	RF24L01_CE_HIGH();	 //�������״̬
	 /*�ȴ������ж�*/
	while(RF24L01_Read_IRQ()!=0); 
	
	RF24L01_CE_LOW();  	 //�������״̬
	/*��ȡstatus�Ĵ�����ֵ  */               
	state=SPI_RF24L01_ReadReg(STATUS);
	 
	/* ����жϱ�־*/      
	SPI_RF24L01_WriteReg(RF24L01_WRITE_REG+STATUS,state);

	/*�ж��Ƿ���յ�����*/
	if(state&RX_DR)                                 //���յ�����
	{
	  SPI_RF24L01_ReadBuf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
	     SPI_RF24L01_WriteReg(FLUSH_RX,NOP);          //���RX FIFO�Ĵ���
	  return RX_DR; 
	}
	else    
		return ERROR;                    //û�յ��κ�����
}


/***************************************majianghua*************************************************/

