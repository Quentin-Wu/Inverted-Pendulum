#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "math.h"

#include "led.h"
#include "myuart.h"
#include "delay.h"
#include "usmart.h"
#include "myqei.h"
#include "mypwm.h"
#include "pid.h"
#include "myadc.h"
#include "mytimer.h"
#include "control.h"
#include "screen.h"

extern uint16_t intpurscreenString[80] ;
/*****************************************************************************************************************************************************/
//编码器 PC5 PC6
//马达 pwm PD0
//马达控制端 PB5 PB0
//电位器 PE3

//外设使用： uart0   优先级0
//					定时器3  优先级0x80   用于中断控制切换任务
//					定时器5  优先级0xc0   usmart 用于中断读取串口调试信息 
//          uart2    优先级       uart2  用于传输串口屏数据
//          uart     优先级       uart   用于蓝牙数据交换

//PS: 只需调用切换任务函数 void mode_change(uint8_t modee) 即可切换任务
//    读取time_count ， time_count1的值可显示相应的时间
/***************************************************************************************************************************************************************/
extern PIDTypedef SPEED_PID,ANGEL_PID;
//最低点和最高点
extern float angel_up ;
extern float angel_down ;

//extern float angel_up_test ;
//extern float angel_down_test ;

extern uint16_t mode6_center ;
extern float mode7_center ;
extern uint32_t mode_time ;
extern uint8_t mode_2_count; //任务2 旋转圈数

//时间读取该值即可， 
//该值为任务2转的时间，
//任务6转一圈的时间
extern uint16_t time_count;  //计时变量 单位0.1s

//任务3 任务4保持时间
extern uint16_t time_count1;  //计时变量 单位0.1s

uint8_t mode=0;

//进行任务切换 取值0~7  0对应停止 
void mode_change(uint8_t modee)
{
	PID_Init();
	QEIPositionSet(QEI1_BASE,QEI_Center); //复位一下电机ab相中值
	mode_init(); //任务1，2一堆连七八糟的变量复位
	mode6_center = QEI_Center; //将任务6 的预设位置初始化一下
	mode_time = 0;  //时间变量复位
	mode = modee;  //改变任务标志
	printf("%d",mode);
	LED_GREEN_TUGGEL;
}

//改变任务2 旋转的圈数
void mode2_change(uint8_t mode_2_counttt)
{
	mode_2_count = mode_2_counttt;
}

//加分其他任务，设置位置  mode7_center取值 0~360
void mode7_change(float mode7_centerr)
{
	if(mode7_centerr>180)
	{
		mode7_centerr-=360;
	}
	mode7_center = mode7_centerr;
}
/*****************************************************************************************************************************************************************/


int main(void)
{
	
	int cnt=0;
	//配置系统时钟80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	usmart_dev.init(80);	//80MHz
	
	PID_Init();
	Moto_Init();
	Myqei_Init();   		//测速外设
	My_Adc_Init();			//adc测角度
	M1PWM0_Init(1, 0); 	//1ms 0us  电机
	My_Timer3_Init(5); 	//5ms  定时中断
	Screen_Init();
	while(1)
	{
		
		
		if(mode==7 ||mode==2)
		{
			sand_txt_time(time_count);
		}
		
		
		if(mode==3 || mode==5)
		{
			sand_txt_time(time_count1);
		}
		
				
		sand_waveform(ANGEL_PID.get);
		if(cnt>=10)
			{
				cnt=0;
				show_center(ANGEL_PID.get);

			}
		cnt++;
	
	
		
		if(intpurscreenString[1]=='x')
		{
			show_center_up(angel_up);
			show_center_down(angel_down);
		}
		
		
		delay_ms(10);
		
		
		

//		printf("%d ",intpurscreenString[0]);
//		printf("%d ",intpurscreenString[1]);
//		printf("%d ",intpurscreenString[2]);
//		printf("%d ",intpurscreenString[3]);	
//		printf("%d ",intpurscreenString[4])
//		printf("%d ",intpurscreenString[5]);
//		printf("%d ",intpurscreenString[6]);
//		printf("%d ",intpurscreenString[7]);
//		printf("%d ",intpurscreenString[8]);
//		printf("%d\n ",intpurscreenString[9]);
	}

}




