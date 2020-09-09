

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
u8 SPWMState=1;	//SPWM����״̬��
u8 PointCount;//ʵ�ʵķ��ظ�������

//������Կ��ƿ���
u8 HighOutput=1;
u8 LowOutput=1;

//���ʹ�ܿ������
u8 OC1OutputEnable=0;//Ĭ�ϲ����
u8 OC2OutputEnable=0;
u8 OC3OutputEnable=0;

void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=1800-1;             // �Զ���װ�ؼĴ�����ֵ	72M/1800=40khz
	TIM_TimeBaseStruct.TIM_Prescaler=0;            // ʱ��Ԥ��Ƶ��
//	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // ������Ƶ
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//�ظ��Ĵ����������Զ�����pwmռ�ձ�    
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM6,TIM_FLAG_Update); //�����ʱ���жϱ�־
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //ʹ���ж�
	TIM_Cmd(TIM6,ENABLE); //����ʱ��
}








//��ʱ25us����PWMռ�ձȺͷ���
//��ʱֻ���ͨ��1����������ͨ��3
void TIM6_IRQHandler(void)
{
	static u8 count=0;
	count++;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))	 //�ж��Ƿ�ʱ�ж�
	{
		//ǰ1/4����---------------------------------------------------
		if(SPWMState == 1)
		{
			TIM_SetCompare1(TIM8,Sin_Table[0][count]);
			if(HighOutput)//�ر�TIM3���
			{
				TIM_SetCompare2(TIM3,0);
				HighOutput=0;//ֻ��һ��
			}
			if(count > PointCount)
			{
				count=0;SPWMState=2;//�����������������һ��״̬
			}
		}
		//ǰ2/4����---------------------------------------------------
		else if(SPWMState == 2)
		{
			TIM_SetCompare1(TIM8,Sin_Table[0][PointCount-count]);
			if(count > PointCount)
			{
				count=0;SPWMState=3;//�����������������һ��״̬
				LowOutput=1;//��һ״̬�ر��������ִֻ��һ�ε���
			}
		}
		//ǰ3/4����---------------------------------------------------
		else if(SPWMState == 3)//ǰ3/4����
		{

			TIM_SetCompare2(TIM3,Sin_Table[0][count]);
			if(LowOutput)
			{
				TIM_SetCompare1(TIM8,0);
				LowOutput=0;
			}
			if(count > PointCount)
			{
				count=0;SPWMState=4;//�����������������һ��״̬
			}
		}
		//ǰ4/4����---------------------------------------------------
		else if(SPWMState == 4)//ǰ4/4����
		{
			TIM_SetCompare2(TIM3,Sin_Table[0][PointCount-count]);
			if(count > PointCount)
			{
				count=0;SPWMState=1;//�����������������һ��״̬
				HighOutput=1;//��һ״̬�رշ������ִֻ��һ�ε���
			}
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);//��������жϱ�־λ����һֱ�ж�
}

//OCx��	1��2��3
//Freq��100hz----1000Hz
//Ampl��0-----99
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl)
{
	u16 SamplingCount;
	u8 i;
	float MaxAmpl;
	//�Ƚ��в�����Ч���ж�
	if((Freq<100)||(Freq>1000))return;
	if(Ampl>99)return;
	
	//�ȸ���Ƶ�ʼ����������
//	printf("Ampl is %d\r\n",Ampl);
//	Ampl &=0x7f;//�����λ����λдΪ1����Ϊ�з�����
//	printf("Ampl is %d\r\n",Ampl);
//	printf("Ampl is %f\r\n",(float)Ampl);
	SamplingCount = 40000/Freq/4;//���㲻�ظ��Ĳ����㣬��1/4���ڵĲ�����
	MaxAmpl = (float)(Ampl)/100*360;//������BUG
//	if(OCx == 1)
		PointCount = SamplingCount;
//	printf("Point count(1/4) is:  %d\r\n",PointCount);
	
//	printf("MaxAmpl = %f\r\n",MaxAmpl);
	//����ÿ���������ռ�ձ�
	for(i=0;i<SamplingCount;i++)
	{
		Sin_Table[OCx-1][i]=(u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
	}
	
//	for(i=0;i<SamplingCount;i++)
//	{
////		printf("%d\r\n",Sin_Table[OCx-1][i]);
//	}
//	printf("count = %d\r\n",SamplingCount);
}
