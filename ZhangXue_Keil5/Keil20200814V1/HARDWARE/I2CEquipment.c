
//IIC设备相关文件
//温度检查，EEPROM存储





#include "I2CEquipment.h"
#include "delay.h"

//EEPROM存储IIC初始化
//	Storage_SCL------>PA11
//	Storage_SDA------>PA12


void StorageI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	IIC_SCL=1;
//	IIC_SDA=1;
}


//温度传感器IIC初始化
//	Temp_SCL------>PB11
//	Temp_SDA------>PB12
void TemperatureI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	I2CTemp_SCL=1;
	I2CTemp_SDA=1;
}





//-----------------------------------------------------温度传感器IIC通信相关------------------------------------//


//产生I2CTemp起始信号
void I2CTemp_Start(void)
{
	TempSDA_OUT();     //sda线输出
	I2CTemp_SDA = 1;	  	  
	I2CTemp_SCL = 1;
	delay_us(4);
 	I2CTemp_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2CTemp_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生I2CTemp停止信号
void I2CTemp_Stop(void)
{
	TempSDA_OUT();//sda线输出
	I2CTemp_SCL = 0;
	I2CTemp_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2CTemp_SCL = 1; 
	I2CTemp_SDA = 1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t I2CTemp_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	TempSDA_IN();      //SDA设置为输入  
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
	I2CTemp_SCL = 0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//I2CTemp发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void I2CTemp_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	TempSDA_OUT(); 	    
    I2CTemp_SCL = 0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			I2CTemp_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2CTemp_SCL = 1;
		delay_us(2); 
		I2CTemp_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t I2CTemp_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	TempSDA_IN();//SDA设置为输入
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
        I2CTemp_NAck();//发送nACK
    else
        I2CTemp_Ack(); //发送ACK   
    return receive;
}







//-----------------------------------------------------AT24C256存储器IIC通信相关------------------------------------//


//产生I2CStorage起始信号
void I2CStorage_Start(void)
{
	StorageSDA_OUT();     //sda线输出
	I2CStorage_SDA = 1;	  	  
	I2CStorage_SCL = 1;
	delay_us(4);
 	I2CStorage_SDA = 0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	I2CStorage_SCL = 0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生I2CStorage停止信号
void I2CStorage_Stop(void)
{
	StorageSDA_OUT();//sda线输出
	I2CStorage_SCL = 0;
	I2CStorage_SDA = 0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	I2CStorage_SCL = 1; 
	I2CStorage_SDA = 1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t I2CStorage_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	StorageSDA_IN();      //SDA设置为输入  
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
	I2CStorage_SCL = 0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//I2CStorage发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void I2CStorage_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	StorageSDA_OUT(); 	    
    I2CStorage_SCL = 0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
			I2CStorage_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		I2CStorage_SCL = 1;
		delay_us(2); 
		I2CStorage_SCL = 0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t I2CStorage_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	StorageSDA_IN();//SDA设置为输入
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
        I2CStorage_NAck();//发送nACK
    else
        I2CStorage_Ack(); //发送ACK   
    return receive;
}




//-----------------------------------------------------------模块使用访问相关------------------------------------------------//


//---------------------函数说明--------------------------//
//函数功能：	指定AT24C256的地址写入一个字节数据
//入口参数：	WriteAddr:	要写入的地址
//						DataToWrite：要写入的数据
//返回值：无

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
//--------------------------------
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
	I2CStorage_Start();  
	
	I2CStorage_Send_Byte(0XA0);	    //发送写命令
	I2CStorage_Wait_Ack();
		I2CStorage_Send_Byte(WriteAddr>>8);//发送高地址
		I2CStorage_Wait_Ack();	   
  I2CStorage_Send_Byte(WriteAddr%256);   //发送低地址
	I2CStorage_Wait_Ack(); 	 										  		   
		I2CStorage_Send_Byte(DataToWrite);     //发送字节							   
		I2CStorage_Wait_Ack();  	
	
  I2CStorage_Stop();//产生一个停止条件 
	delay_ms(10);
}



//---------------------函数说明--------------------------//
//函数功能：	指定AT24C256的地址读取一个字节数据
//入口参数：	ReadAddr:	要读取的地址
//返回值：	读取的结果

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
//--------------------------------
uint8_t StorageReadOneByte(uint16_t ReadAddr)
{
	uint8_t temp=0;	
	
  I2CStorage_Start(); 
	
	I2CStorage_Send_Byte(0XA0);	   //发送写命令
	I2CStorage_Wait_Ack();
		I2CStorage_Send_Byte(ReadAddr>>8);//发送高地址
		I2CStorage_Wait_Ack();		  
  I2CStorage_Send_Byte(ReadAddr%256);   //发送低地址
	I2CStorage_Wait_Ack();	
	
	I2CStorage_Start();  	 	   
	I2CStorage_Send_Byte(0XA1);           //进入接收模式			   
	I2CStorage_Wait_Ack();	
	
  temp=I2CStorage_Read_Byte(0);		   
  I2CStorage_Stop();//产生一个停止条件
	
	return temp;
}




//---------------------函数说明--------------------------------------------//
//函数功能：	指定起始地址写入指定长度个字节数据到AT24C256存储器
//入口参数：	StarAddr:	写入AT24C56的起始地址
//					*WriteBuffer:	需要写入的数据的存放地址（数据来源）
//					Len:	需要写入的字节数
//返回值：	无

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
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


//---------------------函数说明--------------------------------------------//
//函数功能：指定AT24C256的起始地址读取指定长度个字节数据
//入口参数：StarAddr:起始地址
//					*WriteBuffer:需要读出的数据的存放起始地址
//					Len:需要读出的字节数
//返回值：	无

//--------------------------------
//作者：Liao_tingchun		
//修改日期：2020/05/07
//修改内容：初次完成基本功能
//--------------------------------
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len)
{
	while(Len)
	{
		*ReadBuffer++=StorageReadOneByte(StartAddr++);	
		Len--;
	}
}

