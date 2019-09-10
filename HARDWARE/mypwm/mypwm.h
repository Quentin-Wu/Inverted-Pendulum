#ifndef __MYPWM_H
#define __MYPWM_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"

void  M1PWM0_Init(uint32_t freq, float width);
void  M1PWM1_Init(uint32_t freq, float width);

void M1PWM0_PWM_Set(uint32_t width);
void M1PWM1_PWM_Set(uint32_t width);

#endif

