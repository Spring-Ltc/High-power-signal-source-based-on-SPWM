

//系统任务定时器，时间片分配
//暂时分配TIM1
//定时触发的任务有
//	1、温度检测任务
//	2、电流检测任务
//	3、PID控制周期任务（根据时间考虑来不来得及再写）
//	4、供电电压检测任务


#include "TaskTimer.h"
#include "CommonGPIO.h"


u8 Flag_TaskTemp=0;//温度检测任务
u8 Flag_TaskPower=0;//电源电压检测任务
u8 Flag_TaskCurrent=0;//电流检测任务
u8 Flag_TaskReadEncode=0;//编码开关扫描任务
u8 Flag_TaskDisplayUpdate=0;//液晶更新显示任务
u8 Flag_TaskDataDtorage=0;//数据存储任务


u8 Count_TaskTemp=0;
u8 Count_TaskPower=0;
u8 Count_TaskDisplay=0;





//设置最小定时中断，10ms
//定时时间 Tout = (period+1)*(prescaler+1)/TIMx_CLK		（定时器时钟参考STM32的时钟树timetree）
void TaskTime_Init(void)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=99;             // 自动重装载寄存器的值	计数（99+1）次即10*100 us = 10ms
	TIM_TimeBaseStruct.TIM_Prescaler=7199;            // 时钟预分频数，每（7199+1）/72M = 100us计数一次
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频//[做输入才有用，可以不设置]
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//重复寄存器  //[高级定时器才用到，可以不设置] 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE); //使能中断
	TIM_Cmd(TIM1,ENABLE); //开启时钟
}



//10ms产生一次中断
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update))	 //判断是否定时中断
	{
		Flag_TaskCurrent = 1;//10ms检测一次电流
		Flag_TaskReadEncode = 1;//10ms读取一次EC11编码开关的值
		
		Count_TaskTemp++;
		if(Count_TaskTemp >= 10)//100ms检测一次温度
		{
			Flag_TaskTemp = 1;
			Count_TaskTemp = 0;
		}
		
		Count_TaskPower++;
		if(Count_TaskPower >= 100)//1000ms检测一次电源电压
		{
			Flag_TaskPower = 1;
			Count_TaskPower = 0;
		}
		
//		Count_TaskDisplay++;
//		if(Count_TaskPower >= 40)//200ms更新一次显示
//		{
////			Flag_TaskDisplayUpdate = 1;	//一直更新占用时间，改为按键和编码开关操作置位
//			Count_TaskDisplay = 0;
//		}

	}
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
}


