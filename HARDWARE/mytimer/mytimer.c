#include "mytimer.h"
#include "myadc.h"
#include "myuart.h"
#include "led.h"
#include "myqei.h"
#include "control.h"
#include "pid.h"

extern uint8_t mode;

extern PIDTypedef SPEED_PID,ANGEL_PID;

void Timer3IntHandler(void);

void My_Timer3_Init(uint16_t xms)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	//周期性计数模式
  TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(TIMER3_BASE, TIMER_A, 80000*xms);
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER3A);
	//允许处理某个定时器的中断请求
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER3A, 1<<5);
	//注册中断处理函数
	TimerIntRegister(TIMER3_BASE, TIMER_BOTH, Timer3IntHandler); 
	//开启定时器
  TimerEnable(TIMER3_BASE, TIMER_A);
}

void Timer3IntHandler(void)
{
	// 清除定时器中断标志位
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
		ANGEL_PID.get = My_Adc_Get();

	switch(mode)
		{
			case 1: mode_1(); 
//							printf("task1");
							break;
			case 2: mode_2();
//							printf("task2");  	
							break;
			case 3: mode_3(QEI_Center);
//							printf("task3");
							break;
			case 5: mode_4();
//							printf("task4");
							break;
			case 6: mode_5();
//							printf("task5");
							break;
			case 7: mode_6();
//							printf("task6");
							break;
		  case 8: mode_7(); 
//							printf("task7");
							break; 
			default: Motor_Out(0); break;
		}

	
}


