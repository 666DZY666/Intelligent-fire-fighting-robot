#include "sys.h"
#include "delay.h"	
#include "timer.h" 
void move(int l_speed,int r_speed)           //l_speed,r_speed的范围为-300 ~ +300
{                                            //当速度达到200~300时基本没有太大的变化
    if(l_speed>=0&&r_speed>=0)
	{     
		v4=1;
		v3=0;
		v2=1;
		v1=0;
	}
    else if(l_speed>0&&r_speed<0)
	{     
		v4=1;
		v3=0;
		v2=0;
		v1=1;
		r_speed=0-r_speed;
	}
	else if(l_speed<0&&r_speed>0)   
	{     
		v4=0;
		v3=1;
		v2=1;
		v1=0;
		l_speed=0-l_speed;
	}
	else
	{
		v4=0;
		v3=1;
		v2=0;
		v1=1;
		r_speed=0-r_speed;l_speed=0-l_speed;
	}
	TIM_SetCompare3(TIM4,l_speed);			//PB8
	TIM_SetCompare4(TIM4,r_speed);			//PB9
}

void fire(int speed)  
{
	if(speed>=0)
	{     
		v5=0;
		v6=1; 
	}
    else
	{     
		v5=1;
		v6=0;
		speed=0-speed;
	}
	TIM_SetCompare1(TIM1,speed);			//PA8
}
