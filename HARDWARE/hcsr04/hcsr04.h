#ifndef __HCSR04_H
#define __HCSR04_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


#define SYSCTL_PERIPH_GPIO_TRIG		SYSCTL_PERIPH_GPIOC
#define GPIO_PORT_BASE_TRIG				GPIO_PORTC_BASE
#define GPIO_PIN_TRIG							GPIO_PIN_4

#define SYSCTL_PERIPH_GPIO_ECHO		SYSCTL_PERIPH_GPIOC
#define GPIO_PORT_BASE_ECHO				GPIO_PORTC_BASE
#define GPIO_PIN_ECHO							GPIO_PIN_5

void Hcsr04_Init(void);
float Hcsr04GetLength(void);




#endif

