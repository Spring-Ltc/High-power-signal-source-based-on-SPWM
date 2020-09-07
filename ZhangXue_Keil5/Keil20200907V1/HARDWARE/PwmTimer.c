


/************************************************************************
	
************************************************************************//*
2020/07/26/10:17
	��ʼ����ʱ��˳�����⣬����Proteus����ʧ�ܣ�����Ϊֻ���ȳ�ʼ��TIM1������֮��ĳ�ʼ����Ч

*************************************************************************/

#include "PwmTimer.h"






//u16 CCR2_Val = 500;
//u16 CCR3_Val = 500;//ռ�ձȣ�����Ϊ1000

/****************************************************************************************
ͨ����Դ��
	TIM8_CH1----->PC6			TIM8_CH1N------->PA7
	TIM8_CH2----->PC7			TIM8_CH2N------->PB0
	TIM8_CH3----->PC8 		TIM8_CH3N------->PB1
��ڲ�����
	BaseFreq����׼Ƶ�ʣ�����С��PWMƵ��
**********************************************************************************************/
//void PWM_Time8Init(u32 BaseFreq)
void PWM_Time8Init(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO�ṹ��
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef    	  TIM_BDTRInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//TIM_Cmd(TIM1, ENABLE);//ʹ�ܶ�ʱ��1
	//ʹ��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	//GPIO��ʼ������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;	//PA7
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PB0��PB1
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//PC6��PC7��PC8
		GPIO_Init(GPIOC, &GPIO_InitStruct);

	//��ʱ��������ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//���Բ������ã���Ϊ���������
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=143;		//143		9999
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;		//0		14499			//����ʱʱ��Ϊ2us����׼Ƶ��500kHz��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	
	//ͨ����ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//�Ƚϻ������ʹ��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //�������
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//�����������
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//ָ������״̬�µ�TIM����Ƚϵ�����״̬��
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//ָ������״̬�µ�TIM��������Ƚϵ�����״̬��
		//ͨ��1
		TIM_OCInitStruct.TIM_Pulse=72;//�Ƚ�ֵ������С�ڸ�ֵΪ�ߵ�ƽ�����ڸ�ֵΪ�͵�ƽ
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//ͨ��2
		TIM_OCInitStruct.TIM_Pulse=72;
		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//ͨ��3
		TIM_OCInitStruct.TIM_Pulse=72;
		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);


	//��������
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_DeadTime = 27; //����ʱ��  72:1us 172:3us 205:5us
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);

	TIM_ARRPreloadConfig(TIM8, ENABLE);//����װ��ֵ ENABLE ������Ч��DISABLE ��һ���Ƚ�������Ч
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//ʹ��PWM��Χ��
	TIM_Cmd(TIM8, ENABLE);//ʹ�ܶ�ʱ��1
}



//ɢ�ȷ��ȵ��٣�������Կ�Ҫ��Ҫ�ŵ����뿪��EC11һ���ʼ��
//PB9----TIM4_CH4
void FanPWM_TIM4Init(void)
{
	
}

