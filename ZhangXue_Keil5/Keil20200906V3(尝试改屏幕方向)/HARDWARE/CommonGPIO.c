

#include "CommonGPIO.h"
#include "delay.h"

//��������Ϊ�жϴ���
void KEY_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��������ʱ��
	
	//GPIO��ʼ��		4������
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	//�ж�������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct .EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_Init(&EXTI_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
	EXTI_InitStruct.EXTI_Line = EXTI_Line11;
	EXTI_Init(&EXTI_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStruct);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStruct);

	//�ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority =2;
	NVIC_Init(&NVIC_InitStruct);
	
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_EN_OC1==0||KEY_EN_OC2==0||KEY_EN_OC3==0||KEY_OC_Switch==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_EN_OC1==0)
		{
			while(KEY_EN_OC1==0);//���ּ��
			return KEY_EN_OC1_PRES;
		}
		else if(KEY_EN_OC2==0)
		{
			while(KEY_EN_OC2==0);//���ּ��
			return KEY_EN_OC2_PRES;
		}
		else if(KEY_EN_OC3==0)
		{
			while(KEY_EN_OC3==0);//���ּ��
			return KEY_EN_OC3_PRES;
		}
		else if(KEY_OC_Switch==0)
		{
			while(KEY_OC_Switch==0);//���ּ��
			return KEY_OC_Switch_PRES;
		}
	}
	else if(KEY_EN_OC1==1&&KEY_EN_OC1==1&&KEY_EN_OC1==1&&KEY_OC_Switch==1)key_up=1; 	     
	return 0;// �ް�������
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
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);
	LED_Select_OC1 = 0;//Ĭ��ѡ��ͨ��1
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



