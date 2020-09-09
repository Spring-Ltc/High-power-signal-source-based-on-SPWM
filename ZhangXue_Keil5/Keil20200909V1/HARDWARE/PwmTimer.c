


/************************************************************************
	
************************************************************************//*
2020/07/26/10:17
	初始化定时器顺序问题，导致Proteus仿真失败，现象为只能先初始化TIM1，而且之后的初始化无效

*************************************************************************/

#include "PwmTimer.h"







//TIM8和TIM3一起用
void PWM_BaseTimeInit(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO结构体
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	//GPIO初始化设置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;	//PA7
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PB0、PB1
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//PC6、PC7、PC8
		GPIO_Init(GPIOC, &GPIO_InitStruct);

	//定时器基本初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//可以不用设置，作为输入才有用
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=360-1;		//143		9999	【当前基准频率为200kHz】
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;		//0		14499			//【定时时间为1.25us，基准频率800kHz】
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	//通道初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//比较互补输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
//	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM输出比较的引脚状态。
//	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。

		//通道1
		TIM_OCInitStruct.TIM_Pulse=55;//比较值，计数小于该值为高电平，大于该值为低电平
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		TIM_OCInitStruct.TIM_Pulse=55;
		TIM_OC2Init(TIM3, &TIM_OCInitStruct);   
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
//		//通道2
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC3Init(TIM3, &TIM_OCInitStruct);   
//		TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
//		//通道3
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		TIM_OCInitStruct.TIM_Pulse=60;
//		TIM_OC4Init(TIM3, &TIM_OCInitStruct);   
//		TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);


	TIM_ARRPreloadConfig(TIM8, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//使能PWM外围输
//	TIM_CtrlPWMOutputs(TIM3, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM8, ENABLE);//使能定时器1
//	TIM_Cmd(TIM3, ENABLE);//使能定时器1
}

















/****************************************************************************************
通道资源：
	TIM8_CH1----->PC6			TIM8_CH1N------->PA7
	TIM8_CH2----->PC7			TIM8_CH2N------->PB0
	TIM8_CH3----->PC8 		TIM8_CH3N------->PB1
**********************************************************************************************/
void PWM_Time8Init(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO结构体
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef    	  TIM_BDTRInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//TIM_Cmd(TIM1, ENABLE);//使能定时器1
	//使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

	//GPIO初始化设置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;	//PA7
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	//PB0、PB1
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//PC6、PC7、PC8
		GPIO_Init(GPIOC, &GPIO_InitStruct);

	//定时器基本初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//可以不用设置，作为输入才有用
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=360-1;		//143		9999
	TIM_TimeBaseInitStruct.TIM_Prescaler=0;		//0		14499			//【定时时间为2us，基准频率500kHz】
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	
	//通道初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//比较互补输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM输出比较的引脚状态。
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。
		//通道1
		TIM_OCInitStruct.TIM_Pulse=180;//比较值，计数小于该值为高电平，大于该值为低电平
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//通道2
//		TIM_OCInitStruct.TIM_Pulse=180;
//		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
//		//通道3
//		TIM_OCInitStruct.TIM_Pulse=180;
//		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
//		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);


	//死区配置
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_DeadTime = 27; //死区时间  72:1us 172:3us 205:5us
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);

	TIM_ARRPreloadConfig(TIM8, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM8, ENABLE);//使能定时器1
}




//通道1，板载的通道3，输出100---200000Hz的方波
void SetPwmVal(u32 Freq,u16 Duty,u8 OCx)
{
	u32 period;
	u32 prescaler;
	u16 pulse;
	
	//判断频率范围
	if(Freq<10000)//低频，100---10 000
	{
		prescaler=100-1;
		period=Count_Clock/(Freq*(prescaler+1))-1;
	}
	else
	{
		prescaler=0;
		period=Count_Clock/Freq-1;
	}
	pulse=(period+1)*Duty/100;	//计算输出脉宽值（即占空比）
	
	
	//根据通道写寄存器
//	if(OCx == 0)
	TIM8->PSC = prescaler;//写分频系数
	TIM8->ARR = period;//写自动重装初值
	TIM8->CCR1 = pulse;//写占空比
	
//	TIM_SetAutoreload(TIM8,period);//重新设置自动重装初值
//	TIM_SetCompare1(TIM8,pulse);
//	TIM_SetCompare2(TIM8,pulse);
}
