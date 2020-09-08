



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
2020/09/06/21:28
	液晶屏幕方向已经更改正常，更改Lcd_SetXY函数中的形参为像素边界减去原来的值
2020/09/07/08:28
	发现问题，系统任务定时器TIM定时中断时间不对，比理论值偏大了很多
	09:06：发现问题，三通道同时开启，系统周期性断电，初步推断是没有设置死区宽度所致
2020/09/07/:09:40
	系统任务定时器定时时间偏大问题解决，原因是没有设置TIM_RepetitionCounter参数，碰巧其缺省值不为0
	设置了死区宽度后，断电问题解决，但发热现象仍然比较严重，主要是驱动芯片IR2104发热
	宽度参数设置现象：27/正常；18/电压拉低，屏幕不亮，但没有断电；9/周期性断电
2020/09/07/10:29
	所有基本功能测试通过，准备写SPWM方法生成正弦波，突然发现互补PWM方案多个周期没办法积分，一度陷入崩溃.............
2020/09/07/14:30
	TIM8的互补通道，刚好可以用TIM3的通道替换，采用两个定时器6个通道完成SPWM输出，ST的IC引脚复用设计救我一命
	设置定时器TIM8和TIM3的基准频率为600kHz,10个基准信号周期为一个SPWM点，每个SPWM周期设置至少2*20个采样点
	故输出正弦波MaxFreq=1.5kHz，设置定时器TIM6定时约16.6us(60kHz)更新一次占空比
2020/09/07/16:33
	写SPWM程序时发现新问题，三个通道共用一个TIM6定时器改占空比的话，一个状态机不能独立调节
2020/09/07/19:32
	SPWM相关函数完成，单通道测试好像没有输出，但是打印采样点的占空比显示正确
2020/09/07/21:22
	先把控制显示部分写了再来看定时器输出，液晶更新显示由原来的定时触发改为按键和编码开关操作触发
	液晶屏显示布局基本确定，但根据实际参数还要做适当的位置调整；移植例程没有数字显示，需要自己做字符转换
2020/09/07/22:14
	数字显示部分完成，但是测试暂未通过
2020/09/08/11:15
	数字显示部分完成，包括数字转字符串函数，测试通过，参数项待调整
2020/09/08/15:46
	简单的界面布局基本完成，参数项调节更新显示完成（包括电流检测、电源电压检测，步进值、幅频参数、开关状态、通道选中）
	理论上还差温度检测、数据存储、输出控制三部分了
2020/09/08/16:35
	数据存储和上电自检部分完成，暂时只做了幅频参数存储，以及历史数据标志位
2020/09/08/18:52
	发现新问题，基准频率为600kHz时（计数120），占空比调节范围受限（50---78），
	示波器测定，高电平持续时间min=640ns；低电平持续时间min=720ns
2020/09/08/20:03
	尝试1：修改基准频率为300kHz，5个周期为一点，至少4*10个点为一个SPWM波形，输出频率上限1.5kHz
		测得结果：MaxDuty=82%,MinDuty=21%,
	尝试2：修改基准频率为200kHz，5个周期为一点，至少4*10个点为一个SPWM波形，输出频率上限1kHz
		测得结果：MaxDuty=88%,MinDuty=14%,
	发现降低基准频率过后，驱动IC发热现象不那么明显了
2020/09/08/20:31
	SPWM波形已经模糊出现，但是还有两个问题：
		问题一：TIM8和TIM1有时间差，直接导致的是两个输出通道波形有相位差，叠加后就失真了
		问题二：SPWM方法的定时器TIM6时间可能不准确，没有按期望5点一采样
				测试输出40kH没毛病，定时应该是OK的,按基准频率添加一个电容看一下效果
2020/09/08/21:51
	后面注意改一下通道对应关系，当前软件的通道和电路板的通道是反着的，（通道1和通道3需要调换一下）
	PCB的通道3背面焊接了一个1206封装的100nF电容试一下，待测试
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
u8 NowSelect=1;//当前选中的通道
u16 StepFreq=1;//调节频率步进值
u16 StepAmpl=1;//调节幅值步进值

//三个通道的当前输出
u8 NowAmplOut[3]={99,99,99};
u16 NowFreqOut[3]={500,500,500};
u16 NowCurrOut[3]={200,200,200};


