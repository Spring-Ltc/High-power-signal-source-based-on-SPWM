



/********************�����ʵ��ѧ2020�굥Ƭ��Ӧ����ƾ���������ѧԺ��************************************

MCU�ͺţ�STM32F103RCT6
���ߣ�Liao_Tingchun�����䣺2093181896@qq.com
�޸���־��
2020/07/19/00:49
	�����¹��̣�����ȷ��MCU��Դ����
2020/07/19/18:03
	���ճ������������½�HARDWARE�¸��ļ�
2020/08/08/22:40
	����ȷ����PCB������MCU��Դ������س�ʼ��(���뿪�ء�������⡢LED��KEY�Ȼ���)
2020/08/09/13:14
	��ֲIIC��ش��롢TFTҺ������ش��룬���д�У�飬��ģ���ʼ������ȫ�����
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
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	delay_init();
	uart_init(9600);//����1��USB����
		
	
	LED_Init();
//	//PWM_Time8Init();
//	LED_Init();
//	//PWM_Init(500);
//	//PWM_Time1Init();
//	PWM_Time8Init();
//

//	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��PWM��Χ��
//		TIM_Cmd(TIM1, ENABLE);//ʹ�ܶ�ʱ��1
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




