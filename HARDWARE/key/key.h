#ifndef		__KEY_H
#define		__KEY_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"


#define	SYSCTL_PERIPH_GPIO_KEY		SYSCTL_PERIPH_GPIOF
#define GPIO_PORT_KEY_BASE				GPIO_PORTF_BASE

#define	SW1		GPIO_PIN_4
#define	SW2		GPIO_PIN_0

void KEY_Init(void);





#endif
