



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
2020/09/06/21:28
	Һ����Ļ�����Ѿ���������������Lcd_SetXY�����е��β�Ϊ���ر߽��ȥԭ����ֵ
2020/09/07/08:28
	�������⣬ϵͳ����ʱ��TIM��ʱ�ж�ʱ�䲻�ԣ�������ֵƫ���˺ܶ�
	09:06���������⣬��ͨ��ͬʱ������ϵͳ�����Զϵ磬�����ƶ���û�����������������
2020/09/07/:09:40
	ϵͳ����ʱ����ʱʱ��ƫ����������ԭ����û������TIM_RepetitionCounter������������ȱʡֵ��Ϊ0
	������������Ⱥ󣬶ϵ���������������������Ȼ�Ƚ����أ���Ҫ������оƬIR2104����
	��Ȳ�����������27/������18/��ѹ���ͣ���Ļ��������û�жϵ磻9/�����Զϵ�
2020/09/07/10:29
	���л������ܲ���ͨ����׼��дSPWM�����������Ҳ���ͻȻ���ֻ���PWM�����������û�취���֣�һ���������.............
2020/09/07/14:30
	TIM8�Ļ���ͨ�����պÿ�����TIM3��ͨ���滻������������ʱ��6��ͨ�����SPWM�����ST��IC���Ÿ�����ƾ���һ��
	���ö�ʱ��TIM8��TIM3�Ļ�׼Ƶ��Ϊ600kHz,10����׼�ź�����Ϊһ��SPWM�㣬ÿ��SPWM������������2*20��������
	��������Ҳ�MaxFreq=1.5kHz�����ö�ʱ��TIM6��ʱԼ16.6us(60kHz)����һ��ռ�ձ�
2020/09/07/16:33
	дSPWM����ʱ���������⣬����ͨ������һ��TIM6��ʱ����ռ�ձȵĻ���һ��״̬�����ܶ�������
2020/09/07/19:32
	SPWM��غ�����ɣ���ͨ�����Ժ���û����������Ǵ�ӡ�������ռ�ձ���ʾ��ȷ
2020/09/07/21:22
	�Ȱѿ�����ʾ����д����������ʱ�������Һ��������ʾ��ԭ���Ķ�ʱ������Ϊ�����ͱ��뿪�ز�������
	Һ������ʾ���ֻ���ȷ����������ʵ�ʲ�����Ҫ���ʵ���λ�õ�������ֲ����û��������ʾ����Ҫ�Լ����ַ�ת��
2020/09/07/22:14
	������ʾ������ɣ����ǲ�����δͨ��
2020/09/08/11:15
	������ʾ������ɣ���������ת�ַ�������������ͨ���������������
2020/09/08/15:46
	�򵥵Ľ��沼�ֻ�����ɣ���������ڸ�����ʾ��ɣ�����������⡢��Դ��ѹ��⣬����ֵ����Ƶ����������״̬��ͨ��ѡ�У�
	�����ϻ����¶ȼ�⡢���ݴ洢�����������������
2020/09/08/16:35
	���ݴ洢���ϵ��Լ첿����ɣ���ʱֻ���˷�Ƶ�����洢���Լ���ʷ���ݱ�־λ
2020/09/08/18:52
	���������⣬��׼Ƶ��Ϊ600kHzʱ������120����ռ�ձȵ��ڷ�Χ���ޣ�50---78����
	ʾ�����ⶨ���ߵ�ƽ����ʱ��min=640ns���͵�ƽ����ʱ��min=720ns
2020/09/08/20:03
	����1���޸Ļ�׼Ƶ��Ϊ300kHz��5������Ϊһ�㣬����4*10����Ϊһ��SPWM���Σ����Ƶ������1.5kHz
		��ý����MaxDuty=82%,MinDuty=21%,
	����2���޸Ļ�׼Ƶ��Ϊ200kHz��5������Ϊһ�㣬����4*10����Ϊһ��SPWM���Σ����Ƶ������1kHz
		��ý����MaxDuty=88%,MinDuty=14%,
	���ֽ��ͻ�׼Ƶ�ʹ�������IC����������ô������
2020/09/08/20:31
	SPWM�����Ѿ�ģ�����֣����ǻ����������⣺
		����һ��TIM8��TIM1��ʱ��ֱ�ӵ��µ����������ͨ����������λ����Ӻ��ʧ����
		�������SPWM�����Ķ�ʱ��TIM6ʱ����ܲ�׼ȷ��û�а�����5��һ����
				�������40kHûë������ʱӦ����OK��,����׼Ƶ�����һ�����ݿ�һ��Ч��
