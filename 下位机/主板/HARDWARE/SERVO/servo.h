#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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
