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
//������ PC5 PC6
//��� pwm PD0
//�����ƶ� PB5 PB0
//��λ�� PE3

//����ʹ�ã� uart0   ���ȼ�0
//					��ʱ��3  ���ȼ�0x80   �����жϿ����л�����
//					��ʱ��5  ���ȼ�0xc0   usmart �����ж϶�ȡ���ڵ�����Ϣ 
//          uart2    ���ȼ�       uart2  ���ڴ��䴮��������
//          uart     ���ȼ�       uart   �����������ݽ���

//PS: ֻ������л������� void mode_change(uint8_t modee) �����л�����
//    ��ȡtime_count �� time_count1��ֵ����ʾ��Ӧ��ʱ��
/***************************************************************************************************************************************************************/
extern PIDTypedef SPEED_PID,ANGEL_PID;
//��͵����ߵ�
extern float angel_up ;
extern float angel_down ;

//extern float angel_up_test ;
//extern float angel_down_test ;

extern uint16_t mode6_center ;
extern float mode7_center ;
extern uint32_t mode_time ;
extern uint8_t mode_2_count; //����2 ��תȦ��

//ʱ���ȡ��ֵ���ɣ� 
//��ֵΪ����2ת��ʱ�䣬
//����6תһȦ��ʱ��
extern uint16_t time_count;  //��ʱ���� ��λ0.1s

//����3 ����4����ʱ��
extern uint16_t time_count1;  //��ʱ���� ��λ0.1s

uint8_t mode=0;

//���������л� ȡֵ0~7  0��Ӧֹͣ 
void mode_change(uint8_t modee)
{
	PID_Init();
	QEIPositionSet(QEI1_BASE,QEI_Center); //��λһ�µ��ab����ֵ
	mode_init(); //����1��2һ�����߰���ı�����λ
	mode6_center = QEI_Center; //������6 ��Ԥ��λ�ó�ʼ��һ��
	mode_time = 0;  //ʱ�������λ
	mode = modee;  //�ı������־
	printf("%d",mode);
	LED_GREEN_TUGGEL;
}

//�ı�����2 ��ת��Ȧ��
void mode2_change(uint8_t mode_2_counttt)
{
	mode_2_count = mode_2_counttt;
}

//�ӷ�������������λ��  mode7_centerȡֵ 0~360
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
	//����ϵͳʱ��80Mhz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	LED_Init();
	My_Uart_Init();
	usmart_dev.init(80);	//80MHz
	
	PID_Init();
	Moto_Init();
	Myqei_Init();   		//��������
	My_Adc_Init();			//adc��Ƕ�
	M1PWM0_Init(1, 0); 	//1ms 0us  ���
	My_Timer3_Init(5); 	//5ms  ��ʱ�ж�
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




