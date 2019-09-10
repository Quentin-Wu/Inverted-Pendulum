#ifndef		__SRCEEN_H
#define		__SRCEEN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"


//时钟
#define SYSCTL_PERIPH_SCREEN  						SYSCTL_PERIPH_UART2
#define SYSCTL_PERIPH_SCREEN_GPIO 			  SYSCTL_PERIPH_GPIOD

//串口号
#define SCREEN_UART_BASE    							UART2_BASE
#define SCREEN_INT_UART										INT_UART2

//管脚
#define SCREEN_GPIO_BASE									GPIO_PORTD_BASE
#define SCREEN_GPIO_RX										GPIO_PD6_U2RX
#define SCREEN_GPIO_TX										GPIO_PD7_U2TX
#define SCREEN_GPIO_PIN_RX                GPIO_PIN_6
#define SCREEN_GPIO_PIN_TX 							  GPIO_PIN_7

//波特率
#define SCREEN_BAUD        								115200


void Screen_Init(void);
void sand_txt_time(uint16_t t1);
void txt_putbuff(char *buff, uint32_t len);
void sand_waveform(int a);
double set_center(void);
uint8_t set_cricle(void);
void show_center(double a);
void show_center_up(float a);
void show_center_down(float a);
#endif

