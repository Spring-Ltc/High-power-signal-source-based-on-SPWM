

#ifndef __PIDcontrolSPWM_H__
#define __PIDcontrolSPWM_H__


#include "sys.h"
#include "math.h"
#define PI 3.1415962		//计算三角函数要用到

extern u8 OC1OutputEnable;
extern u8 OC2OutputEnable;
extern u8 OC3OutputEnable;


//各通道和互补通道的使能位
#define OutputOC1State_Enable			TIM8->CCER |= 0x0001
#define OutputOC1State_Disable		TIM8->CCER &= 0xfffe
#define OutputOC1NState_Enable		TIM8->CCER |= 0x0004
#define OutputOC1NState_Disable		TIM8->CCER &= 0xfffb
#define OutputOC2State_Enable			TIM8->CCER |= (0x0001 <<4)
#define OutputOC2State_Disable		TIM8->CCER &= (0xfffe <<4)
#define OutputOC2NState_Enable		TIM8->CCER |= (0x0004 <<4)
#define OutputOC2NState_Disable		TIM8->CCER &= (0xfffb <<4)
#define OutputOC3State_Enable			TIM8->CCER |= (0x0001 <<8)
#define OutputOC3State_Disable		TIM8->CCER &= (0xfffe <<8)
#define OutputOC3NState_Enable		TIM8->CCER |= (0x0004 <<8)
#define OutputOC3NState_Disable		TIM8->CCER &= (0xfffb <<8)




void SPWM_TimeInit(void);
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl);

#endif

