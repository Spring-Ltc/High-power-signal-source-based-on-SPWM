
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



#define Adress_AmplOut		0x0000
#define Adress_FreqOut		0x1000
#define Adress_StorageFlag	0x1234	//�洢���λ���ж��ǲ��ǵ�һ�δ洢

#define TempDevice_Adress1 0x90	//000x����������¶ȴ�����
#define TempDevice_Adress2 0x92	//001x


//AT24C256���
void StorageWriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);//ָ����ַд��һ���ֽ�
uint8_t StorageReadOneByte(uint16_t ReadAddr);//ָ����ַ��ȡһ���ֽ�
void StorageWriteLenByte(uint16_t StartAddr,uint8_t *WriteBuffer,uint8_t Len);//ָ����ʼ��ַд��ָ�����ȸ��ֽ�����
void StorageReadLenByte(uint16_t StartAddr,uint8_t *ReadBuffer,uint8_t Len);//ָ����ʼ��ַ����ָ�����ȸ��ֽڵ�����


//���������궨���������� LM75AD �豸��IICͨ������
#define us4 1
#define us2 1
#define us1 1


void StorageI2C_Init(void);
void TemperatureI2C_Init(void);

float ReadTemperature(u8 deviceAddr);





#endif



