#ifndef __PwmTimer_H__
#define __PwmTimer_H__

#include "sys.h"


#define Count_Clock 72000000	//定时器时钟频率72MHz





void PWMOC1_Time6Init(void);
void PWMOC2_Time8Init(void);
void PWMOC3_Time3Init(void);


void SetPwmVal(u32 Freq,u16 Duty,u8 OCx);

#endif


