



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
2020/09/05/14:47
	�����жϿ���������δ����������̳������delay�жϳ�ͻ������ɨ�跽ʽ֤��������·����
	����������ԭ�ӵ�delay��sys�ļ�
2020/09/05/16:54
	���ֲ������·���⣺��ģ��з�װ���ԣ������������ֵ�У����������������ѹ���Ŵ��ӵ����Ϊ1.1k��ʵ�������ѹԼ2.15V���������������
	�������оƬ�ڶ�·�����Ǻ���ʱ���ˣ�����IC��Ӳ���̽�������������MOS���أ�������·������
2020/09/05/20:37
	��ת���뿪��EC11������ϣ������������֤OK���ܹ��ж���ת���������������ʹ�ö�ʱ����ɨ���ʡ���ж���Դ
2020/09/05/21:35
	��Ӱ����л�ͨ����������������OK��ΪʹLED֪ʶ��ɫ˳���ͨ��һ�£����������LED���ţ�OC1��OC3����select���ţ�
	���뿪�صİ�����������һ��ʹ�ð����жϣ�������ǿ��������ǲ���ֵ��غ����Ѿ����
2020/09/06/10:56
	���뿪�ص��ⲿ�жϿ������������������жϷ���������д���ˣ�����Ӣ��ûѧ�ã�ctrl+C/V������
2020/09/06/12:51
	�������ⲿ�жϿ�����������EXTI��ʼ����ʱ��Ҫ�ֿ���ʼ��������ʹ�û�����
2020/09/06/17:15
	�߼���ʱ���������PWM�����֤��ϣ�ע���ʼ����ʱ��ԭͨ���ͻ���ͨ���������������һ�����ǻ������
	�������⣺��׼Ƶ������̫�ߣ�500kHz��Լ2us������ϵͳ�쳣�ϵ�
		������⣺���������������ʵ��������ǻ������
	�������PWMʱ��PCB�����ƽ�淢�ȱȽ����ԣ�������MC34063�������裬���������ѹ12.3V����ƫ��
	
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


//==============================ȫ�ֱ�������===============================//
u16 NowcountFreq=0;//��ǰ���뿪�ؼ���ֵ
u16 LastcountFreq=0;
u16 NowcountAmpl=0;//�ϴα��뿪�ؼ���ֵ
u16 LastcountAmpl=0;
u8 SelectCount=0;
u8 NowSelect;//��ǰѡ�е�ͨ��
u16 StepFreq=5;//����Ƶ�ʲ���ֵ
u16 StepAmpl=5;//���ڷ�ֵ����ֵ



//======================����������============================//
void TaskEncodeUpdate_Freq(u16 nowcount);
void TaskEncodeUpdate_Ampl(u16 nowcount);
void TaskKeyScan(u8 key);
void TaskSwitchSelectSwitch(u8 select);


int main(void)
{
//	u8 i,KeyValue;
//	u16 PowerVoltage;//����Դ��ѹ
//	float Current;
//	u32 count;
	
	//1��system basic config
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// �����ж����ȼ�����2��2λ��ռ���ȼ���2λ�����ȼ��������ȼ�����ֻ������һ�Ρ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
		
	//2��user equipment config
//	PWM_Time8Init();//SPWM��ʱ����ʼ��																������ͨ����
	EC11Encoder_Init();//EC11���뿪�ض�ʱ����ʼ��											������ͨ����
	TaskTime_Init();//ϵͳ����ʱ��Ƭ����
	LcdDisplay_Init();//Һ����ʾ��ʼ��																�����԰�ͨ����������������Ļ���������⡿
	CurrentADC_Init();//���ص������ADC��ʼ��													������ͨ����
	SupplyVoltageADC_Init();//���빩���ѹ���ADC��ʼ��								������ͨ����
	KEY_Init();LED_Init();BUZZ_Init();//����������LED����������ʼ��		������ͨ����
	StorageI2C_Init();//EEPROM�洢IIC��ʼ��														������ͨ����
//	TemperatureI2C_Init();//LM75AD�¶ȴ�����IIC��ʼ��
//	MyUsart2_Init(9600);//��������ͨ�Ŵ��ڳ�ʼ������ѡ��
//	SPWM_TimeInit();
	//3��historical data self-check����ȡEEPROM�û����õ���ʷ����

	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(32,70,BLUE,RED, "ȫ�̼���֧��");
	Flag_TaskReadEncode=1;
  while (1)
  {
		//��������
//		KeyValue=KEY_Scan(1);		//�õ���ֵ
//		if( KeyValue)//�а�������
//			TaskKeyScan(KeyValue);

		LED_System =!LED_System;
		delay_ms(200);

//		Current = ReadCurrent(1);
//		printf("Current is: %f  A\r\n\r\n",Current);
		
		
		
		if(Flag_TaskPower)//��ȡ��Դ��ѹ
		{
			Flag_TaskPower = 0;
//		PowerVoltage = ReadPowerVoltage();
//		printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
		}
		
		if(Flag_TaskTemp)//���PCB�¶�
		{
			Flag_TaskTemp = 0;
			
		}
		if(Flag_TaskReadEncode)//ˢ�±��뿪�ص�ֵ
		{
			//Flag_TaskReadEncode=0;
			NowcountFreq = TIM_GetCounter(TIM4);
			if(NowcountFreq != LastcountFreq)
				TaskEncodeUpdate_Freq(NowcountFreq);
			NowcountAmpl = TIM_GetCounter(TIM2);
			if(NowcountAmpl != LastcountAmpl)
				TaskEncodeUpdate_Ampl(NowcountAmpl);
		}
  }
}



