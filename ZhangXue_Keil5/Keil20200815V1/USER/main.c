



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


int main(void)
{

	//1��system basic config
	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	uart_init(9600);//����1��USB����
		
	//2��user equipment config
	PWM_Time8Init();//SPWM��ʱ����ʼ��
	EC11Encoder_Init();//EC11���뿪�ض�ʱ����ʼ��
	TaskTime_Init();//ϵͳ����ʱ��Ƭ����
	LcdDisplay_Init();//Һ����ʾ��ʼ��
	CurrentADC_Init();//���ص������ADC��ʼ��
	SupplyVoltageADC_Init();//���빩���ѹ���ADC��ʼ��
	KEY_Init();LED_Init();BUZZ_Init();//����������LED����������ʼ��
	StorageI2C_Init();//EEPROM�洢IIC��ʼ��
	TemperatureI2C_Init();//LM75AD�¶ȴ�����IIC��ʼ��
	MyUsart2_Init(9600);//��������ͨ�Ŵ��ڳ�ʼ������ѡ��
	
	//3��historical data self-check����ȡEEPROM�û����õ���ʷ����
	
	


	printf("COM Text\r\n\r\n");
	LED_System=1;
  while (1)
  {
		LED_System =!LED_System;
		delay_ms(500);
  }
}




