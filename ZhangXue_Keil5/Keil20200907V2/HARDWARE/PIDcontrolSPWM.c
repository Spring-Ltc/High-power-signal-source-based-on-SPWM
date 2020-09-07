

#include "PIDcontrolSPWM.h"

//这里也需要一个定时器，定时到达切换占空比和时间周期，用以生成SPWM波
//暂时分配TIM3


void SPWM_TimeInit(void)
{
	
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
		
	TIM_TimeBaseStruct.TIM_Period=3600-1;             // 自动重装载寄存器的值	36M/1800=20khz
	TIM_TimeBaseStruct.TIM_Prescaler=1;            // 时钟预分频数
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;    // 采样分频
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;//重复寄存器，用于自动更新pwm占空比    
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update); //清除定时器中断标志
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能中断
	TIM_Cmd(TIM3,ENABLE); //开启时钟
	
	
}



void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))	 //判断是否定时中断
			{

//				TIM_SetCompare1(TIM8,Sin_Table[a]*8);	

			}
	   TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);//必须清除中断标志位否则一直中断


}

