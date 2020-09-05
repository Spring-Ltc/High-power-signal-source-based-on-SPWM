
#include "CurrentSensorADC.h"
#include "delay.h"





//============================================负载电流检测 ADC 相关=========================================//
//使用ADC1的通道15


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
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//ADC相关参数配置
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//关闭扫描模式，单次转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐，都是12位的
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStruct.ADC_Mode= ADC_Mode_Independent;//独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//单通道模式
	ADC_Init(ADC2,&ADC_InitStruct);
	
	//复位和校准两步必须要有
	ADC_Cmd(ADC1,ENABLE);//使能
	ADC_ResetCalibration(ADC1);//复位
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);//校准
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_Init(ADC1,&ADC_InitStruct);
}

//获得ADC值
u16 GetCurrent_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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

//读取负载电流值，入口参数位通道，取值可选1.2.3
float ReadCurrent(u8 OCelect)
{
	float MeasureVoltage;
	u16 value;
	value = GetCurrent_Adc_Average(OCelect+3,5);//获取5次的平均值,ADC实际通道为电路板上的通道加3
	MeasureVoltage = 3.3 * value /4096;	//ADC是12位的
//	printf("Measure vlotage is: %f V\r\n",MeasureVoltage);//【测试用，记得注释掉】
	return MeasureVoltage*10 - 25;//根据ACS712的U-I曲线做数据转换【 U = 2.5 + I/10】即【I = 10*U - 25】
}










//============================================电源电压检测 ADC 相关=========================================//
//使用ADC2的通道15



//输入端电源电压检测
//	PC5---ADC12_IN15
void SupplyVoltageADC_Init(void)
{
	GPIO_InitTypeDef				GPIO_InitStruct;
	ADC_InitTypeDef					ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//使能ADC时钟
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//ADC相关参数配置
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//关闭扫描模式，单次转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐，都是12位的
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStruct.ADC_Mode= ADC_Mode_Independent;//独立工作模式
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;//单通道模式
	ADC_Init(ADC2,&ADC_InitStruct);
	
	//复位和校准两步必须要有
	ADC_Cmd(ADC2,ENABLE);//使能
	ADC_ResetCalibration(ADC2);//复位
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);//校准
	while(ADC_GetCalibrationStatus(ADC2));
}


//获得ADC值
u16 GetPower_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC2);	//返回最近一次ADC1规则组的转换结果
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

//读取电源电压值，单位mV
//电路上使用5.1k和1k电阻分压，实际采集电压为真实电压的 1/6.1
u16 ReadPowerVoltage(void)
{
	float MeasureVoltage;
	u16 value;
	value = GetPower_Adc_Average(15,10);//获取10次的平均值
	MeasureVoltage = 3300 * value /4096;	//ADC是12位的
//	printf("Measure data is: %f mV\r\n",MeasureVoltage);//【测试用，记得注释掉】
	return (u16)(MeasureVoltage*6.13);//受分压电阻精度和走线阻抗影响，做参数修正
}
