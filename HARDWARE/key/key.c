#include "key.h"

volatile uint16_t keyvalue = 0;

void KEY_IntHandler(void)
{
	uint32_t IntStatus_raw;
	IntStatus_raw = GPIOIntStatus(GPIO_PORT_KEY_BASE, true);	//读取GPIO值
	
	if((IntStatus_raw & SW1)==SW1)  // SW1引脚有中断事件
	{
			keyvalue = 1;
			GPIOIntClear(GPIO_PORT_KEY_BASE,SW1);//清除SW1
	}
	
	if((IntStatus_raw & SW2)==SW2)  // SW2引脚有中断事件
	{
			keyvalue = 2;
			GPIOIntClear(GPIO_PORT_KEY_BASE,SW2);//清除SW2
	}

}

void KEY_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_KEY);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIO_KEY));
	
	//解锁PF0(SW2按键)
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;
	
	GPIOPinTypeGPIOInput(GPIO_PORT_KEY_BASE, SW1 |SW2 );	
	GPIOPadConfigSet(GPIO_PORT_KEY_BASE, SW1 |SW2 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	//中断 下降沿触发
	GPIOIntTypeSet(GPIO_PORT_KEY_BASE, SW1 |SW2 , GPIO_FALLING_EDGE );
	//使能中断
	GPIOIntEnable(GPIO_PORT_KEY_BASE, SW1 |SW2 );
	//注册中断处理句柄
	GPIOIntRegister(GPIO_PORT_KEY_BASE, KEY_IntHandler);
	//清除中断标志位
	GPIOIntClear(GPIO_PORT_KEY_BASE, SW1 |SW2 );

}


