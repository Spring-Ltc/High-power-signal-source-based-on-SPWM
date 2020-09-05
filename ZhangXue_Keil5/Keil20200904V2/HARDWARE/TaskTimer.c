

//ϵͳ����ʱ����ʱ��Ƭ����
//��ʱ����TIM1
//��ʱ������������
//	1���¶ȼ������
//	2�������������
//	3��PID�����������񣨸���ʱ�俼���������ü���д��
//	4�������ѹ�������


#include "TaskTimer.h"

u8 Flag_TaskTemp=0;
u8 Flag_TaskPower=0;
u8 Flag_TaskCurrent=0;
u8 Count_TaskTemp=0;
u8 Count_TaskPower=0;






//������С��ʱ�жϣ�5ms
//��ʱʱ�� Tout = (period+1)*(prescaler+1)/TIMx_CLK		����ʱ��ʱ�Ӳο�STM32��ʱ����timetree��
void TaskTime_Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=49;             // �Զ���װ�ؼĴ�����ֵ	������49+1���μ�50*100 us = 5ms
	TIM_TimeBaseStruct.TIM_Prescaler=7199;            // ʱ��Ԥ��Ƶ����ÿ��7199+1��/72M = 100us����һ��
	//TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ//[����������ã����Բ�����]
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	//TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//�ظ��Ĵ���  //[�߼���ʱ�����õ������Բ�����] 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);	
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_Cmd(TIM1,ENABLE); //����ʱ��
	
}



void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		Flag_TaskCurrent = 1;//5ms���һ�ε���
		
		Count_TaskTemp++;
		if(Count_TaskTemp >= 10)//50ms���һ���¶�
		{
			Flag_TaskTemp = 1;
			Count_TaskTemp = 0;
		}
		
		Count_TaskPower++;
		if(Count_TaskPower >= 100)//500ms���һ�ε�Դ��ѹ
		{
			Flag_TaskPower = 1;
			Count_TaskPower = 0;
		}

	}
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
}


