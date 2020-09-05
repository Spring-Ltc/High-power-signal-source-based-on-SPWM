
//IIC�豸����ļ�
//�¶ȼ�飬EEPROM�洢





#include "I2CEquipment.h"
#include "delay.h"

//EEPROM�洢IIC��ʼ��
//	Storage_SCL------>PA11
//	Storage_SDA------>PA12


void StorageI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	IIC_SCL=1;
//	IIC_SDA=1;
}


//�¶ȴ�����IIC��ʼ��
//	Temp_SCL------>PB11
//	Temp_SDA------>PB12
void TemperatureI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	I2CTemp_SCL=1;
	I2CTemp_SDA=1;
}





//-----------------------------------------------------�¶ȴ�����IICͨ�����------------------------------------//


//����I2CTemp��ʼ�ź�
void I2CTemp_Start(void)
{
	TempSDA_OUT();     //sda�����
	I2CTemp_SDA = 1;	  	  
	I2CTemp_SCL = 1;
	delay_us(4);
 	I2CTemp_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2CTemp_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����I2CTempֹͣ�ź�
void I2CTemp_Stop(void)
{
	TempSDA_OUT();//sda�����
	I2CTemp_SCL = 0;
	I2CTemp_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2CTemp_SCL = 1; 
	I2CTemp_SDA = 1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t I2CTemp_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	TempSDA_IN();      //SDA����Ϊ����  
	I2CTemp_SDA = 1;delay_us(1);	   
	I2CTemp_SCL = 1;delay_us(1);	 
	while(ReadTemp_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2CTemp_Stop();
			return 1;
		}
	}
	I2CTemp_SCL = 0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void I2CTemp_Ack(void)
{
	I2CTemp_SCL = 0;
	TempSDA_OUT();
	I2CTemp_SDA = 0;
	delay_us(2);
	I2CTemp_SCL = 1;
	delay_us(2);
	I2CTemp_SCL = 0;
}
//������ACKӦ��		    
void I2CTemp_NAck(void)
{
	I2CTemp_SCL = 0;
	TempSDA_OUT();
	I2CTemp_SDA = 1;
	delay_us(2);
	I2CTemp_SCL = 1;
	delay_us(2);
	I2CTemp_SCL = 0;
}					 				     
//I2CTemp����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I2CTemp_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	TempSDA_OUT(); 	    
    I2CTemp_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			I2CTemp_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2CTemp_SCL = 1;
		delay_us(2); 
		I2CTemp_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t I2CTemp_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	TempSDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        I2CTemp_SCL = 0; 
        delay_us(2);
		I2CTemp_SCL = 1;
        receive<<=1;
        if(ReadTemp_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I2CTemp_NAck();//����nACK
    else
        I2CTemp_Ack(); //����ACK   
    return receive;
}







//-----------------------------------------------------AT24C256�洢��IICͨ�����------------------------------------//


//����I2CStorage��ʼ�ź�
void I2CStorage_Start(void)
{
	StorageSDA_OUT();     //sda�����
	I2CStorage_SDA = 1;	  	  
	I2CStorage_SCL = 1;
	delay_us(4);
 	I2CStorage_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2CStorage_SCL = 0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����I2CStorageֹͣ�ź�
void I2CStorage_Stop(void)
{
	StorageSDA_OUT();//sda�����
	I2CStorage_SCL = 0;
	I2CStorage_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2CStorage_SCL = 1; 
	I2CStorage_SDA = 1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t I2CStorage_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	StorageSDA_IN();      //SDA����Ϊ����  
	I2CStorage_SDA = 1;delay_us(1);	   
	I2CStorage_SCL = 1;delay_us(1);	 
	while(ReadStorage_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2CStorage_Stop();
			return 1;
		}
	}
	I2CStorage_SCL = 0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void I2CStorage_Ack(void)
{
	I2CStorage_SCL = 0;
	StorageSDA_OUT();
	I2CStorage_SDA = 0;
	delay_us(2);
	I2CStorage_SCL = 1;
	delay_us(2);
	I2CStorage_SCL = 0;
}
//������ACKӦ��		    
void I2CStorage_NAck(void)
{
	I2CStorage_SCL = 0;
	StorageSDA_OUT();
	I2CStorage_SDA = 1;
	delay_us(2);
	I2CStorage_SCL = 1;
	delay_us(2);
	I2CStorage_SCL = 0;
}					 				     
//I2CStorage����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void I2CStorage_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	StorageSDA_OUT(); 	    
    I2CStorage_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
			I2CStorage_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2CStorage_SCL = 1;
		delay_us(2); 
		I2CStorage_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t I2CStorage_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	StorageSDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        I2CStorage_SCL = 0; 
        delay_us(2);
		I2CStorage_SCL = 1;
        receive<<=1;
        if(ReadStorage_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I2CStorage_NAck();//����nACK
    else
        I2CStorage_Ack(); //����ACK   
    return receive;
}




//-----------------------------------------------------------ģ��ʹ�÷������------------------------------------------------//


//---------------------����˵��--------------------------//
//�������ܣ�	ָ��AT24C256�ĵ�ַд��һ���ֽ�����
//��ڲ�����	WriteAddr:	Ҫд��ĵ�ַ
//						DataToWrite��Ҫд�������
//����ֵ����

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//--------------------------------
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
	I2CStorage_Start();  
	
	I2CStorage_Send_Byte(0XA0);	    //����д����
	I2CStorage_Wait_Ack();
		I2CStorage_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
		I2CStorage_Wait_Ack();	   
  I2CStorage_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	I2CStorage_Wait_Ack(); 	 										  		   
		I2CStorage_Send_Byte(DataToWrite);     //�����ֽ�							   
		I2CStorage_Wait_Ack();  	
	
  I2CStorage_Stop();//����һ��ֹͣ���� 
	delay_ms(10);
}



//---------------------����˵��--------------------------//
//�������ܣ�	ָ��AT24C256�ĵ�ַ��ȡһ���ֽ�����
//��ڲ�����	ReadAddr:	Ҫ��ȡ�ĵ�ַ
//����ֵ��	��ȡ�Ľ��

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//--------------------------------
uint8_t StorageReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp=0;	
	
  I2CStorage_Start(); 
	
	I2CStorage_Send_Byte(0XA0);	   //����д����
	I2CStorage_Wait_Ack();
		I2CStorage_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
		I2CStorage_Wait_Ack();		  
  I2CStorage_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	I2CStorage_Wait_Ack();	
	
	I2CStorage_Start();  	 	   
	I2CStorage_Send_Byte(0XA1);           //�������ģʽ			   
	I2CStorage_Wait_Ack();	
	
  temp=I2CStorage_Read_Byte(0);		   
  I2CStorage_Stop();//����һ��ֹͣ����
	
	return temp;
}




