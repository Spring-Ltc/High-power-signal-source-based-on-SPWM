

#ifndef __PIDcontrolSPWM_H__
#define __PIDcontrolSPWM_H__


#include "sys.h"
#include "math.h"
#define PI 3.1415962		//�������Ǻ���Ҫ�õ�

extern u8 OC1OutputEnable;
extern u8 OC2OutputEnable;
extern u8 OC3OutputEnable;

void SPWM_TimeInit(void);
void UpdateSinTable(u8 OCx,u16 Freq,u8 Ampl);

#endif

