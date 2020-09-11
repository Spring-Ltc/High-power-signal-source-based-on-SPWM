

#include "PIDcontrolSPWM.h"
#include "CommonGPIO.h"
//这里也需要一个定时器，定时到达切换占空比和时间周期，用以生成SPWM波
//暂时分配TIM3
//202009071132：TIM3用作PWM输出了，这里不能用了
//					只有考虑TIM6和TIM7了



//最大占空比为120
//设置最小频率100	Hz，周期10000  us，600个点，150个非重复点	
//设置最大频率1500Hz，周期666.66 us，40个点， 10个非重复点

//输出查表
//u8 Sin_TableOC1[180]={0};
//u8 Sin_TableOC2[180]={0};
//u8 Sin_TableOC3[180]={0};
u16 Sin_Table[3][205];//最小频率50Hz时，1/4周期有200个点


//状态机相关
u8 SPWMState1=1;	//SPWM周期状态机
u8 SPWMState2=1;
u8 SPWMState3=1;

u8 PointCount1;//实际的非重复计数点
u8 PointCount2;//实际的非重复计数点
u8 PointCount3;//实际的非重复计数点

//输出极性控制开关
u8 HighOutput1=1;
u8 LowOutput1=1;
u8 HighOutput2=1;
u8 LowOutput2=1;
u8 HighOutput3=1;
u8 LowOutput3=1;
//输出使能控制相关
u8 OC1OutputEnable=0;//默认不输出
u8 OC2OutputEnable=0;
u8 OC3OutputEnable=0;

void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6|RCC_APB1Periph_TIM7|RCC_APB1Periph_TIM3,ENABLE);//要用到三个定时器
		
	TIM_TimeBaseStruct.TIM_Period=1800-1;             // 自动重装载寄存器的值	72M/1800=40khz
	TIM_TimeBaseStruct.TIM_Prescaler=0;            // 时钟预分频数
//	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比    
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStruct);	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
	
	//TIM6中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	//TIM7中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	//TIM3中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
//	
	TIM_ClearFlag(TIM6,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ClearFlag(TIM7,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ClearFlag(TIM3,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //使能中断
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //使能中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能中断
	TIM_Cmd(TIM6,ENABLE); //开启时钟
	TIM_Cmd(TIM7,ENABLE); //开启时钟
	TIM_Cmd(TIM3,ENABLE); //开启时钟
}
//定时器基本初始化完毕



//通道3，板子上是通道1
void TIM3_IRQHandler(void)
{
	static u16 count1=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))	 //判断是否定时中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
		if(!OC1OutputEnable)
		{
			OutputOC3NState_Disable;OutputOC3State_Disable;
			return;
		}
		count1++;
		
		
		//前1/4周期---------------------------------------------------
		if(SPWMState1 == 1)
		{
			TIM8->CCR3 = Sin_Table[0][count1];//写占空比
			if(HighOutput1)//输出正半周期，关闭互补输出
			{
				OutputOC3NState_Disable;//关闭互补输出
				OutputOC3State_Enable;
				HighOutput1=0;//只关一次
				LowOutput1=1;	
			}
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=2;//清计数变量，进入下一个状态	
			}
		}
		else if(SPWMState1 ==2)//前2/4周期
		{
			TIM8->CCR3 = Sin_Table[0][PointCount1-count1];//写占空比
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=3;//清计数变量，进入下一个状态	
			}
		}

		//负半周期---------------------------------------------------
		else if(SPWMState1 ==3)
		{
			TIM8->CCR3 = Sin_Table[0][count1];//写占空比
			if(LowOutput1)//输出负半周期，关闭原通道输出
			{
				OutputOC3State_Disable;//关闭原通道输出
				OutputOC3NState_Enable;
				LowOutput1=0;
				HighOutput1=1;//只关一次
			}
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=4;//清计数变量，进入下一个状态
			}
		}
		else if(SPWMState1 ==4)
		{
			TIM8->CCR3 = Sin_Table[0][PointCount1-count1];//写占空比
			if(count1 > PointCount1)
			{
				count1=0;SPWMState1=1;//清计数变量，进入下一个状态	
			}
		}
	}

}
////OC1动态SPWM

