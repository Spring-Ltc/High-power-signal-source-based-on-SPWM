
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



//AT24C256相关
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);//指定地址写入一个字节
uint8_t StorageReadOneByte(uint16_t ReadAddr);//指定地址读取一个字节
void StorageWriteLenByte(uint16_t StartAddr,uint8_t *WriteBuffer,uint8_t Len);//指定起始地址写入指定长度个字节数据
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len);//指定起始地址读出指定长度个字节的数据





#endif