2020/09/08/21:51
	����ע���һ��ͨ����Ӧ��ϵ����ǰ�����ͨ���͵�·���ͨ���Ƿ��ŵģ���ͨ��1��ͨ��3��Ҫ����һ�£�
	PCB��ͨ��3���溸����һ��1206��װ��100nF������һ�£�������
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
u8 NowSelect=1;//��ǰѡ�е�ͨ��
u16 StepFreq=1;//����Ƶ�ʲ���ֵ
u16 StepAmpl=1;//���ڷ�ֵ����ֵ

//����ͨ���ĵ�ǰ���
u8 NowAmplOut[3]={99,99,99};
u16 NowFreqOut[3]={500,500,500};
u16 NowCurrOut[3]={200,200,200};


//======================����������============================//
void TaskEncodeUpdate_Freq(u16 nowcount);
void TaskEncodeUpdate_Ampl(u16 nowcount);
void TaskKeyScan(u8 key);
void TaskSwitchSelectSwitch(u8 select);
void TaskDisplayUpdate(void);
void TaskDataDtorage(void);
void HistoryDataCheck(void);


int main(void)
{
//	u8 i,KeyValue;
	u16 PowerVoltage;//����Դ��ѹ
	
	//1��system basic config
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// �����ж����ȼ�����2��2λ��ռ���ȼ���2λ�����ȼ��������ȼ�����ֻ������һ�Ρ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
		
	//2��user equipment config
	PWM_BaseTimeInit();
//	PWM_Time8Init();//�������PWM��ʱ����ʼ��																������ͨ���������ֻ���PWM���ܲ���SPWM��
	EC11Encoder_Init();//EC11���뿪�ض�ʱ����ʼ��																				������ͨ����
	TaskTime_Init();//ϵͳ����ʱ��Ƭ����																	 							������ͨ����
	LcdDisplay_Init();//Һ����ʾ��ʼ��																		  						������ͨ����
	CurrentADC_Init();//���ص������ADC��ʼ��																						������ͨ����
	SupplyVoltageADC_Init();//���빩���ѹ���ADC��ʼ��																	������ͨ����
	KEY_Init();LED_Init();BUZZ_Init();FANControl_Init();//����������LED����������ʼ��		������ͨ����
	StorageI2C_Init();//EEPROM�洢IIC��ʼ��																							������ͨ����
//	TemperatureI2C_Init();//LM75AD�¶ȴ�����IIC��ʼ��
//	MyUsart2_Init(9600);//��������ͨ�Ŵ��ڳ�ʼ������ѡ��
	SPWM_TimeInit();				//������δ��ɡ�
	
	//3��historical data self-check����ȡEEPROM�û����õ���ʷ����
	HistoryDataCheck();//																																������ͨ����
	TaskDisplayUpdate();//����ʾĬ�ϲ���
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);
	
	
  while (1)
  {
		//��������
//		KeyValue=KEY_Scan(1);		//�õ���ֵ
//		if( KeyValue)//�а�������
//			TaskKeyScan(KeyValue);

		
		
//		LED_System =!LED_System;
		delay_ms(500);

		if(Flag_TaskCurrent)//�����������
		{
			Flag_TaskCurrent=0;
			NowCurrOut[0] = ReadCurrent(1);
			NowCurrOut[1] = ReadCurrent(2);
			NowCurrOut[2] = ReadCurrent(3);
			Gui_DrawNum_GBK16(100,140,BLUE,GRAY0,NowCurrOut[0]);
			Gui_DrawNum_GBK16(160,140,BLUE,GRAY0,NowCurrOut[1]);
			Gui_DrawNum_GBK16(220,140,BLUE,GRAY0,NowCurrOut[2]);//��ʾ����
		}
		
		
		
		
		if(Flag_TaskPower)//��ȡ��Դ��ѹ������ѹ�����ͣ��ر����������·
		{
			Flag_TaskPower = 0;
			PowerVoltage = ReadPowerVoltage();
			Gui_DrawNum_GBK16(210,30,BLUE,YELLOW,PowerVoltage);//Һ��������ʾ��ѹֵ
//			printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
			if(PowerVoltage <7)
			{
				printf("\t*****************\t Error :Power Fault!\t*************\r\n");
			}
		}
		
		if(Flag_TaskTemp)//���PCB�¶�
		{
			Flag_TaskTemp = 0;
			if(1)
				FAN_ON;
			else
				FAN_OFF;
		}
		if(Flag_TaskDataDtorage)//���ݴ洢����
		{
			Flag_TaskDataDtorage=0;
			TaskDataDtorage();
		}
		if(Flag_TaskReadEncode)//ˢ�±��뿪�ص�ֵ,10ms/��
		{
			Flag_TaskReadEncode=0;
			NowcountFreq = TIM_GetCounter(TIM4);
			if(NowcountFreq != LastcountFreq)
				TaskEncodeUpdate_Freq(NowcountFreq);
			NowcountAmpl = TIM_GetCounter(TIM2);
			if(NowcountAmpl != LastcountAmpl)
				TaskEncodeUpdate_Ampl(NowcountAmpl);
		}
		
		if(Flag_TaskDisplayUpdate)//����Һ����ʾ
		{
			Flag_TaskDisplayUpdate = 0;
			TaskDisplayUpdate();
//			printf("%d\r\n",(u16)NowAmplOut[1]);
//			TIM_SetCompare1(TIM8,(u16)NowAmplOut[1]*360/100);
//			TIM_SetCompare2(TIM3,NowAmplOut[2]*360/100);	//ͨ��һ����23�ķ�ֵ��ռ�ձȲ���
		}
  }
}