//��������������ʱɨ���ֵ��ʽ
void TaskKeyScan(u8 key)
{
	switch(key)
	{				 
		case KEY_EN_OC1_PRES:
			LED_EN_OC1=!LED_EN_OC1;
			break;
		case KEY_EN_OC2_PRES:
			LED_EN_OC2=!LED_EN_OC2;
			break;
		case KEY_EN_OC3_PRES:				
			LED_EN_OC3=!LED_EN_OC3;
			break;
		case KEY_OC_Switch_PRES:	
		{
			SelectCount++;
			NowSelect = SelectCount%3+1;
			TaskSwitchSelectSwitch(NowSelect);
//			printf("Switch\r\n");
			break;
		}
	} 
}

//ͨ���л�������
void TaskSwitchSelectSwitch(u8 select)
{
	if(select == 1)
	{
		LED_Select_OC1 = 0;LED_Select_OC2 = 1;LED_Select_OC3 = 1;	
	}
	else if(select == 2)
	{
		LED_Select_OC1 = 1;LED_Select_OC2 = 0;LED_Select_OC3 = 1;	
	}
	else if(select == 3)
	{
		LED_Select_OC1 = 1;LED_Select_OC2 = 1;LED_Select_OC3 = 0;	
	}
	
}



//Ƶ�ʸı�������
void TaskEncodeUpdate_Freq(u16 nowcount)
{
	u16 ValueChanged;
	if(nowcount < LastcountFreq)//Ƶ�ʼ�С	
	{
		ValueChanged = (LastcountFreq - nowcount)/2;
		LastcountFreq = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		//������PWM��������ء�
		printf("Freq disces %d\r\n",ValueChanged*StepFreq);
	}
	else//Ƶ������
	{
		ValueChanged = (nowcount - LastcountFreq)/2;
		LastcountFreq = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		//������PWM��������ء�
		printf("Freq incres %d\r\n",ValueChanged*StepFreq);
	}
	
}
//��ֵ�ı�������
void TaskEncodeUpdate_Ampl(u16 nowcount)
{
	u16 ValueChanged;
	if(nowcount < LastcountAmpl)//��ֵ��С
	{
		ValueChanged = (LastcountAmpl - nowcount)/2;
		LastcountAmpl = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		//������PWM��������ء�
		printf("Ampl dicrese %d\r\n",ValueChanged*StepAmpl);
	}
	else//��ֵ����
	{
		ValueChanged = (nowcount - LastcountAmpl)/2;
		LastcountAmpl = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		//������PWM��������ء�
		printf("Ampl increse %d\r\n",ValueChanged*StepAmpl);
	}
}




//============================�ò���Ϊ�����жϷ�����=================================//

//�ж���10--15����һ���жϷ�����
//4���������ⲿ�жϷ�����
void EXTI15_10_IRQHandler(void)
{
	delay_ms(2);//��������
	
	//����1
	if(KEY_EN_OC1 == 0)//����������
	{
		LED_EN_OC1 = !LED_EN_OC1;
		printf("KEY1\r\n");
		while(!KEY_EN_OC1);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line10);	//����жϱ�־λ
		return;
	}
		
	//����2
	else if(KEY_EN_OC2 == 0)//����������
	{
		LED_EN_OC2 = !LED_EN_OC2;
		printf("KEY2\r\n");
		while(!KEY_EN_OC2);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line11);	//����жϱ�־λ
		return;
	}
	
	//����3
	else if(KEY_EN_OC3 == 0)//����������
	{
		LED_EN_OC3 = !LED_EN_OC3;
		printf("KEY3\r\n");
		while(!KEY_EN_OC3);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line12);	//����жϱ�־λ
		return;
	}

	//����4
	else if(KEY_OC_Switch == 0)//����������
	{
		SelectCount++;
		NowSelect = SelectCount%3+1;
		TaskSwitchSelectSwitch(NowSelect);
		printf("KEY4\r\n");
		while(!KEY_OC_Switch);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line13);	//����жϱ�־λ
		return;
	}
	printf("KEY BUTTON\r\n");
	EXTI_ClearITPendingBit(EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13);	//����жϱ�־λ
}


//���뿪�ذ����жϷ������ı�Ƶ�ʵ��ڵĲ���ֵ
void EXTI9_5_IRQHandler(void)
{
	static u8 stepfreqcount=0;
	delay_ms(2);//��������
	if(KEY_Encod1 == 0)
	{
		stepfreqcount++;
		switch (stepfreqcount%3+1)
		{
			case 1:
				StepFreq = 10;break;
			case 2:
				StepFreq = 100;break;
			case 3:
				StepFreq = 1000;break;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line5);	//����жϱ�־λ
}

//���뿪�ذ����жϷ������ı��ֵ���ڵĲ���ֵ
void EXTI3_IRQHandler(void)
{
	static u8 stepamplcount=0;
//	printf("KEY Ampl Pres\r\n");
	delay_ms(2);//��������
	if(KEY_Encod2 == 0)
	{
		stepamplcount++;
		switch (stepamplcount%3+1)
		{
			case 1:
				StepAmpl = 10;break;
			case 2:
				StepAmpl = 100;break;
			case 3:
				StepAmpl = 1000;break;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);	//����жϱ�־λ
}





