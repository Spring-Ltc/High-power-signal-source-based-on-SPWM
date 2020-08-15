
#ifndef __I2CEquipment_H__
#define __I2CEquipment_H__


#include "sys.h"


//�¶ȴ�����IIC�궨��
#define TempSDA_IN()  {GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=0x00080000;}
#define TempSDA_OUT() {GPIOB->CRH&=0xFFF0FFFF;GPIOB->CRH|=0x00030000;}
#define I2CTemp_SCL    PBout(11) //SCL
#define I2CTemp_SDA    PBout(12) //SDA	 
#define ReadTemp_SDA   PBin(12)  //����SDA 

//�洢��IIC�궨��
#define StorageSDA_IN()  {GPIOA->CRH&=0xFFF0FFFF;GPIOA->CRH|=0x00080000;}
#define StorageSDA_OUT() {GPIOA->CRH&=0xFFF0FFFF;GPIOA->CRH|=0x00030000;}
#define I2CStorage_SCL    PAout(11) //SCL
#define I2CStorage_SDA    PAout(12) //SDA	 
#define ReadStorage_SDA   PAin(12)  //����SDA 



//AT24C256���
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);//ָ����ַд��һ���ֽ�
uint8_t StorageReadOneByte(uint16_t ReadAddr);//ָ����ַ��ȡһ���ֽ�
void StorageWriteLenByte(uint16_t StartAddr,uint8_t *WriteBuffer,uint8_t Len);//ָ����ʼ��ַд��ָ�����ȸ��ֽ�����
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len);//ָ����ʼ��ַ����ָ�����ȸ��ֽڵ�����





#endif