//======================任务函数声明============================//
void TaskEncodeUpdate_Freq(u16 nowcount);
void TaskEncodeUpdate_Ampl(u16 nowcount);
void TaskKeyScan(u8 key);
void TaskSwitchSelectSwitch(u8 select);
void TaskDisplayUpdate(void);
void TaskDataDtorage(void);
void HistoryDataCheck(void);


int main(void)
{
//	u8 i,KeyValue;
	u16 PowerVoltage;//主电源电压
	
	//1、system basic config
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);// 设置中断优先级分组2（2位先占优先级和2位从优先级）【优先级分组只能设置一次】
	uart_init(9600);	 //串口初始化为9600
		
	//2、user equipment config
	PWM_BaseTimeInit();
//	PWM_Time8Init();//互补输出PWM定时器初始化																【测试通过，但发现互补PWM不能产生SPWM】
	EC11Encoder_Init();//EC11编码开关定时器初始化																				【测试通过】
	TaskTime_Init();//系统任务时间片分配																	 							【测试通过】
	LcdDisplay_Init();//液晶显示初始化																		  						【测试通过】
	CurrentADC_Init();//负载电流检测ADC初始化																						【测试通过】
	SupplyVoltageADC_Init();//输入供电电压检测ADC初始化																	【测试通过】
	KEY_Init();LED_Init();BUZZ_Init();FANControl_Init();//基本按键、LED、蜂鸣器初始化		【测试通过】
	StorageI2C_Init();//EEPROM存储IIC初始化																							【测试通过】
//	TemperatureI2C_Init();//LM75AD温度传感器IIC初始化
//	MyUsart2_Init(9600);//无线网络通信串口初始化（可选）
	SPWM_TimeInit();				//【测试未完成】
	
	//3、historical data self-check即读取EEPROM用户配置的历史数据
	HistoryDataCheck();//																																【测试通过】
	TaskDisplayUpdate();//先显示默认参数
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);
	
	
  while (1)
  {
		//按键部分
//		KeyValue=KEY_Scan(1);		//得到键值
//		if( KeyValue)//有按键按下
//			TaskKeyScan(KeyValue);

		
		
//		LED_System =!LED_System;
		delay_ms(500);

		if(Flag_TaskCurrent)//电流检测任务
		{
			Flag_TaskCurrent=0;
			NowCurrOut[0] = ReadCurrent(1);
			NowCurrOut[1] = ReadCurrent(2);
			NowCurrOut[2] = ReadCurrent(3);
			Gui_DrawNum_GBK16(100,140,BLUE,GRAY0,NowCurrOut[0]);
			Gui_DrawNum_GBK16(160,140,BLUE,GRAY0,NowCurrOut[1]);
			Gui_DrawNum_GBK16(220,140,BLUE,GRAY0,NowCurrOut[2]);//显示电流
		}
		
		
		
		
		if(Flag_TaskPower)//读取电源电压，若电压被拉低，关闭输出保护电路
		{
			Flag_TaskPower = 0;
			PowerVoltage = ReadPowerVoltage();
			Gui_DrawNum_GBK16(210,30,BLUE,YELLOW,PowerVoltage);//液晶更新显示电压值
//			printf("Power Voltage is: %d  mV\r\n\r\n",PowerVoltage);
			if(PowerVoltage <7)
			{
				printf("\t*****************\t Error :Power Fault!\t*************\r\n");
			}
		}
		
		if(Flag_TaskTemp)//检测PCB温度
		{
			Flag_TaskTemp = 0;
			if(1)
				FAN_ON;
			else
				FAN_OFF;
		}
		if(Flag_TaskDataDtorage)//数据存储任务
		{
			Flag_TaskDataDtorage=0;
			TaskDataDtorage();
		}
		if(Flag_TaskReadEncode)//刷新编码开关的值,10ms/次
		{
			Flag_TaskReadEncode=0;
			NowcountFreq = TIM_GetCounter(TIM4);
			if(NowcountFreq != LastcountFreq)
				TaskEncodeUpdate_Freq(NowcountFreq);
			NowcountAmpl = TIM_GetCounter(TIM2);
			if(NowcountAmpl != LastcountAmpl)
				TaskEncodeUpdate_Ampl(NowcountAmpl);
		}
		
		if(Flag_TaskDisplayUpdate)//更新液晶显示
		{
			Flag_TaskDisplayUpdate = 0;
			TaskDisplayUpdate();
//			printf("%d\r\n",(u16)NowAmplOut[1]);
//			TIM_SetCompare1(TIM8,(u16)NowAmplOut[1]*360/100);
//			TIM_SetCompare2(TIM3,NowAmplOut[2]*360/100);	//通道一，用23的幅值做占空比测试
		}
  }
}



