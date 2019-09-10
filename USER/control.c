#include "control.h"
#include "myuart.h"
#include "myqei.h"
#include "mypwm.h"
#include "pid.h"
#include "myadc.h"
#include "kalman.h"

float warebuf[8] = {0};

extern PIDTypedef SPEED_PID,ANGEL_PID;

//最低点和最高点
float angel_up = 280;
float angel_down = 90;

uint8_t position_count = 0; //位置pid 控制变量， position_count次进行一次pid控制
uint32_t mode_time = 0;

uint8_t mode_2_count = 5; //模式2 旋转圈数
uint8_t mode_2_countt = 5;

uint16_t mode6_center = QEI_Center;  //模式6 位置
float mode7_center = 0;  //模式7 位置
float mode7_fen = 0;		//模式7 位置细分

uint16_t time_count=0;  //计时变量 单位0.1s
uint16_t time_countt=0;  //计时变量细分
//稳定时间
uint16_t time_count1=0;  //计时变量 单位0.1s
uint16_t time_countt1=0;  //计时变量细分

float out_speed_max =980; //限幅
int32_t out_pwm_max =980;

//乱七八遭的变量 ，不用管
int32_t mode1_a=0,mode1_b=0,mode1_c=500; //输出pwm, 起摆幅度变量， 摆动幅度
int32_t mode2_a=0,mode2_b=26,mode2_c=3,mode2_d=1,mode2_e=0; //输出pwm，初始给一个反向的冲力, 过程控制变量， 反冲力只执行一次， 判断是否转圈
void mode_init(void)
{
	mode1_a=0;mode1_b=0;mode1_c=500;
	mode2_a=0;mode2_b=26;mode2_c=3;mode2_d=1;mode2_e=0; mode_2_countt= mode_2_count-1;
	time_count =0; time_countt=0;
	time_count1=0; time_countt1=0; 
	position_count = 0;
}

// 摆杆从处于自然下垂状态（摆角 0° ） 开始， 驱动电机带动旋转臂作
// 往复旋转使摆杆摆动， 并尽快使摆角达到或超过-60° ~ +60°
void mode_1(void)
{
	if(mode_time >= 150) {mode_time = 0; mode1_b++;}
	if(mode1_b >=1) {mode1_b=3; mode1_c=270;}
	
	mode1_a = mode1_c* sin(2*3.1415*mode_time/150);

	Motor_Out(mode1_a);
	mode_time++ ;
}

// 从摆杆处于自然下垂状态开始， 尽快增大摆杆的摆动幅度， 直至完成
// 圆周运动；
void mode_2(void)
{
	
	if(mode2_d) //初始给一个反向的冲力 用于快速起摆
	{
		if(--mode2_b) mode2_a=-800;
		else {mode2_d=0; mode2_c=0;}
	}
	
	if(mode_2_countt) //可设置转动的圈数
	{
		if(mode_time >= 150) {mode_time = 0;}
		if(ANGEL_PID.get>270 && ANGEL_PID.get<290) {mode2_c=1; mode2_e=1;} 
		if(mode2_e==1 && ANGEL_PID.get>80 && ANGEL_PID.get<100) {mode2_e=0; mode_2_countt--;}
		
		if (mode2_c==0) 	mode2_a = 500* sin(2*3.1415*mode_time/150) ;
		if (mode2_c==1)
		{
			if ( ANGEL_PID.get>180 && ANGEL_PID.get<360)  mode2_a=-290;
			else if ( ANGEL_PID.get>0 && ANGEL_PID.get<180)  mode2_a=290; 
			else mode2_a=0;
		}
		
		Motor_Out(mode2_a);
		mode_time++ ;
		if(++time_countt >=20) {time_count++; time_countt=0;}
	}
	else
	{
		Motor_Out(0);
	}
}

