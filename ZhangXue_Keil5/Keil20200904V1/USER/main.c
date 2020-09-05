



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
2020/09/04/21:00
	AT24C256存储IIC测试通过；电源电压ADC采集测试OK，且已做分压参数修正；

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


int main(void)
{
	u8 i;
	u16 PowerVoltage;//主电源电压
	
	
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
	StorageI2C_Init();//EEPROM存储IIC初始化【测试通过】
//	TemperatureI2C_Init();//LM75AD温度传感器IIC初始化
//	MyUsart2_Init(9600);//无线网络通信串口初始化（可选）
//	SPWM_TimeInit();
	//3、historical data self-check即读取EEPROM用户配置的历史数据
	
	


	printf("COM Text\r\n\r\n");
	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(32,70,BLUE,RED, "全程技术支持");
	

//	StorageWriteOneByte(0x1200,0x66);//写入0x55

	printf("read the data is %x \r\n",StorageReadOneByte(0x1200));
	
	
  while (1)
  {
		LED_System =!LED_System;
		PowerVoltage = ReadPowerVoltage();
		printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
		delay_ms(1000);
		
		
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




