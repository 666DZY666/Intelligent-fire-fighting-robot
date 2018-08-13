#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"	  

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

#define v1 PBout(0)
#define v2 PBout(1)
#define v3 PCout(8)
#define v4 PCout(9)

#define v5 PBout(2)
#define v6 PBout(3)

#endif

