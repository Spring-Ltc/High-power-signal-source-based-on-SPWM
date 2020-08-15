

#ifndef __CommonGPIO_H__
#define __CommonGPIO_H__


#include "sys.h"



void KEY_Init(void);
void LED_Init(void);
void BUZZ_Init(void);




//LED指示灯宏定义
#define LED_System 				PBout(8)
#define LED_Select_OC1		PCout(0)
#define LED_Select_OC2		PCout(1)
#define LED_Select_OC3		PCout(2)
#define LED_EN_OC1				PAout(8)
#define LED_EN_OC2				PCout(9)
#define LED_EN_OC3				PCout(4)


//按键宏定义
#define KEY_EN_OC1				PCin(10)
#define KEY_EN_OC2				PCin(11)
#define KEY_EN_OC3				PCin(12)
#define KEY_OC_Switch			PCin(13)


//蜂鸣器宏定义
#define BUZZ 							PDout(2)
#define BUZZ_ON						BUZZ=1
#define BUZZ_OFF					BUZZ=0

#endif






