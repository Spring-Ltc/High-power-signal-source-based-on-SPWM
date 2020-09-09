#ifndef __TaskTimer_H__
#define __TaskTimer_H__

#include "sys.h"

extern u8 Flag_TaskTemp;
extern u8 Flag_TaskPower;
extern u8 Flag_TaskCurrent;
extern u8 Flag_TaskReadEncode;
extern u8 Flag_TaskDisplayUpdate;
extern u8 Flag_TaskDataDtorage;
extern u8 Flag_TaskOutputControl;

void TaskTime_Init(void);



#endif

