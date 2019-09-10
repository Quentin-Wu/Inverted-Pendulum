#ifndef		__LED_H
#define		__LED_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"


#define	SYSCTL_PERIPH_GPIO_LED	SYSCTL_PERIPH_GPIOF
#define GPIO_PORT_LED_BASE			GPIO_PORTF_BASE

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

#define ON  1
#define	OFF 0

#define LED_RED(a)	if(a) \
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , RED_LED);	\
					else		\
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED , 0)	
					
#define LED_BLUE(a)	if(a) \
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED, BLUE_LED);	\
					else		\
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED, 0)	
					
#define LED_GREEN(a)	if(a) \
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED, GREEN_LED);	\
					else		\
					GPIOPinWrite(GPIO_PORT_LED_BASE, RED_LED |BLUE_LED |GREEN_LED, 0)	
	
#define LED_GREEN_DUBUG(a)	if(a) \
					GPIOPinWrite(GPIO_PORT_LED_BASE, GREEN_LED, GREEN_LED);	\
					else		\
					GPIOPinWrite(GPIO_PORT_LED_BASE, GREEN_LED, 0)			

#define LED_RED_TUGGEL 		{GPIOPinWrite(GPIO_PORTF_BASE, RED_LED,(GPIOPinRead(GPIO_PORT_LED_BASE, RED_LED) == 0) ? 			RED_LED : 0);}
#define LED_BLUE_TUGGEL 	{GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED,(GPIOPinRead(GPIO_PORT_LED_BASE, BLUE_LED) == 0) ? 		BLUE_LED : 0);}
#define LED_GREEN_TUGGEL 	{GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED,(GPIOPinRead(GPIO_PORT_LED_BASE, GREEN_LED) == 0) ? 	GREEN_LED : 0);}					

void LED_Init(void);
void LED_RED_DEBUG(bool a);
void LED_BLUE_DEBUG(bool a);
void LED_GREEN_DEBUG(bool a);
#endif

