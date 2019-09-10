#ifndef __CONTROL_H
#define __CONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

#define SYSCTL_PERIPH_GPIO_M1_1		SYSCTL_PERIPH_GPIOB
#define GPIO_PORT_BASE_M1_1				GPIO_PORTB_BASE
#define GPIO_PIN_M1_1							GPIO_PIN_5
#define SYSCTL_PERIPH_GPIO_M1_2		SYSCTL_PERIPH_GPIOB
#define GPIO_PORT_BASE_M1_2				GPIO_PORTB_BASE
#define GPIO_PIN_M1_2 						GPIO_PIN_0

void mode_init(void);
void mode_1(void);
void mode_2(void);
void mode_3(uint16_t motor_center);
void mode_4(void);
void mode_5(void);
void mode_6(void);
void mode_7(void);

void Moto_Init(void);
void Motor_Out(int32_t out_x);
void M1_Forward(int32_t width);
void M1_Backward(int32_t width);

#define M1_STOP()			{GPIOPinWrite(GPIO_PORT_BASE_M1_1, GPIO_PIN_M1_1 , 0);GPIOPinWrite(GPIO_PORT_BASE_M1_2, GPIO_PIN_M1_2, 0);}

#endif





