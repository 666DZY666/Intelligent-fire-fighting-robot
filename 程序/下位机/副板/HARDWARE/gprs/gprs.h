#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"	  
void GSM_Init(void);
void GSM_Send(u8 *p1,u8 *p2);
char hand(char *ptr);
void clear_rec_data(void);
#endif
