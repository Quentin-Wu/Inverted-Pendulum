#include "mypwm.h"

//PD0
void  M1PWM0_Init(uint32_t freq, float width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	
	//配置 PWM1 Generator3·发生器
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM1 Generator3 周期
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 1250*freq - 1);  //us
	//配置 PWM1 Generator3 占空比
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 125*width/100 - 1);  //us
	
	//使能 PWM1 输出
  PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
  //使能 PWM1 发生器模块
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

//PD1
void  M1PWM1_Init(uint32_t freq, float width)
{
	//设置PWM时钟为系统时钟的8分频
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//使能PWM1外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//配置引脚为PWM功能
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	//配置 PWM1 Generator3·发生器
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//配置 PWM1 Generator3 周期
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 1250*freq - 1);
	//配置 PWM1 Generator3 占空比
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 125*width/100 - 1);
	
	//使能 PWM1 输出
  PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
  //使能 PWM1 发生器模块
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

void M1PWM0_PWM_Set(uint32_t width)
{
	//配置 PWM1 Generator3 占空比
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1.25*width - 1);
}

void M1PWM1_PWM_Set(uint32_t width)
{
	//配置 PWM1 Generator3 占空比
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 1.25*width - 1);
}



