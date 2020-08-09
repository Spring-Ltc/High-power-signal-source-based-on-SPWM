



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



int main(void)
{

//	NVIC_Configuration();    
	SystemInit();	// ����ϵͳʱ��Ϊ72M 	
	
	uart_init(115200);//����1��USB����
		
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




