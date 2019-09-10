#include "screen.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"


int num_screen = 0;
uint8_t  startBit_Screen = 0;
uint8_t  endBit_Screen = 0;
uint16_t intpurscreenString[80] = {0};
uint16_t SccreenBit = 0;
extern uint8_t mode;
extern void mode_change(uint8_t modee);
extern void mode7_change(float mode7_centerr);
extern void mode2_change(uint8_t mode_2_counttt);
extern float angel_up ;
extern float angel_down ;

float angel_up_test ;
float angel_down_test ;


void txt_putbuff(char *buff, uint32_t len);
uint8_t set_cricle(void);




//中断服务函数
void ScreenIntHandler(void)
{
		uint32_t flag = UARTIntStatus(SCREEN_GPIO_BASE,1);
		//清除中断标志
		UARTIntClear(SCREEN_GPIO_BASE,flag);

		SccreenBit =UARTCharGet(SCREEN_UART_BASE);	//读取接收到的数据

		
	
	//接受任务
		if(SccreenBit == '$' || SccreenBit =='%' || SccreenBit=='&')
	    {
	      	startBit_Screen = 1;
					num_screen = 0;
	    }
	    if(startBit_Screen == 1)
	    {
	       	intpurscreenString[num_screen] = SccreenBit; 
					
	    }  
	    if (startBit_Screen == 1 && SccreenBit == '#') 
	    {		
					endBit_Screen = 1; 
					startBit_Screen = 0;
					mode_change(intpurscreenString[1]-0x30);
					if(intpurscreenString[0]=='%')
					{
						int send_center=intpurscreenString[2]+intpurscreenString[3]*256;
						mode7_change(send_center);
					}
					if(intpurscreenString[1]=='c')
					{
						mode2_change(set_cricle());
					}
					if(intpurscreenString[0]=='&')
					{
						if(intpurscreenString[1]=='u')
							{
								angel_up=set_center();
								printf("angel_up_test=%f\n",angel_up);
							}
						if(intpurscreenString[1]=='d')
							{
								angel_down=set_center();
								printf("angel_down_test=%f\n",angel_down);
							}
						
					}
	    }
			num_screen++;
			if(num_screen >= 80)
				{
					num_screen = 0;
					startBit_Screen = 0;
					endBit_Screen	= 0;
				}	 
}


//初始化
void Screen_Init(void)
{
	//使能UART外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN);
	//使能GPIO外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SCREEN_GPIO);
//	//GPIO复用模式配置
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xFF;
	
	GPIOPinConfigure(SCREEN_GPIO_RX);
	GPIOPinConfigure(SCREEN_GPIO_TX);
	GPIOPinTypeUART(SCREEN_GPIO_BASE, SCREEN_GPIO_PIN_RX |SCREEN_GPIO_PIN_TX);
	//uart配置 波特率：115200 8位 1位停止位 无校验
	UARTConfigSetExpClk(SCREEN_UART_BASE, SysCtlClockGet(),
                    SCREEN_BAUD, (UART_CONFIG_WLEN_8 |UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
	
	//UART禁用FIFO 默认FIFO Level为4/8 寄存器满8字节后产生中断
	//禁用后接收1位就产生中断
	UARTFIFODisable(SCREEN_GPIO_BASE);
	//使能UART0中断
	IntEnable(SCREEN_INT_UART);
	//使能UART0接收中断
	UARTIntEnable(SCREEN_UART_BASE,UART_INT_RX |UART_INT_RT);
	//UART中断地址注册
	UARTIntRegister(SCREEN_UART_BASE,ScreenIntHandler);
	
	IntPrioritySet(SCREEN_INT_UART, 3<<5);
	
	
}

//写数组
void txt_putbuff(char *buff, uint32_t len)
{
	while(len--)
	{
		UARTCharPut(UART2_BASE,*buff);
		buff++;
	}
}

//显示时间函数
void sand_txt_time(uint16_t t1)				//t1为1s t2为0.1s
{
	int t10s=t1/100+48,
		  t1s =(t1/10)%10+48,
			t01s=t1%10+48;
		
			
			
	char txt[13]={0};
			 txt[0]='t';
			 txt[1]='2';
			 txt[2]='.';
			 txt[3]='t';
			 txt[4]='x';
			 txt[5]='t';
			 txt[6]='=';
			 txt[7]='"';
			 txt[8]=t10s;
			 txt[9]=t1s;
			 txt[10]='.';
			 txt[11]=t01s;
			 txt[12]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) txt,13);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//显示波形
void sand_waveform(int a)//a最大为2048
{
	char pwm_waveform[15];
	int b,a1,a2,a3;
//	if(a>90)
//		{
//			b=(a-90)/1.4;
//		}
//		else {
//		b=(a+270)/1.4;
//		}
	b=a/1.4;
	a1=(b/100)%10+48;
	a2=(b/10)%10+48;
	a3=b%10+48;
	pwm_waveform[0]='a';
	pwm_waveform[1]='d';
	pwm_waveform[2]='d';
	pwm_waveform[3]=' ';
	pwm_waveform[4]='2';
	pwm_waveform[5]=',';
	pwm_waveform[6]='1';
	pwm_waveform[7]=',';
	pwm_waveform[8]=a1;
	pwm_waveform[9]=a2;
	pwm_waveform[10]=a3;

	double Hex_end[4]={0};
				 Hex_end[0]=0xFF;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;	
		txt_putbuff((char*) pwm_waveform,11);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);
}