//按键任务函数，定时扫描键值方式//【未使用】
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

//通道切换任务函数【仅改变LED指示】
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
		NowFreqOut[NowSelect-1] -= ValueChanged*StepFreq;
		if(NowFreqOut[NowSelect-1] <100)NowFreqOut[NowSelect-1]=100;//往下限幅
		else if(NowFreqOut[NowSelect-1] >1500)NowFreqOut[NowSelect-1]=100;//数据往下溢出，所以仍然往下限幅
	}
	else//频率增大
	{
		ValueChanged = (nowcount - LastcountFreq)/2;
		LastcountFreq = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		NowFreqOut[NowSelect-1] += ValueChanged*StepFreq;
		if(NowFreqOut[NowSelect-1] >1500)NowFreqOut[NowSelect-1]=1500;//往上限幅
	}
	Flag_TaskDisplayUpdate=1;//数据发生改变，允许更新显示
	Flag_TaskDataDtorage=1;//参数被修改，允许记录
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//【设置PWM计数器相关】
//	printf("Freq is %d ;Ampl is %d\r\n",NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//输出看一下呢
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
		
		NowAmplOut[NowSelect-1] -= ValueChanged*StepAmpl;
		if(NowAmplOut[NowSelect-1] <11)NowAmplOut[NowSelect-1]=11;//往下限幅
		else if(NowAmplOut[NowSelect-1] >99)NowAmplOut[NowSelect-1]=11;//数据往下溢出
	}
	else//幅值增大
	{
		ValueChanged = (nowcount - LastcountAmpl)/2;
		LastcountAmpl = nowcount;//更新历史变量
		if(ValueChanged>EncodeMaxChange) return;//一定时间内改变量太大，判断为计数器溢出，忽略本次操作
		NowAmplOut[NowSelect-1] += ValueChanged*StepAmpl;	
		if(NowAmplOut[NowSelect-1] >99)NowAmplOut[NowSelect-1]=99;//往上限幅
	}
	Flag_TaskDisplayUpdate=1;//数据发生改变，允许更新显示
	Flag_TaskDataDtorage=1;//参数被修改，允许记录
	UpdateSinTable(NowSelect,NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//【设置PWM计数器相关】
//	printf("Freq is %d ;Ampl is %d\r\n",NowFreqOut[NowSelect-1],NowAmplOut[NowSelect-1]);//输出看一下呢
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
//		printf("KEY1\r\n");
		if(LED_EN_OC1 == 1)//灯灭关闭输出
			OC1OutputEnable = 0;
		else //灯亮开启输出
			OC1OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC1);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line10);	//清除中断标志位
		return;
	}
		
	//按键2
	else if(KEY_EN_OC2 == 0)//按键并按下
	{
		LED_EN_OC2 = !LED_EN_OC2;
//		printf("KEY2\r\n");
		if(LED_EN_OC2 == 1)//灯灭关闭输出
			OC2OutputEnable = 0;
		else //灯亮开启输出
			OC2OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC2);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line11);	//清除中断标志位
		return;
	}
	
	//按键3
	else if(KEY_EN_OC3 == 0)//按键并按下
	{
		LED_EN_OC3 = !LED_EN_OC3;
//		printf("KEY3\r\n");
		if(LED_EN_OC3 == 1)//灯灭关闭输出
			OC3OutputEnable = 0;
		else //灯亮开启输出
			OC3OutputEnable = 1;
		Flag_TaskDisplayUpdate=1;
		while(!KEY_EN_OC3);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line12);	//清除中断标志位
		return;
	}

	//按键4
	else if(KEY_OC_Switch == 0)//按键并按下
	{
		SelectCount++;
		NowSelect = SelectCount%3+1;
		TaskSwitchSelectSwitch(NowSelect);//改变LED的指示
		Flag_TaskDisplayUpdate=1;//改变液晶屏的显示
//		printf("KEY4\r\n");
		while(!KEY_OC_Switch);//松手检测
		EXTI_ClearITPendingBit(EXTI_Line13);	//清除中断标志位
		return;
	}
