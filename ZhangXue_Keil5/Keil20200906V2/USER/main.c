



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
2020/09/04/21:35
	电流传感器ACS712-20软件测试通过，但是电流需要修改，限流电阻R4、R8、R17改为0Ω或直接短接
	开路不接负载时，传感器输出电压为2.5V左右，经过限流电阻后压降比较大
2020/09/04/22:27
	新增按键中断，但是测试暂未通过
2020/09/05/14:47
	按键中断卡死问题暂未解决，浏览论坛可能是delay中断冲突；采用扫描方式证明按键电路正常
	更新了正点原子的delay和sys文件
2020/09/05/16:54
	发现并解决电路问题：共模电感封装不对，改用两个工字电感；电流传感器输出电压引脚串接电阻改为1.1k，实际输出电压约2.15V，软件上做修正；
	光耦隔离芯片第二路可能是焊接时损坏了，更换IC后硬件短接拉低正常控制MOS开关，其他两路均正常
2020/09/05/20:37
	旋转编码开关EC11测试完毕，添加任务函数验证OK，能够判断旋转方向和脉冲数量，使用定时任务扫描节省了中断资源
2020/09/05/21:35
	添加按键切换通道任务函数，并测试OK，为使LED知识颜色顺序和通道一致，软件更改了LED引脚（OC1和OC3互换select引脚）
	拨码开关的按键，尝试了一下使用按键中断，结果还是卡死，但是步进值相关函数已经添加
2020/09/06/10:56
	编码开关的外部中断卡死问题解决，根因是中断服务函数名称写错了，哎，英语没学好，ctrl+C/V来背锅
2020/09/06/12:51
	按键的外部中断卡死问题解决，EXTI初始化的时候要分开初始化，不用使用或运算
2020/09/06/17:15
	高级定时器输出互补PWM添加验证完毕，注意初始化的时候，原通道和互补通道的输出极性设置一样才是互补输出
	发现问题：基准频率设置太高（500kHz，约2us）导致系统异常断电
		解决问题：输出极性设置有误，实际输出不是互补输出
	正常输出PWM时，PCB背面地平面发热比较明显，更改了MC34063反馈电阻，现在输出电压12.3V左右偏低
	
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


//==============================全局变量声明===============================//
u16 NowcountFreq=0;//当前编码开关计数值
u16 LastcountFreq=0;
u16 NowcountAmpl=0;//上次编码开关计数值
u16 LastcountAmpl=0;
u8 SelectCount=0;
u8 NowSelect;//当前选中的通道
u16 StepFreq=5;//调节频率步进值
u16 StepAmpl=5;//调节幅值步进值



//======================任务函数声明============================//
void TaskEncodeUpdate_Freq(u16 nowcount);
void TaskEncodeUpdate_Ampl(u16 nowcount);
void TaskKeyScan(u8 key);
void TaskSwitchSelectSwitch(u8 select);


int main(void)
{
//	u8 i,KeyValue;
//	u16 PowerVoltage;//主电源电压
//	float Current;
//	u32 count;
	
	//1、system basic config
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// 设置中断优先级分组2（2位先占优先级和2位从优先级）【优先级分组只能设置一次】
	uart_init(9600);	 //串口初始化为9600
		
	//2、user equipment config
//	PWM_Time8Init();//SPWM定时器初始化																【测试通过】
	EC11Encoder_Init();//EC11编码开关定时器初始化											【测试通过】
	TaskTime_Init();//系统任务时间片分配
	LcdDisplay_Init();//液晶显示初始化																【测试半通过，功能正常，屏幕方向还有问题】
	CurrentADC_Init();//负载电流检测ADC初始化													【测试通过】
	SupplyVoltageADC_Init();//输入供电电压检测ADC初始化								【测试通过】
	KEY_Init();LED_Init();BUZZ_Init();//基本按键、LED、蜂鸣器初始化		【测试通过】
	StorageI2C_Init();//EEPROM存储IIC初始化														【测试通过】
//	TemperatureI2C_Init();//LM75AD温度传感器IIC初始化
//	MyUsart2_Init(9600);//无线网络通信串口初始化（可选）
//	SPWM_TimeInit();
	//3、historical data self-check即读取EEPROM用户配置的历史数据

	LED_System=1;
	Lcd_Clear(RED);
	Gui_DrawFont_GBK16(32,70,BLUE,RED, "全程技术支持");
	Flag_TaskReadEncode=1;
  while (1)
  {
		//按键部分
//		KeyValue=KEY_Scan(1);		//得到键值
//		if( KeyValue)//有按键按下
//			TaskKeyScan(KeyValue);

		LED_System =!LED_System;
		delay_ms(200);

//		Current = ReadCurrent(1);
//		printf("Current is: %f  A\r\n\r\n",Current);
		
		
		
		if(Flag_TaskPower)//读取电源电压
		{
			Flag_TaskPower = 0;
//		PowerVoltage = ReadPowerVoltage();
//		printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
		}
		
		if(Flag_TaskTemp)//检测PCB温度
		{
			Flag_TaskTemp = 0;
			
		}
		if(Flag_TaskReadEncode)//刷新编码开关的值
		{
			//Flag_TaskReadEncode=0;
			NowcountFreq = TIM_GetCounter(TIM4);
			if(NowcountFreq != LastcountFreq)
				TaskEncodeUpdate_Freq(NowcountFreq);
			NowcountAmpl = TIM_GetCounter(TIM2);
			if(NowcountAmpl != LastcountAmpl)
				TaskEncodeUpdate_Ampl(NowcountAmpl);
		}
  }
}