//定时25us更新PWM占空比和方向
//暂时只输出通道2，板子上是通道2
void TIM6_IRQHandler(void)
{
	static u16 count2=0;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))	 //判断是否定时中断
	{
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
		if(!OC2OutputEnable)
		{
			OutputOC2NState_Disable;OutputOC2State_Disable;
			return;
		}
		count2++;
		
		
		//前1/4周期---------------------------------------------------
		if(SPWMState2 == 1)
		{
			TIM8->CCR2 = Sin_Table[1][count2];//写占空比
			if(HighOutput2)//输出正半周期，关闭互补输出
			{
				OutputOC2NState_Disable;//关闭互补输出
				OutputOC2State_Enable;
				HighOutput2=0;//只关一次
				LowOutput2=1;	
			}
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=2;//清计数变量，进入下一个状态	
			}
		}
		else if(SPWMState2 ==2)//前2/4周期
		{
			TIM8->CCR2 = Sin_Table[1][PointCount2-count2];//写占空比
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=3;//清计数变量，进入下一个状态	
			}
		}

		//负半周期---------------------------------------------------
		else if(SPWMState2 ==3)
		{
			TIM8->CCR2 = Sin_Table[1][count2];//写占空比
			if(LowOutput2)//输出负半周期，关闭原通道输出
			{
				OutputOC2State_Disable;//关闭原通道输出
				OutputOC2NState_Enable;
				LowOutput2=0;
				HighOutput2=1;//只关一次
			}
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=4;//清计数变量，进入下一个状态
			}
		}
		else if(SPWMState2 ==4)
		{
			TIM8->CCR2 = Sin_Table[1][PointCount2-count2];//写占空比
			if(count2 > PointCount2)
			{
				count2=0;SPWMState2=1;//清计数变量，进入下一个状态	
			}
		}
	}

}
//OC2动态SPWM



////通道1，板子上是通道3
void TIM7_IRQHandler(void)
{
	static u16 count3=0;
	if(TIM_GetITStatus(TIM7,TIM_IT_Update))	 //判断是否定时中断
	{
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
		if(!OC3OutputEnable)
		{
			OutputOC1NState_Disable;OutputOC1State_Disable;
			return;
		}
		count3++;
		
		
		//前1/4周期---------------------------------------------------
		if(SPWMState3 == 1)
		{
				
			TIM8->CCR1 = Sin_Table[2][count3];//写占空比
			if(HighOutput3)//输出正半周期，关闭互补输出
			{
				OutputOC1NState_Disable;//关闭互补输出
				OutputOC1State_Enable;
				HighOutput3=0;//只关一次
				LowOutput3=1;	
			}
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=2;//清计数变量，进入下一个状态
			
			}
		}
		else if(SPWMState3 ==2)//前2/4周期
		{
			TIM8->CCR1 = Sin_Table[2][PointCount3-count3];//写占空比
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=3;//清计数变量，进入下一个状态	
			}
		}

		//负半周期---------------------------------------------------
		else if(SPWMState3 ==3)
		{
			TIM8->CCR1 = Sin_Table[2][count3];//写占空比
			if(LowOutput3)//输出负半周期，关闭原通道输出
			{
				OutputOC1State_Disable;//关闭原通道输出
				OutputOC1NState_Enable;
				LowOutput3=0;
				HighOutput3=1;//只关一次
			}
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=4;//清计数变量，进入下一个状态
			}
		}
		else if(SPWMState3 ==4)
		{
			TIM8->CCR1 = Sin_Table[2][PointCount3-count3];//写占空比
			if(count3 > PointCount3)
			{
				count3=0;SPWMState3=1;//清计数变量，进入下一个状态	
			}
		}
	}

}
////OC3动态SPWM



//OCx：	1、2、3
//Freq：100hz----1000Hz	【基频200kHz】
//Ampl：0-----99
u16 TableOffset = 2;//占空比偏移矫正
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl)
{
	u16 SamplingCount;
	u8 i;
	float MaxAmpl;
	//先进行参数有效性判断
	if((Freq<100)||(Freq>1000))return;
	if(Ampl>99)return;
	
	SamplingCount = 200000/5/Freq/4;//计算不重复的采样点，即1/4周期的采样点
	if(SamplingCount>200)//采样点不要超过脉冲数上限，基频200k脉冲数360
		SamplingCount=200;
	
	MaxAmpl = (float)(Ampl)/100*360;//根据占空比计算最大脉冲数
	
	switch (OCx)
	{
		case 1:
			PointCount1 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//计算每个采样点的占空比
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
		case 2:
			PointCount2 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//计算每个采样点的占空比
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
		case 3:
			PointCount3 = SamplingCount;
			for(i=0;i<SamplingCount;i++)//计算每个采样点的占空比
				Sin_Table[OCx-1][i]=TableOffset + (u16)(MaxAmpl*sin(i*PI/2/SamplingCount));
			break;
	}
	
	//下面代码最后要注释掉
	printf("输出当前的sin表，OC %d\r\n",OCx);
	for(i=0;i<SamplingCount;i++)
	{
		printf("%d\r\n",Sin_Table[OCx-1][i]);
	}
	printf("count = %d\r\n",SamplingCount);
}
//更新sin占空比表OK




