#include "mypwm.h"

//PD0
void  M1PWM0_Init(uint32_t freq, float width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	
	//���� PWM1 Generator3��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator3 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 1250*freq - 1);  //us
	//���� PWM1 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 125*width/100 - 1);  //us
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

//PD1
void  M1PWM1_Init(uint32_t freq, float width)
{
	//����PWMʱ��Ϊϵͳʱ�ӵ�8��Ƶ
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	//ʹ��PWM1����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	
	//��������ΪPWM����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
	
	//���� PWM1 Generator3��������
  PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	//���� PWM1 Generator3 ����
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 1250*freq - 1);
	//���� PWM1 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 125*width/100 - 1);
	
	//ʹ�� PWM1 ���
  PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
  //ʹ�� PWM1 ������ģ��
  PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}

void M1PWM0_PWM_Set(uint32_t width)
{
	//���� PWM1 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1.25*width - 1);
}

void M1PWM1_PWM_Set(uint32_t width)
{
	//���� PWM1 Generator3 ռ�ձ�
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 1.25*width - 1);
}



