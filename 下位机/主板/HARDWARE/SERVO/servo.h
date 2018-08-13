#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
void uart_init(u32);
void uart2_init(u32);
void SetServoLimit( char id,unsigned short int cw_limit,unsigned short int ccw_limit);
void SetServoVelocity(unsigned char id, signed short int velocity);
void SetServoPosition( char id,unsigned short int position,unsigned short int velocity);	
void read_temp(char id);
//void SetServoLimit1( char id,unsigned short int cw_limit,unsigned short int ccw_limit);
//void SetServoPosition1( char id,unsigned short int position,unsigned short int velocity);	
//void read_temp1(char id);
//void SetServoVelocity1(unsigned char id, signed short int velocity);	 				    
#endif
