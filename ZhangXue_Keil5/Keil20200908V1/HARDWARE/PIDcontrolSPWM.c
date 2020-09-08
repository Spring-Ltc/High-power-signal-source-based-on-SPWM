

#include "PIDcontrolSPWM.h"

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
u8 Sin_Table[3][180];


//状态机相关
u8 SPWMState=1;	//SPWM周期状态机
u8 PointCount;//实际的非重复计数点

//输出极性控制开关
u8 HighOutput=1;
u8 LowOutput=1;

//输出使能控制相关
u8 OC1OutputEnable=0;//默认不输出
u8 OC2OutputEnable=0;
u8 OC3OutputEnable=0;

void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	NVIC_InitTypeDef					NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=1200-1;             // 自动重装载寄存器的值	72M/1200=60khz
	TIM_TimeBaseStruct.TIM_Prescaler=0;            // 时钟预分频数
//	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比    
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ClearFlag(TIM6,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //使能中断
	TIM_Cmd(TIM6,ENABLE); //开启时钟
}








//定时16.666us更新PWM占空比和方向
void TIM6_IRQHandler(void)
{
	static u8 count=0;
	count++;
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))	 //判断是否定时中断
	{
		//前1/4周期---------------------------------------------------
		if(SPWMState == 1)
		{
//			printf("%d\r\n",Sin_Table[0][count]);
			TIM_SetCompare1(TIM8,Sin_Table[0][count]);
//			TIM_SetCompare2(TIM8,Sin_Table[1][count]);
//			TIM_SetCompare3(TIM8,Sin_Table[2][count]);
			if(HighOutput)//关闭TIM3输出
			{
//				printf("State = 1\r\n");
				TIM_SetCompare2(TIM3,0);
//				TIM_SetCompare3(TIM3,0);
//				TIM_SetCompare4(TIM3,0);
				HighOutput=0;//只关一次
			}
			if(count > PointCount)
			{
				count=0;SPWMState=2;//清计数变量，进入下一个状态
			}
		}
		//前2/4周期---------------------------------------------------
		else if(SPWMState == 2)
		{
//			printf("%d\r\n",Sin_Table[0][PointCount-count]);
			TIM_SetCompare1(TIM8,Sin_Table[0][PointCount-count]);
//			TIM_SetCompare2(TIM8,Sin_Table[1][PointCount-count]);
//			TIM_SetCompare3(TIM8,Sin_Table[2][PointCount-count]);
			if(count > PointCount)
			{
				count=0;SPWMState=3;//清计数变量，进入下一个状态
				LowOutput=1;//下一状态关闭正向输出只执行一次调用
			}
		}
		//前3/4周期---------------------------------------------------
		else if(SPWMState == 3)//前3/4周期
		{
//			printf("%d\r\n",Sin_Table[0][count]);
			TIM_SetCompare2(TIM3,Sin_Table[0][count]);
//			TIM_SetCompare3(TIM3,Sin_Table[1][count]);
//			TIM_SetCompare4(TIM3,Sin_Table[2][count]);
			if(LowOutput)
			{
//				printf("State = 3\r\n");
				TIM_SetCompare1(TIM8,0);
//				TIM_SetCompare2(TIM8,0);
//				TIM_SetCompare3(TIM8,0);
				LowOutput=0;
			}
			if(count > PointCount)
			{
				count=0;SPWMState=4;//清计数变量，进入下一个状态
			}
		}
		//前4/4周期---------------------------------------------------
		else if(SPWMState == 4)//前4/4周期
		{
//			printf("%d\r\n",Sin_Table[0][PointCount-count]);
			TIM_SetCompare2(TIM3,Sin_Table[0][PointCount-count]);
//			TIM_SetCompare3(TIM3,Sin_Table[1][PointCount-count]);
//			TIM_SetCompare4(TIM3,Sin_Table[2][PointCount-count]);
			if(count > PointCount)
			{
				count=0;SPWMState=1;//清计数变量，进入下一个状态
				HighOutput=1;//下一状态关闭反向输出只执行一次调用
			}
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);//必须清除中断标志位否则一直中断
}

//OCx：	1、2、3
//Freq：100hz----1500Hz
//Ampl：0-----99
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl)
{
	u16 SamplingCount;
	u8 i;
	float MaxAmpl;
	//先进行参数有效性判断
	if((Freq<100)||(Freq>1500))return;
	if(Ampl>99)return;
	
	//先根据频率计算采样点数
//	printf("Ampl is %d\r\n",Ampl);
//	Ampl &=0x7f;//把最高位符号位写为1，变为有符号数
//	printf("Ampl is %d\r\n",Ampl);
//	printf("Ampl is %f\r\n",(float)Ampl);
	SamplingCount = 60000/Freq/4;//计算不重复的采样点，即1/4周期的采样点
	MaxAmpl = (float)(Ampl*120)/100;//这里有BUG，幅值小于120，采样点很有可能大于120
	if(OCx == 1)
		PointCount = SamplingCount;
	
//	printf("MaxAmpl = %f\r\n",MaxAmpl);
	//计算每个采样点的占空比
	for(i=0;i<SamplingCount;i++)
	{
		Sin_Table[OCx-1][i]=(u8)(MaxAmpl*sin(i*PI/2/SamplingCount));
	}
	
//	for(i=0;i<SamplingCount;i++)
//	{
//		printf("%d\r\n",Sin_Table[OCx-1][i]);
//	}
//	printf("count = %d\r\n",SamplingCount);
}
