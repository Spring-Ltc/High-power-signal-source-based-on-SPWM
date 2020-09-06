

#include "PIDcontrolSPWM.h"

//����Ҳ��Ҫһ����ʱ������ʱ�����л�ռ�ձȺ�ʱ�����ڣ���������SPWM��
//��ʱ����TIM3


void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=3600-1;             // �Զ���װ�ؼĴ�����ֵ	36M/1800=20khz
	TIM_TimeBaseStruct.TIM_Prescaler=1;            // ʱ��Ԥ��Ƶ��
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�    
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_Cmd(TIM3,ENABLE); //����ʱ��
	
	
}



void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
			{

//				TIM_SetCompare1(TIM8,Sin_Table[a]*8);	

			}
	   TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�


}

