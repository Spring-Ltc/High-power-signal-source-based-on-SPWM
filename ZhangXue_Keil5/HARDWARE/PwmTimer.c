


/************************************************************************
输出通道1：
		TIM1_CH1N------> PB13/PA7
		TIM1_CH1 ------> PA8
输出通道2：
		TIM8_CH3N------> PB1
		TIM8_CH3 ------> PC8
输出通道1：
		TIM4_CH3------> PB8
		TIM4_CH4 ------> PB9		
************************************************************************//*
2020/07/26/10:17
	初始化定时器顺序问题，导致Proteus仿真失败，现象为只能先初始化TIM1，而且之后的初始化无效

*************************************************************************/

#include "PwmTimer.h"




void PWM_Init(u32 freq)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO结构体
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef    	  TIM1_BDTRInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	//使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8|RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
		//GPIO初始化设置
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1| GPIO_Pin_13;	//PB0、PB1
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//PC6、PC7、PC8
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//定时器基本初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=999;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数？？？
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	
	//通道初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//比较互补输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;//互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM输出比较的引脚状态。
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。
	//通道1
	TIM_OCInitStruct.TIM_Pulse=499;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);   
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//通道2
	TIM_OCInitStruct.TIM_Pulse=299;
	TIM_OC2Init(TIM1, &TIM_OCInitStruct); 
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//通道1
	TIM_OCInitStruct.TIM_Pulse=499;
	TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	//通道2
	TIM_OCInitStruct.TIM_Pulse=299;
	TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	
	//死区配置
	TIM1_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM1_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM1_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM1_BDTRInitStruct.TIM_DeadTime = 0; //死区时间  72:1us 172:3us 205:5us
	TIM1_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM1_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM1_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStruct);
	TIM_BDTRConfig(TIM8,&TIM1_BDTRInitStruct);


	TIM_ARRPreloadConfig(TIM1, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM1, ENABLE);//使能定时器1
	TIM_ARRPreloadConfig(TIM8, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM8, ENABLE);//使能定时器1
	
	
	
}



u16 CCR2_Val = 500;
u16 CCR3_Val = 500;//占空比，周期为1000

void PWM_Time1Init(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO结构体
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef    	  TIM1_BDTRInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	//使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	//GPIO初始化设置
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	//【复用管脚，F1好像没有？？？】
	
	//定时器基本初始化
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=999;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数？？？
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	//通道初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//比较互补输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;//互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM输出比较的引脚状态。
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。
	//通道1
	TIM_OCInitStruct.TIM_Pulse=499;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);   
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
	//通道2
	TIM_OCInitStruct.TIM_Pulse=299;
	TIM_OC2Init(TIM1, &TIM_OCInitStruct); 
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
	
	//死区配置
	TIM1_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM1_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM1_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM1_BDTRInitStruct.TIM_DeadTime = 100; //死区时间  72:1us 172:3us 205:5us
	TIM1_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM1_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM1_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRConfig(TIM1,&TIM1_BDTRInitStruct);

	TIM_ARRPreloadConfig(TIM1, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM1, ENABLE);//使能定时器1
	
}
void PWM_Time4Init(void)
{

}



/****************************************************************************************
通道资源：
	TIM8_CH1----->PC6			TIM8_CH1N------->PA7
	TIM8_CH2----->PC7			TIM8_CH2N------->PB0
	TIM8_CH3----->PC8 		TIM8_CH3N------->PB1
入口参数：
	BaseFreq：基准频率，即最小的PWM频率
**********************************************************************************************/
//void PWM_Time8Init(u32 BaseFreq)
void PWM_Time8Init(void)
{
	GPIO_InitTypeDef 					GPIO_InitStruct;//GPIO结构体
	TIM_TimeBaseInitTypeDef	  TIM_TimeBaseInitStruct;
	TIM_BDTRInitTypeDef    	  TIM_BDTRInitStruct;
	TIM_OCInitTypeDef					TIM_OCInitStruct;

	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);//使能定时器1
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
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=999;
	TIM_TimeBaseInitStruct.TIM_Prescaler=71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;//重复计数？？？
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
	
	//通道初始化
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//比较互补输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;//互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM输出比较的引脚状态。
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。
		//通道1
		TIM_OCInitStruct.TIM_Pulse=199;
		TIM_OC1Init(TIM8, &TIM_OCInitStruct);   
		TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//通道2
		TIM_OCInitStruct.TIM_Pulse=399;
		TIM_OC2Init(TIM8, &TIM_OCInitStruct); 
		TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
		//通道3
		TIM_OCInitStruct.TIM_Pulse=599;
		TIM_OC3Init(TIM8, &TIM_OCInitStruct); 
		TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);


	//死区配置
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStruct.TIM_DeadTime = 0; //死区时间  72:1us 172:3us 205:5us
	TIM_BDTRInitStruct.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRInitStruct.TIM_Break=TIM_Break_Disable;
	TIM_BDTRInitStruct.TIM_BreakPolarity=TIM_BreakPolarity_High;
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);

	TIM_ARRPreloadConfig(TIM8, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效
	TIM_CtrlPWMOutputs(TIM8, ENABLE);//使能PWM外围输
	TIM_Cmd(TIM8, ENABLE);//使能定时器1
}




