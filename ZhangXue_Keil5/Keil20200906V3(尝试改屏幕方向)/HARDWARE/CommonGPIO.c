

#include "CommonGPIO.h"
#include "delay.h"

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

	//中断线配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct .EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStruct);

	//中断优先级设置
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;
	NVIC_Init(&NVIC_InitStruct);
	
}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_EN_OC1==0||KEY_EN_OC2==0||KEY_EN_OC3==0||KEY_OC_Switch==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY_EN_OC1==0)
		{
			while(KEY_EN_OC1==0);//松手检测
			return KEY_EN_OC1_PRES;
		}
		else if(KEY_EN_OC2==0)
		{
			while(KEY_EN_OC2==0);//松手检测
			return KEY_EN_OC2_PRES;
		}
		else if(KEY_EN_OC3==0)
		{
			while(KEY_EN_OC3==0);//松手检测
			return KEY_EN_OC3_PRES;
		}
		else if(KEY_OC_Switch==0)
		{
			while(KEY_OC_Switch==0);//松手检测
			return KEY_OC_Switch_PRES;
		}
	}
	else if(KEY_EN_OC1==1&&KEY_EN_OC1==1&&KEY_EN_OC1==1&&KEY_OC_Switch==1)key_up=1; 	     
	return 0;// 无按键按下
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
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);
	LED_Select_OC1 = 0;//默认选中通道1
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



