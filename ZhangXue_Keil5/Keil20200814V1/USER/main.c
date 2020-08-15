



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



#define LED PAout(0)


void MyDelay(u32 time)
{
	u16 i;
	while(--time);
		for(i=0;i<10000;i++);
}

int main(void)
{

	NVIC_Configuration();    
	SystemInit();	// 配置系统时钟为72M 	
	delay_init();
	uart_init(9600);//串口1，USB连接
		
	
	LED_Init();
//	//PWM_Time8Init();
//	LED_Init();
//	//PWM_Init(500);
//	//PWM_Time1Init();
//	PWM_Time8Init();
//

//	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输
//		TIM_Cmd(TIM1, ENABLE);//使能定时器1
	PWM_Time8Init();
	
	USART_SendData(USART1,0xaa);
	LED_System=1;
  while (1)
  {
		//delay_ms(500);//delay_ms(1500);delay_ms(1500);delay_ms(1500);
//		delay_ms(1500);delay_ms(1500);delay_ms(1500);delay_ms(1500);
//		delay_ms(1500);delay_ms(1500);delay_ms(1500);delay_ms(1500);
		//LED=!LED;
	//	printf("COM Text\r\n\r\n");
		LED_System =!LED_System;
	//printf("AA");
		//USART_SendData(USART1,0xaa);
		MyDelay(0xffff);MyDelay(0xffff);MyDelay(0xffff);MyDelay(0xffff);MyDelay(0xffff);
  }
}




