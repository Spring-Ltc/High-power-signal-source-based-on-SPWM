

//ϵͳ����ʱ����ʱ��Ƭ����
//��ʱ����TIM1
//��ʱ������������
//	1���¶ȼ������
//	2�������������
//	3��PID�����������񣨸���ʱ�俼���������ü���д��
//	4�������ѹ�������


#include "TaskTimer.h"
#include "CommonGPIO.h"


u8 Flag_TaskTemp=0;//�¶ȼ������
u8 Flag_TaskPower=0;//��Դ��ѹ�������
u8 Flag_TaskCurrent=0;//�����������
u8 Flag_TaskReadEncode=0;//���뿪��ɨ������
u8 Flag_TaskDisplayUpdate=0;//Һ��������ʾ����
u8 Flag_TaskDataDtorage=0;//���ݴ洢����


u8 Count_TaskTemp=0;
u8 Count_TaskPower=0;
u8 Count_TaskDisplay=0;





//������С��ʱ�жϣ�10ms
//��ʱʱ�� Tout = (period+1)*(prescaler+1)/TIMx_CLK		����ʱ��ʱ�Ӳο�STM32��ʱ����timetree��
void TaskTime_Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=99;             // �Զ���װ�ؼĴ�����ֵ	������99+1���μ�10*100 us = 10ms
	TIM_TimeBaseStruct.TIM_Prescaler=7199;            // ʱ��Ԥ��Ƶ����ÿ��7199+1��/72M = 100us����һ��
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ//[����������ã����Բ�����]
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//�ظ��Ĵ���  //[�߼���ʱ�����õ������Բ�����] 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_Cmd(TIM1,ENABLE); //����ʱ��
}



//10ms����һ���ж�
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		Flag_TaskCurrent = 1;//10ms���һ�ε���
		Flag_TaskReadEncode = 1;//10ms��ȡһ��EC11���뿪�ص�ֵ
		
		Count_TaskTemp++;
		if(Count_TaskTemp >= 10)//100ms���һ���¶�
		{
			Flag_TaskTemp = 1;
			Count_TaskTemp = 0;
		}
		
		Count_TaskPower++;
		if(Count_TaskPower >= 100)//1000ms���һ�ε�Դ��ѹ
		{
			Flag_TaskPower = 1;
			Count_TaskPower = 0;
		}
		
//		Count_TaskDisplay++;
//		if(Count_TaskPower >= 40)//200ms����һ����ʾ
//		{
////			Flag_TaskDisplayUpdate = 1;	//һֱ����ռ��ʱ�䣬��Ϊ�����ͱ��뿪�ز�����λ
//			Count_TaskDisplay = 0;
//		}

	}
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
}


