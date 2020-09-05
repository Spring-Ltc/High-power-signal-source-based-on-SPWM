

#include "CommonGPIO.h"


//按键配置为中断触发
void KEY_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启复用时钟
	
	//GPIO初始化		4个按键
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource10|GPIO_PinSource11|GPIO_PinSource12|GPIO_PinSource13);//中断映射
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource11);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource12);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource13);
	
	//中断线配置
	EXTI_InitStruct.EXTI_Line = EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct .EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);
	
	//中断优先级设置
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =0x02;
	NVIC_Init(&NVIC_InitStruct);
	
}








void LED_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//GPIO初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;		//LED_EN_OC1
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;		//LED_System
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_9;
		GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_SetBits(GPIOA,GPIO_Pin_8);//初始电平拉高，关闭LED
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);//上电默认当前选中通道0
}


void BUZZ_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;		//BUZZ
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);//初始电平拉低，关闭蜂鸣器
}



