

#include "PIDcontrolSPWM.h"
#include "CommonGPIO.h"
//����Ҳ��Ҫһ����ʱ������ʱ�����л�ռ�ձȺ�ʱ�����ڣ���������SPWM��
//��ʱ����TIM3
//202009071132��TIM3����PWM����ˣ����ﲻ������
//					ֻ�п���TIM6��TIM7��



//���ռ�ձ�Ϊ120
//������СƵ��100	Hz������10000  us��600���㣬150�����ظ���	
//�������Ƶ��1500Hz������666.66 us��40���㣬 10�����ظ���

//������
//u8 Sin_TableOC1[180]={0};
//u8 Sin_TableOC2[180]={0};
//u8 Sin_TableOC3[180]={0};
u16 Sin_Table[3][205];//��СƵ��50Hzʱ��1/4������200����


//״̬�����
u8 SPWMState1=1;	//SPWM����״̬��
u8 SPWMState2=1;
u8 SPWMState3=1;

u8 PointCount1;//ʵ�ʵķ��ظ�������
u8 PointCount2;//ʵ�ʵķ��ظ�������
u8 PointCount3;//ʵ�ʵķ��ظ�������

//������Կ��ƿ���
u8 HighOutput1=1;
u8 LowOutput1=1;
u8 HighOutput2=1;
u8 LowOutput2=1;
u8 HighOutput3=1;
u8 LowOutput3=1;
//���ʹ�ܿ������
u8 OC1OutputEnable=0;//Ĭ�ϲ����
u8 OC2OutputEnable=0;
u8 OC3OutputEnable=0;

void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM7|RCC_APB1Periph_TIM3,ENABLE);//Ҫ�õ�������ʱ��
		
	TIM_TimeBaseStruct.TIM_Period=1800-1;             // �Զ���װ�ؼĴ�����ֵ	72M/1800=40khz
	TIM_TimeBaseStruct.TIM_Prescaler=0;            // ʱ��Ԥ��Ƶ��
//	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�    
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStruct);	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
	
	//TIM6�ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	//TIM7�ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	//TIM3�ж����ȼ�
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
//	
	TIM_ClearFlag(TIM6,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ClearFlag(TIM7,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ClearFlag(TIM3,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_Cmd(TIM6,ENABLE); //����ʱ��
	TIM_Cmd(TIM7,ENABLE); //����ʱ��
	TIM_Cmd(TIM3,ENABLE); //����ʱ��
}
//��ʱ��������ʼ�����



//ͨ��3����������ͨ��1
void TIM3_IRQHandler(void)
{
	static u16 count1=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
		if(!OC1OutputEnable)
		{
			OutputOC3NState_Disable;OutputOC3State_Disable;
			return;
		}
		count1++;
		
		
		//ǰ1/4����---------------------------------------------------
		if(SPWMState1 == 1)
		{
			TIM8->CCR3 = Sin_Table[0][count1];//дռ�ձ�
			if(HighOutput1)//����������ڣ��رջ������
			{
				OutputOC3NState_Disable;//�رջ������
				OutputOC3State_Enable;
				HighOutput1=0;//ֻ��һ��
				LowOutput1=1;	
			}
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=2;//�����������������һ��״̬	
			}
		}
		else if(SPWMState1 ==2)//ǰ2/4����
		{
			TIM8->CCR3 = Sin_Table[0][PointCount1-count1];//дռ�ձ�
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=3;//�����������������һ��״̬	
			}
		}

		//��������---------------------------------------------------
		else if(SPWMState1 ==3)
		{
			TIM8->CCR3 = Sin_Table[0][count1];//дռ�ձ�
			if(LowOutput1)//����������ڣ��ر�ԭͨ�����
			{
				OutputOC3State_Disable;//�ر�ԭͨ�����
				OutputOC3NState_Enable;
				LowOutput1=0;
				HighOutput1=1;//ֻ��һ��
			}
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=4;//�����������������һ��״̬
			}
		}
		else if(SPWMState1 ==4)
		{
			TIM8->CCR3 = Sin_Table[0][PointCount1-count1];//дռ�ձ�
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=1;//�����������������һ��״̬	
			}
		}
	}

}
////OC1��̬SPWM

