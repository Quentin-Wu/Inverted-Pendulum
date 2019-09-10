#include "myadc.h"
#include "led.h"

void My_Adc_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	
	ADCSequenceEnable(ADC0_BASE, 1);
	ADCIntEnable(ADC0_BASE,1);
}

float My_Adc_Get(void)
{
	uint32_t ui32ADC0Value[4];
	//ADC中断清零清零
	ADCIntClear(ADC0_BASE, 1);
	//ADC处理触发器
	ADCProcessorTrigger(ADC0_BASE, 1);
	//等ADC转换结束
	while(!ADCIntStatus(ADC0_BASE, 1, false));
	//获取ADC序列数据
	ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
	
	return  (float)(ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3])/4*360/4096;
}




