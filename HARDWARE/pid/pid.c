#include "pid.h"
#include "math.h"
#include "myuart.h"

PIDTypedef SPEED_PID,ANGEL_PID;

void PID_Init(void)
{
	SPEED_PID.set = 0;
	SPEED_PID.get = 0;
	SPEED_PID.out = 0;
	
	SPEED_PID.Kp = 0;
	SPEED_PID.Kd = 0;
	SPEED_PID.Ki = 0;
	
	SPEED_PID.Ek	= 0;
	SPEED_PID.Ek1 = 0;
	SPEED_PID.Ek_sum = 0;
	
/**********************************************/
/**********************************************/
	ANGEL_PID.set = 0;
	ANGEL_PID.get = 0;
	ANGEL_PID.out = 0;
	
	ANGEL_PID.Kp = 0;
	ANGEL_PID.Kd = 0;
	ANGEL_PID.Ki = 0;
	
	ANGEL_PID.Ek	= 0;
	ANGEL_PID.Ek1 = 0;
	ANGEL_PID.Ek_sum = 0;
}

void Set_SPEED_Kp(float kpp)
{
	SPEED_PID.Kp = kpp;
	printf("SPEED_Kp = %f\n", SPEED_PID.Kp);
}

void Set_SPEED_Kd(float kdd)
{
	SPEED_PID.Kd = kdd;
	printf("SPEED_Kd = %f\n", SPEED_PID.Kd);
}

void Set_SPEED_Ki(float kii)
{
	SPEED_PID.Ki = kii;
	printf("SPEED_Ki = %f\n", SPEED_PID.Ki);
}
/**************************************************************************************/
void Set_ANGEL_Kp(float kpp)
{
	ANGEL_PID.Kp = kpp;
	printf("ANGEL_Kp = %f\n", ANGEL_PID.Kp);
}

void Set_ANGEL_Kd(float kdd)
{
	ANGEL_PID.Kd = kdd;
	printf("ANGEL_Kd = %f\n", ANGEL_PID.Kd);
}

void Set_ANGEL_Ki(float kii)
{
	ANGEL_PID.Ki = kii;
	printf("SPEED_Ki = %f\n", ANGEL_PID.Ki);
}

void SPEED_PID_realize(void)
{	  
	SPEED_PID.Ek = SPEED_PID.set - SPEED_PID.get;	
	SPEED_PID.Ek = SPEED_PID.Ek*0.8f + SPEED_PID.Ek1*0.2f; //低通滤波降低位置对角度的影响
	SPEED_PID.Ek_sum += SPEED_PID.Ek;

	SPEED_PID.out = SPEED_PID.Kp*SPEED_PID.Ek 
					+SPEED_PID.Ki*SPEED_PID.Ek_sum
					+SPEED_PID.Kd*(SPEED_PID.Ek -SPEED_PID.Ek1);
	
	SPEED_PID.Ek1 = SPEED_PID.Ek;	
}

void ANGEL_PID_realize(void)
{	  
	ANGEL_PID.Ek =ANGEL_PID.set  - ANGEL_PID.get;	
	ANGEL_PID.Ek_sum += ANGEL_PID.Ek;

	ANGEL_PID.out = ANGEL_PID.Kp*ANGEL_PID.Ek 
					+ANGEL_PID.Ki*ANGEL_PID.Ek_sum
					+ANGEL_PID.Kd*(ANGEL_PID.Ek -ANGEL_PID.Ek1);
	
	ANGEL_PID.Ek1 = ANGEL_PID.Ek;	
}