//按键任务函数，定时扫描键值方式
void TaskKeyScan(u8 key)
{
	switch(key)
	{				 
		case KEY_EN_OC1_PRES:
			LED_EN_OC1=!LED_EN_OC1;
			break;
		case KEY_EN_OC2_PRES:
			LED_EN_OC2=!LED_EN_OC2;
			break;
		case KEY_EN_OC3_PRES:				
			LED_EN_OC3=!LED_EN_OC3;
			break;
		case KEY_OC_Switch_PRES:	
		{
			SelectCount++;
			NowSelect = SelectCount%3+1;
			TaskSwitchSelectSwitch(NowSelect);
//			printf("Switch\r\n");
			break;
		}
	} 
}

//通道切换任务函数
void TaskSwitchSelectSwitch(u8 select)
{
	if(select == 1)
	{
		LED_Select_OC1 = 0;LED_Select_OC2 = 1;LED_Select_OC3 = 1;	
	}
	else if(select == 2)
	{
		LED_Select_OC1 = 1;LED_Select_OC2 = 0;LED_Select_OC3 = 1;	
	}
	else if(select == 3)
	{
		LED_Select_OC1 = 1;LED_Select_OC2 = 1;LED_Select_OC3 = 0;	
	}
	
}



//频率改变任务函数
void TaskEncodeUpdate_Freq(u16 nowcount)
{
	u16 ValueChanged;
	if(nowcount < LastcountFreq)//频率减小	
	{
		ValueChanged = (LastcountFreq - nowcount)/2;
		LastcountFreq = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		//【设置PWM计数器相关】
		printf("Freq disces %d\r\n",ValueChanged*StepFreq);
	}
	else//频率增大
	{
		ValueChanged = (nowcount - LastcountFreq)/2;
		LastcountFreq = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		//【设置PWM计数器相关】
		printf("Freq incres %d\r\n",ValueChanged*StepFreq);
	}
	
}
//幅值改变任务函数
void TaskEncodeUpdate_Ampl(u16 nowcount)
{
	u16 ValueChanged;
	if(nowcount < LastcountAmpl)//幅值减小
	{
		ValueChanged = (LastcountAmpl - nowcount)/2;
		LastcountAmpl = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		//【设置PWM计数器相关】
		printf("Ampl dicrese %d\r\n",ValueChanged*StepAmpl);
	}
	else//幅值增大
	{
		ValueChanged = (nowcount - LastcountAmpl)/2;
		LastcountAmpl = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		//【设置PWM计数器相关】
		printf("Ampl increse %d\r\n",ValueChanged*StepAmpl);
	}
}




//============================该部分为按键中断服务函数=================================//

//中断线10--15共用一个中断服务函数
//4个按键的外部中断服务函数
void EXTI15_10_IRQHandler(void)
{
	delay_ms(2);//按键消抖
	
	//按键1
	if(KEY_EN_OC1 == 0)//按键并按下
	{
		LED_EN_OC1 = !LED_EN_OC1;
		printf("KEY1\r\n");
		while(!KEY_EN_OC1);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line10);	//清除中断标志位
		return;
	}
		
	//按键2
	else if(KEY_EN_OC2 == 0)//按键并按下
	{
		LED_EN_OC2 = !LED_EN_OC2;
		printf("KEY2\r\n");
		while(!KEY_EN_OC2);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line11);	//清除中断标志位
		return;
	}
	
	//按键3
	else if(KEY_EN_OC3 == 0)//按键并按下
	{
		LED_EN_OC3 = !LED_EN_OC3;
		printf("KEY3\r\n");
		while(!KEY_EN_OC3);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line12);	//清除中断标志位
		return;
	}

	//按键4
	else if(KEY_OC_Switch == 0)//按键并按下
	{
		SelectCount++;
		NowSelect = SelectCount%3+1;
		TaskSwitchSelectSwitch(NowSelect);
		printf("KEY4\r\n");
		while(!KEY_OC_Switch);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line13);	//清除中断标志位
		return;
	}
	printf("KEY BUTTON\r\n");
	EXTI_ClearITPendingBit(EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13);	//清除中断标志位
}


//编码开关按键中断服务函数改变频率调节的步进值
void EXTI9_5_IRQHandler(void)
{
	static u8 stepfreqcount=0;
	delay_ms(2);//按键消抖
	if(KEY_Encod1 == 0)
	{
		stepfreqcount++;
		switch (stepfreqcount%3+1)
		{
			case 1:
				StepFreq = 10;break;
			case 2:
				StepFreq = 100;break;
			case 3:
				StepFreq = 1000;break;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line5);	//清除中断标志位
}

//编码开关按键中断服务函数改变幅值调节的步进值
void EXTI3_IRQHandler(void)
{
	static u8 stepamplcount=0;
//	printf("KEY Ampl Pres\r\n");
	delay_ms(2);//按键消抖
	if(KEY_Encod2 == 0)
	{
		stepamplcount++;
		switch (stepamplcount%3+1)
		{
			case 1:
				StepAmpl = 10;break;
			case 2:
				StepAmpl = 100;break;
			case 3:
				StepAmpl = 1000;break;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);	//清除中断标志位
}





