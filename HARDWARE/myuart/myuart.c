#include "myuart.h"
#include "led.h"

void UART0IntHandler(void);

uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	

void My_Uart_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//GPIO复用模式配置
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(),
                    115200, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(UART0_BASE);
	//使能UART0中断
	IntEnable(INT_UART0);
	//使能UART0接收中断
	UARTIntEnable(UART0_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(UART0_BASE,UART0IntHandler);
	IntPrioritySet(INT_UART0, 0);
	
}

void UART0IntHandler(void)
{
	//获取中断标志 原始中断状态 不屏蔽中断标志
//	uint32_t flag = UARTIntStatus(UART0_BASE,0);
	//获取中断标志 原始中断状态 屏蔽中断标志
	uint32_t flag = UARTIntStatus(UART0_BASE,1);
	uint8_t	Res;

	//清除中断标志
	UARTIntClear(UART0_BASE,flag);
	
	if(flag&UART_INT_RX)	//接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//UARTCharsAvail()判断FIFO是否还有数据
//		while(UARTCharsAvail(UART0_BASE))
		Res =UARTCharGet(UART0_BASE);	//读取接收到的数据
	
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}
				}
			}
	}
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		UARTCharPut(UART0_BASE, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (UARTCharsAvail(UART0_BASE));		
	
		return(ch);
}

//发送指定长度的数组
void usart_putbuff(uint8_t *buff, uint32_t len)
{
//	LED_GREEN_DUBUG(ON);
	while(len--)
	{
		UARTCharPut(UART0_BASE,*buff);
		buff++;
	}
//	LED_GREEN_DUBUG(OFF);
}

//根据通信协议向上位机发送数据
void vcan_sendware(uint8_t *wareaddr, uint32_t waresize)
{
	#define CMD_WARE												3 //3表示虚拟示波器，1摄像头，2CCD
	uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE};
	uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE};
	
	usart_putbuff(cmdf, sizeof(cmdf));
	usart_putbuff(wareaddr, waresize);
	usart_putbuff(cmdr, sizeof(cmdr));
}

