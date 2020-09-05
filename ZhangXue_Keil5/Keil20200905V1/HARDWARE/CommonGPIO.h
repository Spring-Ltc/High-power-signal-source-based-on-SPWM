

#ifndef __CommonGPIO_H__
#define __CommonGPIO_H__


#include "sys.h"





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
#define KEY_EN_OC1_PRES	1		
#define KEY_EN_OC2_PRES	2		
#define KEY_EN_OC3_PRES	3		
#define KEY_OC_Switch_PRES	4		


//蜂鸣器宏定义
#define BUZZ 							PDout(2)
#define BUZZ_ON						BUZZ=1
#define BUZZ_OFF					BUZZ=0



void KEY_Init(void);
void LED_Init(void);
void BUZZ_Init(void);
u8 KEY_Scan(u8 mode);  	//按键扫描函数	






#endif






