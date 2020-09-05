

#include "Encoder.h"

//初始化旋转编码开关EC11的相关定时器及按键
//	Encode1：	A/PA0/TIM2_CH1
//						B/PA1/TIM2_CH2
//						KEY/PB5
//	Encode2:	A/PB6/TIM4_CH1
//						B/PB7/TIM4_CH2
//						KEY/PC3
void EC11Encoder_Init(void)
{
	GPIO_InitTypeDef 						GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef 		TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef						TIM_ICInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM2,ENABLE);//定时器时钟使能

	//GPIO初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		//两个按键和定时器输入通道的GPIO口
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
		GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//定时器基本参数配置
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStruct.TIM_Period=0xffff;
	TIM_TimeBaseInitStruct.TIM_Prescaler=0x0000;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	//配置编码器模式
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI1,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI1,TIM_ICPolarity_Falling,TIM_ICPolarity_Falling);
	
//	TIM_ICInitStruct.TIM_Channel=
//	TIM_ICInitStruct.TIM_ICFilter=
//	TIM_ICInitStruct.TIM_ICPolarity=
//	TIM_ICInitStruct.TIM_ICPrescaler=
//	TIM_ICInitStruct.TIM_ICSelection=
		TIM_ICInit(TIM2,&TIM_ICInitStruct);
		TIM_ICInit(TIM4,&TIM_ICInitStruct);
		
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}





