
#include "CurrentSensorADC.h"
#include "delay.h"


//初始化电流检测的三个ADC通道
//		ADC_OC1---->PA6/ADC12_IN6
//		ADC_OC2---->PA5/ADC12_IN5
//		ADC_OC3---->PA4/ADC12_IN4
void CurrentADC_Init(void)
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	ADC_InitTypeDef					ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能ADC时钟
	
//RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//ADC相关参数配置
//	ADC_InitStruct.ADC_ContinuousConvMode=
//	ADC_InitStruct.ADC_DataAlign=
//	ADC_InitStruct.ADC_ExternalTrigConv=
//	ADC_InitStruct.ADC_Mode=
//	ADC_InitStruct.ADC_NbrOfChannel=
//	ADC_InitStruct.ADC_ScanConvMode=
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE);
}








//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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
