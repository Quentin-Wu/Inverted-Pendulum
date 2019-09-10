#include "usmart.h"
#include "usmart_str.h"

#include "led.h"
#include "delay.h"
#include "mypwm.h"

extern void mode_change(uint8_t modee);
extern void mode7_change(float mode7_centerr);
extern void mode2_change(uint8_t mode_2_counttt);

//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 xms)",
 	(void*)delay_us,"void delay_us(u32 xus)",	
	(void*)LED_RED_DEBUG, "void LED_RED_DEBUG(bool a)",
	(void*)LED_BLUE_DEBUG, "void LED_BLUE_DEBUG(bool a)",
	(void*)LED_GREEN_DEBUG, "void LED_GREEN_DEBUG(bool a)",
	(void*)mode_change, "void mode_change(uint8_t modee)",
	(void*)mode7_change, "void mode7_change(float mode7_centerr)",
	(void*)mode2_change, "void mode2_change(uint8_t mode_2_counttt)",
};	




//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};


