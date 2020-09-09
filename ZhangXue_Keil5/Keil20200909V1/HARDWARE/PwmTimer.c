


/************************************************************************
	
************************************************************************//*
2020/07/26/10:17
	��ʼ����ʱ��˳�����⣬����Proteus����ʧ�ܣ�����Ϊֻ���ȳ�ʼ��TIM1������֮��ĳ�ʼ����Ч

*************************************************************************/

#include "PwmTimer.h"







//TIM8��TIM3һ����
void PWM_BaseTimeInit(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO�ṹ��
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
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
	TIM_TimeBaseInitStruct.TIM_Period=360-1;		//143		9999	����ǰ��׼Ƶ��Ϊ200kHz��
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;		//0		14499			//����ʱʱ��Ϊ1.25us����׼Ƶ��800kHz��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//ͨ����ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//�Ƚϻ������ʹ��
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //�������
//	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//�����������
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//ָ������״̬�µ�TIM����Ƚϵ�����״̬��
//	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//ָ������״̬�µ�TIM��������Ƚϵ�����״̬��

		//ͨ��1
		TIM_OCInitStruct.TIM_Pulse=55;//�Ƚ�ֵ������С�ڸ�ֵΪ�ߵ�ƽ�����ڸ�ֵΪ�͵�ƽ
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		TIM_OCInitStruct.TIM_Pulse=55;
		TIM_OC2Init(TIM3, &TIM_OCInitStruct);   
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
//		//ͨ��2
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC3Init(TIM3, &TIM_OCInitStruct);   
//		TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
//		//ͨ��3
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC4Init(TIM3, &TIM_OCInitStruct);   
//		TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);


	TIM_ARRPreloadConfig(TIM8, ENABLE);//����װ��ֵ ENABLE ������Ч��DISABLE ��һ���Ƚ�������Ч
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//ʹ��PWM��Χ��
//	TIM_CtrlPWMOutputs(TIM3, ENABLE);//ʹ��PWM��Χ��
	TIM_Cmd(TIM8, ENABLE);//ʹ�ܶ�ʱ��1
//	TIM_Cmd(TIM3, ENABLE);//ʹ�ܶ�ʱ��1
}

















/****************************************************************************************
ͨ����Դ��
	TIM8_CH1----->PC6			TIM8_CH1N------->PA7
	TIM8_CH2----->PC7			TIM8_CH2N------->PB0
	TIM8_CH3----->PC8 		TIM8_CH3N------->PB1
**********************************************************************************************/
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
	TIM_TimeBaseInitStruct.TIM_Period=360-1;		//143		9999
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
		TIM_OCInitStruct.TIM_Pulse=180;//�Ƚ�ֵ������С�ڸ�ֵΪ�ߵ�ƽ�����ڸ�ֵΪ�͵�ƽ
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//ͨ��2
//		TIM_OCInitStruct.TIM_Pulse=180;
//		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		//ͨ��3
//		TIM_OCInitStruct.TIM_Pulse=180;
//		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);


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




//ͨ��1�����ص�ͨ��3�����100---200000Hz�ķ���
void SetPwmVal(u32 Freq,u16 Duty,u8 OCx)
{
	u32 period;
	u32 prescaler;
	u16 pulse;
	
	//�ж�Ƶ�ʷ�Χ
	if(Freq<10000)//��Ƶ��100---10 000
	{
		prescaler=100-1;
		period=Count_Clock/(Freq*(prescaler+1))-1;
	}
	else
	{
		prescaler=0;
		period=Count_Clock/Freq-1;
	}
	pulse=(period+1)*Duty/100;	//�����������ֵ����ռ�ձȣ�
	
	
	//����ͨ��д�Ĵ���
//	if(OCx == 0)
	TIM8->PSC = prescaler;//д��Ƶϵ��
	TIM8->ARR = period;//д�Զ���װ��ֵ
	TIM8->CCR1 = pulse;//дռ�ձ�
	
//	TIM_SetAutoreload(TIM8,period);//���������Զ���װ��ֵ
//	TIM_SetCompare1(TIM8,pulse);
//	TIM_SetCompare2(TIM8,pulse);
}
