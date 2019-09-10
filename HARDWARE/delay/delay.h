#ifndef	__DELAY_H
#define __DELAY_H

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"


void delay_us(uint16_t xus);
void delay_ms(uint16_t xms);


#endif



