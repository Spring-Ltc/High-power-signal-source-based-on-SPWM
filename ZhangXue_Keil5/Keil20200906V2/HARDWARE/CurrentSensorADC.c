
#include "CurrentSensorADC.h"
#include "delay.h"





//============================================���ص������ ADC ���=========================================//
//ʹ��ADC1��ͨ��15


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
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//ADC��ز�������
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//�ر�ɨ��ģʽ������ת��
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ��룬����12λ��
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�������
	ADC_InitStruct.ADC_Mode= ADC_Mode_Independent;//��������ģʽ
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//��ͨ��ģʽ
	ADC_Init(ADC2,&ADC_InitStruct);
	
	//��λ��У׼��������Ҫ��
	ADC_Cmd(ADC1,ENABLE);//ʹ��
	ADC_ResetCalibration(ADC1);//��λ
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);//У׼
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_Init(ADC1,&ADC_InitStruct);
}

//���ADCֵ
u16 GetCurrent_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 GetCurrent_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=GetCurrent_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

//��ȡ���ص���ֵ����ڲ���λͨ����ȡֵ��ѡ1.2.3
float ReadCurrent(u8 OCelect)
{
	float MeasureVoltage;
	u16 value;
	value = GetCurrent_Adc_Average(OCelect+3,5);//��ȡ5�ε�ƽ��ֵ,ADCʵ��ͨ��Ϊ��·���ϵ�ͨ����3
	MeasureVoltage = 3.3 * value /4096;	//ADC��12λ��
//	printf("Measure vlotage is: %f V\r\n",MeasureVoltage);//�������ã��ǵ�ע�͵���
	return MeasureVoltage*10 - 25;//����ACS712��U-I����������ת���� U = 2.5 + I/10������I = 10*U - 25��
}










//============================================��Դ��ѹ��� ADC ���=========================================//
//ʹ��ADC2��ͨ��15



//����˵�Դ��ѹ���
//	PC5---ADC12_IN15
void SupplyVoltageADC_Init(void)
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	ADC_InitTypeDef					ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//ʹ��ADCʱ��
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//ADC��ز�������
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//�ر�ɨ��ģʽ������ת��
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//�����Ҷ��룬����12λ��
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�������
	ADC_InitStruct.ADC_Mode= ADC_Mode_Independent;//��������ģʽ
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//��ͨ��ģʽ
	ADC_Init(ADC2,&ADC_InitStruct);
	
	//��λ��У׼��������Ҫ��
	ADC_Cmd(ADC2,ENABLE);//ʹ��
	ADC_ResetCalibration(ADC2);//��λ
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);//У׼
	while(ADC_GetCalibrationStatus(ADC2));
}


//���ADCֵ
u16 GetPower_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC2);	//�������һ��ADC1�������ת�����
}

u16 GetPower_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=GetPower_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

//��ȡ��Դ��ѹֵ����λmV
//��·��ʹ��5.1k��1k�����ѹ��ʵ�ʲɼ���ѹΪ��ʵ��ѹ�� 1/6.1
u16 ReadPowerVoltage(void)
{
	float MeasureVoltage;
	u16 value;
	value = GetPower_Adc_Average(15,10);//��ȡ10�ε�ƽ��ֵ
	MeasureVoltage = 3300 * value /4096;	//ADC��12λ��
//	printf("Measure data is: %f mV\r\n",MeasureVoltage);//�������ã��ǵ�ע�͵���
	return (u16)(MeasureVoltage*6.13);//�ܷ�ѹ���辫�Ⱥ������迹Ӱ�죬����������
}
