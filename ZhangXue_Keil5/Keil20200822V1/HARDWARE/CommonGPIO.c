

#include "CommonGPIO.h"


void KEY_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//GPIO��ʼ��		4������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}


void LED_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	//GPIO��ʼ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;		//LED_EN_OC1
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;		//LED_System
		GPIO_Init(GPIOB,&GPIO_InitStruct);
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_9;
		GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_SetBits(GPIOA,GPIO_Pin_8);//��ʼ��ƽ���ߣ��ر�LED
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);//�ϵ�Ĭ�ϵ�ǰѡ��ͨ��0
}


void BUZZ_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;		//BUZZ
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);//��ʼ��ƽ���ͣ��رշ�����
}