//	printf("KEY BUTTON\r\n");
	EXTI_ClearITPendingBit(EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13);	//清除中断标志位
}
//

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
				StepFreq = 1;break;
			case 2:
				StepFreq = 10;break;
			case 3:
				StepFreq = 50;break;
		}
	}
	Flag_TaskDisplayUpdate=1;//数据发生改变，允许更新显示
	EXTI_ClearITPendingBit(EXTI_Line5);	//清除中断标志位
}
//

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
				StepAmpl = 1;break;
			case 2:
				StepAmpl = 5;break;
			case 3:
				StepAmpl = 10;break;
		}
	}
	Flag_TaskDisplayUpdate=1;//数据发生改变，允许更新显示
	EXTI_ClearITPendingBit(EXTI_Line3);	//清除中断标志位
}
///

//液晶屏更新显示任务
void TaskDisplayUpdate(void)
{
	//通道选中背景高亮指示
	Gui_DrawFont_GBK16(100,200,BLUE,WHITE," OC1 ");//先恢复默认颜色
	Gui_DrawFont_GBK16(160,200,BLUE,WHITE," OC2 ");
	Gui_DrawFont_GBK16(220,200,BLUE,WHITE," OC3 ");
	if(NowSelect == 1)
	{
		Gui_DrawFont_GBK16(100,200,BLACK,RED," OC1 ");
	}
	else if(NowSelect == 2)
	{
		Gui_DrawFont_GBK16(160,200,BLACK,GREEN," OC2 ");
	}
	else if(NowSelect == 3)
	{
		Gui_DrawFont_GBK16(220,200,BLACK,BLUE," OC3 ");
	}
	
	//使能指示
	if(!OC1OutputEnable) Gui_DrawFont_GBK16(100,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(100,110,BLUE,YELLOW," Open ");
	if(!OC2OutputEnable) Gui_DrawFont_GBK16(160,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(160,110,BLUE,YELLOW," Open ");
	if(!OC3OutputEnable) Gui_DrawFont_GBK16(220,110,BLUE,GRAY0,"close ");
	else Gui_DrawFont_GBK16(220,110,BLUE,YELLOW," Open ");
	
	

	//OC1栏显示
	Gui_DrawNum_GBK16(100,180,BLUE,GRAY0,NowFreqOut[0]);
	Gui_DrawNum_GBK16(100,160,BLUE,GRAY0,NowAmplOut[0]);
//	Gui_DrawNum_GBK16(100,140,BLUE,GRAY0,NowCurrOut[0]);//显示电流
	//OC2栏显示
	Gui_DrawNum_GBK16(160,180,BLUE,GRAY0,NowFreqOut[1]);
	Gui_DrawNum_GBK16(160,160,BLUE,GRAY0,NowAmplOut[1]);
//	Gui_DrawNum_GBK16(160,140,BLUE,GRAY0,NowCurrOut[1]);//显示电流
	//OC3栏显示
	Gui_DrawNum_GBK16(220,180,BLUE,GRAY0,NowFreqOut[2]);
	Gui_DrawNum_GBK16(220,160,BLUE,GRAY0,NowAmplOut[2]);
//	Gui_DrawNum_GBK16(220,140,BLUE,GRAY0,NowCurrOut[2]);//显示电流
	//步进栏显示
	Gui_DrawNum_GBK16(280,180,BLUE,GRAY0,StepFreq);
	Gui_DrawNum_GBK16(280,160,BLUE,GRAY0,StepAmpl);

}

//数据存储任务
void TaskDataDtorage(void)
{
	u8 i=0;
	i=0;//存储幅值
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[0]);i++;
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[1]);i++;
	StorageWriteOneByte(Adress_AmplOut+i,NowAmplOut[2]);i++;
	i=0;//存储频率
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[0]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[0]%256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[1]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[1]%256);i++;	
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[2]/256);i++;
	StorageWriteOneByte(Adress_FreqOut+i,NowFreqOut[2]%256);i++;
	//写标志位
	StorageWriteOneByte(Adress_StorageFlag,0x55);	
}


//上电自检，读取历史数据
void HistoryDataCheck(void)
{
	u8 i;
	if(StorageReadOneByte(Adress_StorageFlag) != 0x55)
		return;	//第一次使用存储，没有历史数据，直接退出函数
	
	for(i=0;i<3;i++)
		NowAmplOut[i] = StorageReadOneByte(Adress_AmplOut+i);//读取上次的幅值数据设为当前输出
	
	for(i=0;i<6;i++)
	{
		NowFreqOut[i/2] = StorageReadOneByte(Adress_FreqOut+i);i++;
		NowFreqOut[i/2] <<= 8;
		NowFreqOut[i/2] += StorageReadOneByte(Adress_FreqOut+i);
	}
}



