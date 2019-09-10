#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "math.h"

typedef struct
{
	float get;
	float set;
	float out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Ek;
	float Ek1;
	float Ek_sum;
}PIDTypedef;

void PID_Init(void);
void Set_SPEED_Kp(float kpp);
void Set_SPEED_Kd(float kdd);
void Set_SPEED_Ki(float kii);
void SPEED_PID_realize(void);

void Set_ANGEL_Kp(float kpp);
void Set_ANGEL_Kd(float kdd);
void Set_ANGEL_Ki(float kii);
void ANGEL_PID_realize(void);


#endif

