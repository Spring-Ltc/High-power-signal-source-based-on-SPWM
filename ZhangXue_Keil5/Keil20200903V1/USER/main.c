



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


unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};

int main(void)
{
	u8 i;

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
	SPWM_TimeInit();
	//3��historical data self-check����ȡEEPROM�û����õ���ʷ����
	
	
	
	Lcd_Clear(BLUE);
	//BUZZ_ON;delay_ms(300);BUZZ_OFF;
	
 LED_Select_OC1	= 0;	delay_ms(300);LED_Select_OC1	= 1;
 LED_Select_OC2		= 0;	delay_ms(300);	LED_Select_OC2		= 1;
 LED_Select_OC3			= 0;	delay_ms(300); LED_Select_OC3			= 1;
 

 
 LED_EN_OC1		= 0;	delay_ms(300);			LED_EN_OC1		= 1;
 LED_EN_OC2		= 0;	delay_ms(300);		 LED_EN_OC2		= 1;	
 LED_EN_OC3			= 0;	delay_ms(300);		LED_EN_OC3			= 1;



	printf("COM Text\r\n\r\n");
	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "ȫ�̼���֧��");
	
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+30,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	
	
  while (1)
  {
		LED_System =!LED_System;

		delay_ms(500);
		
		
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