// 在摆杆处于自然下垂状态下， 外力拉起摆杆至接近 165°位置， 外力
// 撤除同时， 启动控制旋转臂使摆杆保持倒立状态时间不少于 5s； 期间
// 旋转臂的转动角度不大于 90° 。
void mode_3(uint16_t motor_center)
{

	if (fabs(ANGEL_PID.get - angel_up) <80 && fabs(ANGEL_PID.get - angel_up) >=2)
	{
		//自适应pid
//		if (fabs(ANGEL_PID.get - angel_up) >45) {ANGEL_PID.Kp = 30; ANGEL_PID.Kd = 100;}
//		else {ANGEL_PID.Kp = 70; ANGEL_PID.Kd = 100;}
		
		ANGEL_PID.Kp = 366;
		ANGEL_PID.Kd = 100;
		
		ANGEL_PID.get = KalmanFilter(ANGEL_PID.get,1.5,1.5);
		ANGEL_PID.set = angel_up;
		ANGEL_PID_realize();
//		&& fabs(ANGEL_PID.get - angel_up) <30
		if(++position_count >4 )
		{
			SPEED_PID.get = Myqei_Position_Get();
			SPEED_PID.set = motor_center;
			SPEED_PID.Kp = 0;
			SPEED_PID.Kd = -180;
			SPEED_PID_realize();
			position_count = 0;
		}
		Motor_Out(ANGEL_PID.out - SPEED_PID.out);
		if(++time_countt1 >=20) {time_count1++; time_countt1=0;}
	}
	else
	{
		Motor_Out(0);
	}
}

// 从摆杆处于自然下垂状态开始， 控制旋转臂作往复旋转运动， 尽快使
// 摆杆摆起倒立， 保持倒立状态时间不少于 10s；
//if(mode2_time >= 150) {mode2_time = 0;}
//a = 500* sin(2*3.1415*mode2_time/150) ;
void mode_4(void)
{	
	if ( mode_time < 35)
	{
		mode_time++;
		Motor_Out(-980);
	}
	else if ( mode_time < 50 )
	{
		mode_time++;
		Motor_Out(980);
	}
	else if (  mode_time >= 50 &&  mode_time < 80 )
	{
		mode_time = 90;
		Motor_Out(0);
		QEIPositionSet(QEI1_BASE,QEI_Center);
	}

	if ( mode_time == 90 )
	{
		mode_3(QEI_Center);
	}
}

// 在摆杆保持倒立状态下，施加干扰后摆杆能继续保持倒立或 2s 内恢复
// 倒立状态；
void mode_5(void)
{
	mode_3(QEI_Center);
}

// 在摆杆保持倒立状态的前提下， 旋转臂作圆周运动， 并尽快使单方向
// 转过角度达到或超过 360° ；
void mode_6(void)
{
	mode6_center +=4; 
	if (mode6_center >= QEI_Center+2000) mode6_center=QEI_Center +2000;
	else if(++time_countt >=20) {time_count++; time_countt=0;}
	mode_3(mode6_center);
}

// 扩展
// 可调节转轴的位置-180~180
void mode_7(void)
{
	float mode7_center1;
	mode7_center1 = 4.33f*mode7_center;
	//逐渐完目标值靠近
	if(mode7_center1 >mode7_fen) mode7_fen++;
	if(mode7_center1 <mode7_fen) mode7_fen--;
	
	mode_3(QEI_Center + mode7_fen);
}


void Moto_Init(void)
{
	//M1_IN1,2   
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_M1_1);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_M1_1, GPIO_PIN_M1_1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIO_M1_2);
	GPIOPinTypeGPIOOutput(GPIO_PORT_BASE_M1_2, GPIO_PIN_M1_2);
	M1_STOP();
}


void M1_Forward(int32_t width)
{
	GPIOPinWrite(GPIO_PORT_BASE_M1_1, GPIO_PIN_M1_1, 0);
	GPIOPinWrite(GPIO_PORT_BASE_M1_2, GPIO_PIN_M1_2, GPIO_PIN_M1_2);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1.25*width - 1);
}

void M1_Backward(int32_t width)
{
	GPIOPinWrite(GPIO_PORT_BASE_M1_1, GPIO_PIN_M1_1, GPIO_PIN_M1_1);
	GPIOPinWrite(GPIO_PORT_BASE_M1_2, GPIO_PIN_M1_2, 0);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1.25*width - 1);
}

void Motor_Out(int32_t out_x)
{
	if(out_x >  out_pwm_max)  out_x =  out_pwm_max;
	if(out_x < -out_pwm_max)  out_x = -out_pwm_max;
	
	if(out_x >= 0)  M1_Forward(out_x);
	if(out_x <  0)  M1_Backward(fabs(out_x));  
	
	warebuf[0] = ANGEL_PID.get;
	warebuf[1] = ANGEL_PID.out;
	warebuf[2] = SPEED_PID.get;
	warebuf[3] = SPEED_PID.out;
	warebuf[4] = out_x;
	warebuf[5] = ANGEL_PID.set;
	vcan_sendware((uint8_t *)warebuf,sizeof(warebuf)); 
}






