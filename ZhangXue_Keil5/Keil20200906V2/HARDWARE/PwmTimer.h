#ifndef __PwmTimer_H__
#define __PwmTimer_H__

#include "sys.h"

extern u32 PwmBasicFreq;		//µ¥Î» Hz



void PWM_Time1Init(void);
void PWM_Time4Init(void);
void PWM_Init(u32 freq);

//void PWM_Time8Init(u32 BaseFreq);
void PWM_Time8Init(void);
#endif


