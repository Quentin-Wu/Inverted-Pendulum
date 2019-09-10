#include "hcsr04.h"
#include "delay.h"
#include "myuart.h"

//使用定时器中断3
void Timer3IntHandler(void);

u16 msHcCount = 0;
u16 timer_init_count = 0;  //计数器初值

//1ms
void My_Timer3_Init(void)
{		
	//使能外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	//周期性计数模式
  TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	
	//设置计数范围
	TimerLoadSet(TIMER3_BASE, TIMER_A, (SysCtlClockGet()/1000)-1); //1ms
	
	//在系统层使能定时器中断
	IntEnable(INT_TIMER3A);
	//允许处理某个定时器的中断请求
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT); 
	//设置中断优先级
	IntPrioritySet(INT_TIMER3A, 3);
	//注册中断处理函数
	TimerIntRegister(TIMER3_BASE, TIMER_BOTH, Timer3IntHandler); 
	//关闭定时器
  TimerDisable(TIMER3_BASE, TIMER_A);
}

void Timer3IntHandler(void)
{
	msHcCount++;
	// 清除定时器中断标志位
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
}

void Hcsr04_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_TRIG);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG);
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, 0);
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_ECHO);
	GPIOPinTypeGPIOInput(GPIO_PORT_BASE_ECHO,GPIO_PIN_ECHO);
//	GPIOPadConfigSet(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_OD); 
	
	My_Timer3_Init();
}

/************开关点定时器******/
static void OpenTimerForHc(){
	msHcCount = 0;
	TimerLoadSet(TIMER3_BASE, TIMER_A, (SysCtlClockGet()/1000)-1);
	TimerEnable(TIMER3_BASE, TIMER_A);
	timer_init_count = TimerValueGet(TIMER3_BASE, TIMER_A) /80 ;
}

static void CloseTimerForHc(){
	TimerDisable(TIMER3_BASE, TIMER_A);
}

/********获取定时器时间us********************/
u32 GetEchoTimer(void){
	u32 t = 0;
	t = msHcCount*1000; //得到us
//	printf("msHcCount: %d\n",msHcCount);
	t += (1000- (TimerValueGet(TIMER3_BASE, TIMER_A) /80)); //得到us
//	printf("TimerValue: %d\n",(1000- (TimerValueGet(TIMER3_BASE, TIMER_A) /80)));
//	t += timer_init_count;
//	printf("timer_init_count: %d\n",timer_init_count);
//	TimerLoadSet(TIMER3_BASE, TIMER_A, 0);
	delay_ms(50);
	msHcCount = 0;
	return t;
}

/*******超声波测距*****/
float Hcsr04GetLength(void){
	u32 t= 0;
	double lengthTemp = 0;
	
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, GPIO_PIN_TRIG); //发送高电平
	delay_us(20);
	GPIOPinWrite(GPIO_PORT_BASE_TRIG,GPIO_PIN_TRIG, 0);
	
	while(GPIOPinRead(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO) != GPIO_PIN_ECHO);
	OpenTimerForHc();
	while(GPIOPinRead(GPIO_PORT_BASE_ECHO, GPIO_PIN_ECHO) == GPIO_PIN_ECHO);
	CloseTimerForHc();
	
	t = GetEchoTimer();
	lengthTemp = (float)t*0.017;
//	printf("lengthTemp:%f\n\n",lengthTemp);

	return (float)lengthTemp;
}





