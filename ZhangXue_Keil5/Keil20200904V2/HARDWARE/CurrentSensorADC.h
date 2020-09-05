
#ifndef __CurrentSensorADC_H__
#define __CurrentSensorADC_H__

#include "sys.h"


void CurrentADC_Init(void);
void SupplyVoltageADC_Init(void);
u16 ReadPowerVoltage(void);
float  ReadCurrent(u8 OCelect);

#endif