//��������������ʱɨ���ֵ��ʽ//��δʹ�á�
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

//ͨ���л������������ı�LEDָʾ��
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
		NowFreqOut[NowSelect-1] -= ValueChanged*StepFreq;
		if(NowFreqOut[NowSelect-1] <100)NowFreqOut[NowSelect-1]=100;//�����޷�
		else if(NowFreqOut[NowSelect-1] >1500)NowFreqOut[NowSelect-1]=100;//�������������������Ȼ�����޷�
	}
	else//Ƶ������
	{
		ValueChanged = (nowcount - LastcountFreq)/2;
		LastcountFreq = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		NowFreqOut[NowSelect-1] += ValueChanged*StepFreq;
		if(NowFreqOut[NowSelect-1] >1500)NowFreqOut[NowSelect-1]=1500;//�����޷�
	}
	Flag_TaskDisplayUpdate=1;//���ݷ����ı䣬���������ʾ
	Flag_TaskDataDtorage=1;//�������޸ģ������¼
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//������PWM��������ء�
//	printf("Freq is %d ;Ampl is %d\r\n",NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//�����һ����
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
		
		NowAmplOut[NowSelect-1] -= ValueChanged*StepAmpl;
		if(NowAmplOut[NowSelect-1] <11)NowAmplOut[NowSelect-1]=11;//�����޷�
		else if(NowAmplOut[NowSelect-1] >99)NowAmplOut[NowSelect-1]=11;//�����������
	}
	else//��ֵ����
	{
		ValueChanged = (nowcount - LastcountAmpl)/2;
		LastcountAmpl = nowcount;//������ʷ����
		if(ValueChanged>EncodeMaxChange) return;//һ��ʱ���ڸı���̫���ж�Ϊ��������������Ա��β���
		NowAmplOut[NowSelect-1] += ValueChanged*StepAmpl;	
		if(NowAmplOut[NowSelect-1] >99)NowAmplOut[NowSelect-1]=99;//�����޷�
	}
	Flag_TaskDisplayUpdate=1;//���ݷ����ı䣬���������ʾ
	Flag_TaskDataDtorage=1;//�������޸ģ������¼
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//������PWM��������ء�
//	printf("Freq is %d ;Ampl is %d\r\n",NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//�����һ����
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
//		printf("KEY1\r\n");
		if(LED_EN_OC1 == 1)//����ر����
			OC1OutputEnable = 0;
		else //�����������
			OC1OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC1);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line10);	//����жϱ�־λ
		return;
	}
		
	//����2
	else if(KEY_EN_OC2 == 0)//����������
	{
		LED_EN_OC2 = !LED_EN_OC2;
//		printf("KEY2\r\n");
		if(LED_EN_OC2 == 1)//����ر����
			OC2OutputEnable = 0;
		else //�����������
			OC2OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC2);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line11);	//����жϱ�־λ
		return;
	}
	
	//����3
	else if(KEY_EN_OC3 == 0)//����������
	{
		LED_EN_OC3 = !LED_EN_OC3;
//		printf("KEY3\r\n");
		if(LED_EN_OC3 == 1)//����ر����
			OC3OutputEnable = 0;
		else //�����������
			OC3OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC3);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line12);	//����жϱ�־λ
		return;
	}

	//����4
	else if(KEY_OC_Switch == 0)//����������
	{
		SelectCount++;
		NowSelect = SelectCount%3+1;
		TaskSwitchSelectSwitch(NowSelect);//�ı�LED��ָʾ
		Flag_TaskDisplayUpdate=1;//�ı�Һ��������ʾ
//		printf("KEY4\r\n");
		while(!KEY_OC_Switch);//���ּ��
		EXTI_ClearITPendingBit(EXTI_Line13);	//����жϱ�־λ
		return;
	}
