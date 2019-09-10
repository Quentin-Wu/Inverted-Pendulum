#include "us_100.h"
#include "led.h"
#include "delay.h"

//串口接收 uart4
//PC4 -> Rx
//PC5 -> Tx

void UART4IntHandler(void);

void My_Uart4_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//GPIO复用模式配置
	GPIOPinConfigure(GPIO_PC4_U4RX);
	GPIOPinConfigure(GPIO_PC5_U4TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(),
                    9600, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
//	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
//	//禁用后接收1位就产生中断
//	UARTFIFODisable(UART4_BASE);
////	UARTFIFOEnable(UART4_BASE);
//	//使能UART0中断
//	IntEnable(INT_UART4);
//	//使能UART0接收中断
//	UARTIntEnable(UART4_BASE,UART_INT_RX |UART_INT_RT);
//	//UART中断地址注册
//	UARTIntRegister(UART4_BASE,UART4IntHandler);
}

//void UART4IntHandler(void)
//{
//	uint32_t flag = UARTIntStatus(UART4_BASE,1);
//	int32_t a,b=0;
//	float length;
//	
//	UARTIntClear(UART4_BASE,flag);
//	
//	a = UARTCharGet(UART4_BASE);
//	b = UARTCharGet(UART4_BASE);
//	length = (float)(a*256 + b)/10; //读取FIFO中的数据并处理
//	
//	LED_GREEN_TUGGEL
//}


//非中断处理
bool us_100_GetLength(float* length)
{
	int32_t a,b=0;
	//发送读取距离命令 0x55
	UARTCharPut(UART4_BASE, 0x55);	
	
	//如果FIFO中有数据
	if(UARTCharsAvail(UART4_BASE) == true)
	{	
		a = UARTCharGet(UART4_BASE);
		b = UARTCharGet(UART4_BASE);
		*length = (float)(a*256 + b)/10; //读取FIFO中的数据并处理
		LED_GREEN_TUGGEL
		return 0;
	}
	else
	{	
		return 1;
	}
}










