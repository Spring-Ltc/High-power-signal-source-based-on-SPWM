#ifndef __PwmTimer_H__
#define __PwmTimer_H__

#include "sys.h"


#define Count_Clock 72000000	//定时器时钟频率72MHz




//void PWM_Time1Init(void);
//void PWM_Time4Init(void);
//void PWM_Init(u32 freq);

//void PWM_Time8Init(u32 BaseFreq);
void PWM_Time8Init(void);
void PWM_BaseTimeInit(void);
void SetPwmVal(u32 Freq,u16 Duty,u8 OCx);

#endif