//	printf("KEY BUTTON\r\n");
	EXTI_ClearITPendingBit(EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13);	//����жϱ�־λ
}
//

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
				StepFreq = 1;break;
			case 2:
				StepFreq = 10;break;
			case 3:
				StepFreq = 50;break;
		}
	}
	Flag_TaskDisplayUpdate=1;//���ݷ����ı䣬���������ʾ
	EXTI_ClearITPendingBit(EXTI_Line5);	//����жϱ�־λ
}
//

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
				StepAmpl = 1;break;
			case 2:
				StepAmpl = 5;break;
			case 3:
				StepAmpl = 10;break;
		}
	}
	Flag_TaskDisplayUpdate=1;//���ݷ����ı䣬���������ʾ
	EXTI_ClearITPendingBit(EXTI_Line3);	//����жϱ�־λ
}
///

//Һ����������ʾ����
void TaskDisplayUpdate(void)
{
	//ͨ��ѡ�б�������ָʾ
	Gui_DrawFont_GBK16(100,200,BLUE,WHITE," OC1 ");//�Ȼָ�Ĭ����ɫ
	Gui_DrawFont_GBK16(160,200,BLUE,WHITE," OC2 ");
	Gui_DrawFont_GBK16(220,200,BLUE,WHITE," OC3 ");
	if(NowSelect == 1)
	{
		Gui_DrawFont_GBK16(100,200,BLACK,RED," OC1 ");
	}
	else if(NowSelect == 2)
	{
		Gui_DrawFont_GBK16(160,200,BLACK,GREEN," OC2 ");
	}
	else if(NowSelect == 3)
	{
		Gui_DrawFont_GBK16(220,200,BLACK,BLUE," OC3 ");
	}
	
	//ʹ��ָʾ
	if(!OC1OutputEnable) Gui_DrawFont_GBK16(100,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(100,110,BLUE,YELLOW," Open ");
	if(!OC2OutputEnable) Gui_DrawFont_GBK16(160,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(160,110,BLUE,YELLOW," Open ");
	if(!OC3OutputEnable) Gui_DrawFont_GBK16(220,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(220,110,BLUE,YELLOW," Open ");
	
	

	//OC1����ʾ
	Gui_DrawNum_GBK16(100,180,BLUE,GRAY0,NowFreqOut[0]);
	Gui_DrawNum_GBK16(100,160,BLUE,GRAY0,NowAmplOut[0]);
//	Gui_DrawNum_GBK16(100,140,BLUE,GRAY0,NowCurrOut[0]);//��ʾ����
	//OC2����ʾ
	Gui_DrawNum_GBK16(160,180,BLUE,GRAY0,NowFreqOut[1]);
	Gui_DrawNum_GBK16(160,160,BLUE,GRAY0,NowAmplOut[1]);
//	Gui_DrawNum_GBK16(160,140,BLUE,GRAY0,NowCurrOut[1]);//��ʾ����
	//OC3����ʾ
	Gui_DrawNum_GBK16(220,180,BLUE,GRAY0,NowFreqOut[2]);
	Gui_DrawNum_GBK16(220,160,BLUE,GRAY0,NowAmplOut[2]);
//	Gui_DrawNum_GBK16(220,140,BLUE,GRAY0,NowCurrOut[2]);//��ʾ����
	//��������ʾ
	Gui_DrawNum_GBK16(280,180,BLUE,GRAY0,StepFreq);
	Gui_DrawNum_GBK16(280,160,BLUE,GRAY0,StepAmpl);

}

//���ݴ洢����
void TaskDataDtorage(void)
{
	u8 i=0;
	i=0;//�洢��ֵ
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[0]);i++;
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[1]);i++;
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[2]);i++;
	i=0;//�洢Ƶ��
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[0]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[0]%256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[1]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[1]%256);i++;	
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[2]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[2]%256);i++;
	//д��־λ
	StorageWriteOneByte(Adress_StorageFlag,0x55);	
}


//�ϵ��Լ죬��ȡ��ʷ����
void HistoryDataCheck(void)
{
	u8 i;
	if(StorageReadOneByte(Adress_StorageFlag) != 0x55)
		return;	//��һ��ʹ�ô洢��û����ʷ���ݣ�ֱ���˳�����
	
	for(i=0;i<3;i++)
		NowAmplOut[i] = StorageReadOneByte(Adress_AmplOut+i);//��ȡ�ϴεķ�ֵ������Ϊ��ǰ���
	
	for(i=0;i<6;i++)
	{
		NowFreqOut[i/2] = StorageReadOneByte(Adress_FreqOut+i);i++;
		NowFreqOut[i/2] <<= 8;
		NowFreqOut[i/2] += StorageReadOneByte(Adress_FreqOut+i);
	}
}



