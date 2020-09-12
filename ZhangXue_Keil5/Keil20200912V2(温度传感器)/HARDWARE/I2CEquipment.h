
#ifndef __I2CEquipment_H__
#define __I2CEquipment_H__


#include "sys.h"


//温度传感器IIC宏定义
#define TempSDA_IN()  {GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=0x00080000;}
#define TempSDA_OUT() {GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=0x00030000;}
#define I2CTemp_SCL    PBout(11) //SCL
#define I2CTemp_SDA    PBout(12) //SDA	 
#define ReadTemp_SDA   PBin(12)  //输入SDA 

//存储器IIC宏定义
#define StorageSDA_IN()  {GPIOA->CRH&=0xFFF0FFFF;GPIOA->CRH|=0x00080000;}
#define StorageSDA_OUT() {GPIOA->CRH&=0xFFF0FFFF;GPIOA->CRH|=0x00030000;}
#define I2CStorage_SCL    PAout(11) //SCL
#define I2CStorage_SDA    PAout(12) //SDA	 
#define ReadStorage_SDA   PAin(12)  //输入SDA 



#define Adress_AmplOut		0x0000
#define Adress_FreqOut		0x1000
#define Adress_StorageFlag	0x1234	//存储标记位，判断是不是第一次存储

#define TempDevice_Adress1 0x90	//000x，板载左边温度传感器
#define TempDevice_Adress2 0x92	//001x


//AT24C256相关
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);//指定地址写入一个字节
uint8_t StorageReadOneByte(uint16_t ReadAddr);//指定地址读取一个字节
void StorageWriteLenByte(uint16_t StartAddr,uint8_t *WriteBuffer,uint8_t Len);//指定起始地址写入指定长度个字节数据
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len);//指定起始地址读出指定长度个字节的数据


//下面三个宏定义用来调整 LM75AD 设备的IIC通信速率
#define us4 1
#define us2 1
#define us1 1


void StorageI2C_Init(void);
void TemperatureI2C_Init(void);

float ReadTemperature(u8 deviceAddr);





#endif



