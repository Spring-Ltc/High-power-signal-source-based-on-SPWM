
#include "CurrentSensorADC.h"
#include "delay.h"


//��ʼ��������������ADCͨ��
//		ADC_OC1---->PA6/ADC12_IN6
//		ADC_OC2---->PA5/ADC12_IN5
//		ADC_OC3---->PA4/ADC12_IN4
void CurrentADC_Init(void)
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	ADC_InitTypeDef					ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADCʱ��
	
//RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//ADC��ز�������
//	ADC_InitStruct.ADC_ContinuousConvMode=
//	ADC_InitStruct.ADC_DataAlign=
//	ADC_InitStruct.ADC_ExternalTrigConv=
//	ADC_InitStruct.ADC_Mode=
//	ADC_InitStruct.ADC_NbrOfChannel=
//	ADC_InitStruct.ADC_ScanConvMode=
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE);
}








//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 