//---------------------����˵��--------------------------------------------//
//�������ܣ�	ָ����ʼ��ַд��ָ�����ȸ��ֽ����ݵ�AT24C256�洢��
//��ڲ�����	StarAddr:	д��AT24C56����ʼ��ַ
//					*WriteBuffer:	��Ҫд������ݵĴ�ŵ�ַ��������Դ��
//					Len:	��Ҫд����ֽ���
//����ֵ��	��

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//--------------------------------
void StorageWriteLenByte(uint16_t StartAddr,uint8_t *WriteBuffer,uint8_t Len)
{
	while(Len--)
	{
		StorageWriteOneByte(StartAddr,*WriteBuffer);
		StartAddr++;
		WriteBuffer++;
	}
}


//---------------------����˵��--------------------------------------------//
//�������ܣ�ָ��AT24C256����ʼ��ַ��ȡָ�����ȸ��ֽ�����
//��ڲ�����StarAddr:��ʼ��ַ
//					*WriteBuffer:��Ҫ���������ݵĴ����ʼ��ַ
//					Len:��Ҫ�������ֽ���
//����ֵ��	��

//--------------------------------
//���ߣ�Liao_tingchun		
//�޸����ڣ�2020/05/07
//�޸����ݣ�������ɻ�������
//--------------------------------
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len)
{
	while(Len)
	{
		*ReadBuffer++=StorageReadOneByte(StartAddr++);	
		Len--;
	}
}

