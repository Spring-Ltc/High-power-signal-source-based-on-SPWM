



/********************�����ʵ��ѧ2020�굥Ƭ��Ӧ����ƾ���������ѧԺ��************************************

MCU�ͺţ�STM32F103RCT6
���ߣ�Liao_Tingchun�����䣺2093181896@qq.com
�޸���־��
2020/07/19/00:49
	�����¹��̣�����ȷ��MCU��Դ����
2020/07/19/18:03
	���ճ������������½�HARDWARE�¸��ļ�
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


#define LED PAout(0)

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOA,GPIO_Pin_0);						 //PB.5 �����
}

int main(void)
{

//	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	
//	uart_init(115200);//����1��USB����
		
	//PWM_Time8Init();
	LED_Init();
	//PWM_Init(500);
	//PWM_Time1Init();
	PWM_Time8Init();
	
	delay_init();
	LED=1;
  while (1)
  {
		delay_ms(500);//delay_ms(1500);delay_ms(1500);delay_ms(1500);
//		delay_ms(1500);delay_ms(1500);delay_ms(1500);delay_ms(1500);
//		delay_ms(1500);delay_ms(1500);delay_ms(1500);delay_ms(1500);
		LED=!LED;

  }
}