//显示波形值
void show_center(double a)
{
	int b,t1,t2,t3,t4;
	b=a*10;
	t1=(b/1000)+48;
	t2=(b/100)%10+48;
	t3=(b/10)%10+48;
	t4=(b%10)+48;
		char centerr[15]={0};
					centerr[0]='t';
					centerr[1]='1';
					centerr[2]='5';
					centerr[3]='.';
					centerr[4]='t';
					centerr[5]='x';
					centerr[6]='t';
					centerr[7]='=';
					centerr[8]='"';
					centerr[9]=t1;
					centerr[10]=t2;
					centerr[11]=t3;
					centerr[12]='.';
					centerr[13]=t4;
					centerr[14]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) centerr,15);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//显示上中值
void show_center_up(float a)
{
	int b,t1,t2,t3,t4;
	b=a*10;
	t1=(b/1000)+48;
	t2=(b/100)%10+48;
	t3=(b/10)%10+48;
	t4=(b%10)+48;
		char center_up[14]={0};
					center_up[0]='t';
					center_up[1]='u';
					center_up[2]='.';
					center_up[3]='t';
					center_up[4]='x';
					center_up[5]='t';
					center_up[6]='=';
					center_up[7]='"';
					center_up[8]=t1;
					center_up[9]=t2;
					center_up[10]=t3;
					center_up[11]='.';
					center_up[12]=t4;
					center_up[13]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) center_up,14);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//显示下中值
void show_center_down(float a)
{
	int b,t1,t2,t3,t4;
	b=a*10;
	t1=(b/1000)+48;
	t2=(b/100)%10+48;
	t3=(b/10)%10+48;
	t4=(b%10)+48;
	char  center_down[14]={0};
				center_down[0]='t';
				center_down[1]='d';
				center_down[2]='.';
				center_down[3]='t';
				center_down[4]='x';
				center_down[5]='t';
				center_down[6]='=';
				center_down[7]='"';
				center_down[8]=t1;
				center_down[9]=t2;
				center_down[10]=t3;
				center_down[11]='.';
				center_down[12]=t4;
				center_down[13]='"';
	double Hex_end[4]={0};
				 Hex_end[0]=0xff;
				 Hex_end[1]=0xff;
				 Hex_end[2]=0xff;
		txt_putbuff((char*) center_down,14);
		UARTCharPut(UART0_BASE,Hex_end[0]);		
		UARTCharPut(UART2_BASE,Hex_end[0]);
		UARTCharPut(UART0_BASE,Hex_end[1]);		
		UARTCharPut(UART2_BASE,Hex_end[1]);
		UARTCharPut(UART0_BASE,Hex_end[2]);
		UARTCharPut(UART2_BASE,Hex_end[1]);

}

//设置中值
double set_center()
{
	int findend=2;
	int flag=1;
	double center_up=0;
	double center_down=0;
	double set_center=0;
	if(intpurscreenString[1]=='u')
	{
			for(;intpurscreenString[findend]!='#';findend++)
			{

				if(flag==0)
				{
						center_up=center_up*10+intpurscreenString[findend]-48;
						center_up=center_up/10;	
				}	
				if(intpurscreenString[findend]=='.')
				{
						flag=0;
				}
				if(flag==1)
				{
					center_up=center_up*10+intpurscreenString[findend]-48;
					
				}
			}
			set_center= center_up;
			//printf("cnter_up_end=%f\n",center_up);
	}
	if(intpurscreenString[1]=='d')
	{
			for(;intpurscreenString[findend]!='#';findend++)
			{

				if(flag==0)
				{
						center_down=center_down*10+intpurscreenString[findend]-48;
						center_down=center_down/10;
				}	
				if(intpurscreenString[findend]=='.')
				{
						flag=0;
				}
				if(flag==1)
				{
					center_down=center_down*10+intpurscreenString[findend]-48;
				}
			}
			//printf("cnter_up=%f",center_up);
			set_center= center_down;
	}
	return set_center;
}

//设置圈数
uint8_t set_cricle()
{
		int findend=2;
		int circle_num;
		for(;intpurscreenString[findend]!='#';findend++)
		{
			circle_num=circle_num*10+intpurscreenString[findend]-48;
			printf("circle_num=%d\n",circle_num);
		}
		printf("circle_num_end=%d",circle_num);
		return circle_num;
		
}

//double change_pwm()
//{
//	int i=2;
//	double a;
//	for(;intpurscreenString[i]=='#';i++)
//	{
//			if(intpurscreenString[i]!='#')
//			{
//			if(intpurscreenString[i]=='.')
//			{
//				a=a*10+intpurscreenString[i];
//				a=a/10;
//			}
//			a=a*10+intpurscreenString[i];
//		}
//	}
//	return a;
//}



//double change_pid()
//{
//	int i=2;
//	double a;
//	for(;intpurscreenString[i]=='#';i++)
//	{
//			if(intpurscreenString[i]!='#')
//			{
//			if(intpurscreenString[i]=='.')
//			{
//				a=a*10+intpurscreenString[i];
//				a=a/10;
//			}
//			a=a*10+intpurscreenString[i];
//		}
//	}
//	return a;
//}

