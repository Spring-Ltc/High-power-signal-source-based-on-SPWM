



/********************重庆邮电大学2020年单片机应用设计竞赛（生物学院）************************************

MCU型号：STM32F103RCT6
作者：Liao_Tingchun；邮箱：2093181896@qq.com
修改日志：
2020/07/19/00:49
	建立新工程，大致确定MCU资源分配
2020/07/19/18:03
	按照初步功能需求，新建HARDWARE下各文件
2020/08/08/22:40
	根据确定的PCB，分配MCU资源，做相关初始化(编码开关、电流检测、LED和KEY等基本)
2020/08/09/13:14
	移植IIC相关代码、TFT液晶屏相关代码，还有待校验，各模块初始化基本全部完成
2020/08/15/10:18
	多次查阅相关资料，修改调整Keil和Proteus的时钟和晶振配置，仿真仍无法顺利进行，
	可能主要原因为Proteus没有STM32的外部时钟树，放弃仿真，准备搭好程序框架，返校后硬件在调试
2020/08/15/11:14
	基本搭建好程序框架，整理完善好各外设初始化和上电历史数据自检，新建算法文件PIDcontrolSPWM.c文件
2020/08/25/22:00
	完成系统任务时间片定时相关配置，定义相关任务标准和使能标志
2020/09/03/18:22
	完成电路板焊接，进行基本测试，LED控制正常，TTL串口正常，电源正常（除MC34063升压略高为18V左右，理论16.2V）
	发现PCB问题：保险丝和共模电感封装过大，无法焊接
2020/09/03/18:55
	移植的LCD例程，通过宏定义改为软件模拟SPI才有效，改为横屏时，和自己放置的方向刚好反了,其他功能函数测试通过


*****************************************************************************************************/	

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"

#include "PwmTimer.h"
#include "Encoder.h"
#include "TaskTimer.h"
#include "CurrentSensorADC.h"
#include "LcdDisplay.h"
#include "CommonGPIO.h"
#include "I2CEquipment.h"
#include "SerialPortNetwork.h"
#include "PIDcontrolSPWM.h"
#include "GUI.h"


unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};

int main(void)
{
	u8 i;

	//1、system basic config
	NVIC_Configuration();    
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();
	uart_init(9600);//串口1，USB连接
		
	//2、user equipment config
	PWM_Time8Init();//SPWM定时器初始化
	EC11Encoder_Init();//EC11编码开关定时器初始化
	TaskTime_Init();//系统任务时间片分配
	LcdDisplay_Init();//液晶显示初始化
	CurrentADC_Init();//负载电流检测ADC初始化
	SupplyVoltageADC_Init();//输入供电电压检测ADC初始化
	KEY_Init();LED_Init();BUZZ_Init();//基本按键、LED、蜂鸣器初始化
	StorageI2C_Init();//EEPROM存储IIC初始化
	TemperatureI2C_Init();//LM75AD温度传感器IIC初始化
	MyUsart2_Init(9600);//无线网络通信串口初始化（可选）
	SPWM_TimeInit();
	//3、historical data self-check即读取EEPROM用户配置的历史数据
	
	
	
	Lcd_Clear(BLUE);
	//BUZZ_ON;delay_ms(300);BUZZ_OFF;
	
 LED_Select_OC1	= 0;	delay_ms(300);LED_Select_OC1	= 1;
 LED_Select_OC2		= 0;	delay_ms(300);	LED_Select_OC2		= 1;
 LED_Select_OC3			= 0;	delay_ms(300); LED_Select_OC3			= 1;
 

 
 LED_EN_OC1		= 0;	delay_ms(300);			LED_EN_OC1		= 1;
 LED_EN_OC2		= 0;	delay_ms(300);		 LED_EN_OC2		= 1;	
 LED_EN_OC3			= 0;	delay_ms(300);		LED_EN_OC3			= 1;



	printf("COM Text\r\n\r\n");
	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(16,70,RED,GRAY0, "全程技术支持");
	
	Lcd_Clear(GRAY0);
	Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
	delay_ms(1000);
	Lcd_Clear(GRAY0);

	for(i=0;i<10;i++)
	{
	Gui_DrawFont_Num32((i%3)*40,32*(i/3)+30,RED,GRAY0,Num[i+1]);
	delay_ms(100);
	}
	
	
  while (1)
  {
		LED_System =!LED_System;

		delay_ms(500);
		
		
		if(Flag_TaskPower)//读取电源电压
		{
			Flag_TaskPower = 0;
			
		}
		
		if(Flag_TaskTemp)//检测PCB温度
		{
			Flag_TaskTemp = 0;
			
		}
  }
}




