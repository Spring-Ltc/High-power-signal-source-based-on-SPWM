



/********************�����ʵ��ѧ2020�굥Ƭ��Ӧ����ƾ���������ѧԺ��************************************

MCU�ͺţ�STM32F103RCT6
���ߣ�Liao_Tingchun�����䣺2093181896@qq.com
�޸���־��
2020/07/19/00:49
	�����¹��̣�����ȷ��MCU��Դ����
2020/07/19/18:03
	���ճ������������½�HARDWARE�¸��ļ�
2020/08/08/22:40
	����ȷ����PCB������MCU��Դ������س�ʼ��(���뿪�ء�������⡢LED��KEY�Ȼ���)
2020/08/09/13:14
	��ֲIIC��ش��롢TFTҺ������ش��룬���д�У�飬��ģ���ʼ������ȫ�����
2020/08/15/10:18
	��β���������ϣ��޸ĵ���Keil��Proteus��ʱ�Ӻ;������ã��������޷�˳�����У�
	������Ҫԭ��ΪProteusû��STM32���ⲿʱ�������������棬׼����ó����ܣ���У��Ӳ���ڵ���
2020/08/15/11:14
	������ó����ܣ��������ƺø������ʼ�����ϵ���ʷ�����Լ죬�½��㷨�ļ�PIDcontrolSPWM.c�ļ�
2020/08/25/22:00
	���ϵͳ����ʱ��Ƭ��ʱ������ã�������������׼��ʹ�ܱ�־
2020/09/03/18:22
	��ɵ�·�庸�ӣ����л������ԣ�LED����������TTL������������Դ��������MC34063��ѹ�Ը�Ϊ18V���ң�����16.2V��
	����PCB���⣺����˿�͹�ģ��з�װ�����޷�����
2020/09/03/18:55
	��ֲ��LCD���̣�ͨ���궨���Ϊ���ģ��SPI����Ч����Ϊ����ʱ�����Լ����õķ���պ÷���,�������ܺ�������ͨ��
2020/09/04/21:00
	AT24C256�洢IIC����ͨ������Դ��ѹADC�ɼ�����OK����������ѹ����������
2020/09/04/21:35
	����������ACS712-20�������ͨ�������ǵ�����Ҫ�޸ģ���������R4��R8��R17��Ϊ0����ֱ�Ӷ̽�
	��·���Ӹ���ʱ�������������ѹΪ2.5V���ң��������������ѹ���Ƚϴ�
2020/09/04/22:27
	���������жϣ����ǲ�����δͨ��

*****************************************************************************************************/	

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

#include "PwmTimer.h"
#include "Encoder.h"
#include "TaskTimer.h"
#include "CurrentSensorADC.h"
#include "LcdDisplay.h"
#include "CommonGPIO.h"
#include "I2CEquipment.h"
#include "SerialPortNetwork.h"
#include "PIDcontrolSPWM.h"
#include "GUI.h"


int main(void)
{
	u8 i;
	u16 PowerVoltage;//����Դ��ѹ
	float Current;
	
	//1��system basic config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2 
	delay_init();	
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	uart_init(9600);//����1��USB����
		
	//2��user equipment config
	PWM_Time8Init();//SPWM��ʱ����ʼ��
	EC11Encoder_Init();//EC11���뿪�ض�ʱ����ʼ��
	TaskTime_Init();//ϵͳ����ʱ��Ƭ����
	LcdDisplay_Init();//Һ����ʾ��ʼ��
	CurrentADC_Init();//���ص������ADC��ʼ��													������ͨ����
	SupplyVoltageADC_Init();//���빩���ѹ���ADC��ʼ��								������ͨ����
	KEY_Init();LED_Init();BUZZ_Init();//����������LED����������ʼ��
	StorageI2C_Init();//EEPROM�洢IIC��ʼ��														������ͨ����
//	TemperatureI2C_Init();//LM75AD�¶ȴ�����IIC��ʼ��
//	MyUsart2_Init(9600);//��������ͨ�Ŵ��ڳ�ʼ������ѡ��
//	SPWM_TimeInit();
	//3��historical data self-check����ȡEEPROM�û����õ���ʷ����
	
	


	printf("COM Text\r\n\r\n");
	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(32,70,BLUE,RED, "ȫ�̼���֧��");
	

//	StorageWriteOneByte(0x1200,0x66);//д��0x55

	printf("read the data is %x \r\n",StorageReadOneByte(0x1200));
	
	
  while (1)
  {
		LED_System =!LED_System;
//		PowerVoltage = ReadPowerVoltage();
//		printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
		
		Current = ReadCurrent(1);
		printf("Current is: %f  A\r\n\r\n",Current);
		delay_ms(1000);
		
		
		if(Flag_TaskPower)//��ȡ��Դ��ѹ
		{
			Flag_TaskPower = 0;
			
		}
		
		if(Flag_TaskTemp)//���PCB�¶�
		{
			Flag_TaskTemp = 0;
			
		}
  }
}










//============================�ò���Ϊ�����жϷ�����=================================//

//�ж���10--15����һ���жϷ�����
void EXIT15_10_IRQHander(void)
{
	//����1
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		delay_ms(5);//��������
		if(PCin(10) == 0)//����������
		{
			LED_EN_OC1 = !LED_EN_OC1;
			//�ж��߼�
			//��������������
		}
		while( !PCin(10));//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line10);	//����жϱ�־λ
	}
	//����2
	else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		delay_ms(5);//��������
		if(PCin(11) == 0)//����������
		{
			LED_EN_OC2 = !LED_EN_OC2;
			//�ж��߼�
			//��������������
		}
		while( !PCin(11));//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line11);	//����жϱ�־λ
	}
	//����3
	else if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		delay_ms(5);//��������
		if(PCin(12) == 0)//����������
		{
			LED_EN_OC3 = !LED_EN_OC3;
			//�ж��߼�
			//��������������
		}
		while( !PCin(12));//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line12);	//����жϱ�־λ
	}
	//����4
	else if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		delay_ms(5);//��������
		if(PCin(13) == 0)//����������
		{
			//�ж��߼�
			//��������������
		}
		while( !PCin(13));//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line13);	//����жϱ�־λ
	}
}



