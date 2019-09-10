#include "led.h"


void LED_Init(void)
{
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_LED);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIO_LED));
	
	GPIOPinTypeGPIOOutput(GPIO_PORT_LED_BASE, 
																				RED_LED| BLUE_LED |GREEN_LED);
	GPIOPinWrite(GPIO_PORT_LED_BASE,
															RED_LED |BLUE_LED |GREEN_LED, 0);
	
}

void LED_RED_DEBUG(bool a)	
{
		if(a) GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , RED_LED);	
		else GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , 0);	
}	

void LED_BLUE_DEBUG(bool a)	
{
		if(a) GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , BLUE_LED);	
		else GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , 0);	
}	

void LED_GREEN_DEBUG(bool a)	
{
		if(a) GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , GREEN_LED);	
		else GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , 0);	
}	