//��ʱ25us����PWMռ�ձȺͷ���
//��ʱֻ���ͨ��2����������ͨ��2
void TIM6_IRQHandler(void)
{
	static u16 count2=0;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
		if(!OC2OutputEnable)
		{
			OutputOC2NState_Disable;OutputOC2State_Disable;
			return;
		}
		count2++;
		
		
		//ǰ1/4����---------------------------------------------------
		if(SPWMState2 == 1)
		{
			TIM8->CCR2 = Sin_Table[1][count2];//дռ�ձ�
			if(HighOutput2)//����������ڣ��رջ������
			{
				OutputOC2NState_Disable;//�رջ������
				OutputOC2State_Enable;
				HighOutput2=0;//ֻ��һ��
				LowOutput2=1;	
			}
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=2;//�����������������һ��״̬	
			}
		}
		else if(SPWMState2 ==2)//ǰ2/4����
		{
			TIM8->CCR2 = Sin_Table[1][PointCount2-count2];//дռ�ձ�
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=3;//�����������������һ��״̬	
			}
		}

		//��������---------------------------------------------------
		else if(SPWMState2 ==3)
		{
			TIM8->CCR2 = Sin_Table[1][count2];//дռ�ձ�
			if(LowOutput2)//����������ڣ��ر�ԭͨ�����
			{
				OutputOC2State_Disable;//�ر�ԭͨ�����
				OutputOC2NState_Enable;
				LowOutput2=0;
				HighOutput2=1;//ֻ��һ��
			}
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=4;//�����������������һ��״̬
			}
		}
		else if(SPWMState2 ==4)
		{
			TIM8->CCR2 = Sin_Table[1][PointCount2-count2];//дռ�ձ�
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=1;//�����������������һ��״̬	
			}
		}
	}

}
//OC2��̬SPWM



////ͨ��1����������ͨ��3
void TIM7_IRQHandler(void)
{
	static u16 count3=0;
	if(TIM_GetITStatus(TIM7,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
		if(!OC3OutputEnable)
		{
			OutputOC1NState_Disable;OutputOC1State_Disable;
			return;
		}
		count3++;
		
		
		//ǰ1/4����---------------------------------------------------
		if(SPWMState3 == 1)
		{
				
			TIM8->CCR1 = Sin_Table[2][count3];//дռ�ձ�
			if(HighOutput3)//����������ڣ��رջ������
			{
				OutputOC1NState_Disable;//�رջ������
				OutputOC1State_Enable;
				HighOutput3=0;//ֻ��һ��
				LowOutput3=1;	
			}
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=2;//�����������������һ��״̬
			
			}
		}
		else if(SPWMState3 ==2)//ǰ2/4����
		{
			TIM8->CCR1 = Sin_Table[2][PointCount3-count3];//дռ�ձ�
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=3;//�����������������һ��״̬	
			}
		}

		//��������---------------------------------------------------
		else if(SPWMState3 ==3)
		{
			TIM8->CCR1 = Sin_Table[2][count3];//дռ�ձ�
			if(LowOutput3)//����������ڣ��ر�ԭͨ�����
			{
				OutputOC1State_Disable;//�ر�ԭͨ�����
				OutputOC1NState_Enable;
				LowOutput3=0;
				HighOutput3=1;//ֻ��һ��
			}
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=4;//�����������������һ��״̬
			}
		}
		else if(SPWMState3 ==4)
		{
			TIM8->CCR1 = Sin_Table[2][PointCount3-count3];//дռ�ձ�
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=1;//�����������������һ��״̬	
			}
		}
	}

}
////OC3��̬SPWM



//OCx��	1��2��3
//Freq��100hz----1000Hz	����Ƶ200kHz��
//Ampl��0-----99
u16 TableOffset = 2;//ռ�ձ�ƫ�ƽ���
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl)
{
	u16 SamplingCount;
	u8 i;
	float MaxAmpl;
	//�Ƚ��в�����Ч���ж�
	if((Freq<100)||(Freq>1000))return;
	if(Ampl>99)return;
	
	SamplingCount = 200000/5/Freq/4;//���㲻�ظ��Ĳ����㣬��1/4���ڵĲ�����
	if(SamplingCount>200)//�����㲻Ҫ�������������ޣ���Ƶ200k������360
		SamplingCount=200;
	
	MaxAmpl = (float)(Ampl)/100*360;//����ռ�ձȼ������������
	
	switch (OCx)
	{
		case 1:
			PointCount1 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//����ÿ���������ռ�ձ�
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
		case 2:
			PointCount2 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//����ÿ���������ռ�ձ�
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
		case 3:
			PointCount3 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//����ÿ���������ռ�ձ�
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
	}
	
	//����������Ҫע�͵�
	printf("�����ǰ��sin��OC %d\r\n",OCx);
	for(i=0;i<SamplingCount;i++)
	{
		printf("%d\r\n",Sin_Table[OCx-1][i]);
	}
	printf("count = %d\r\n",SamplingCount);
}
//����sinռ�ձȱ�OK




