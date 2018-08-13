#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#define a Get_Adc_Average(1,10)
#define b Get_Adc_Average(0,10)
#define c Get_Adc_Average(2,10)

void Adc_Init(void);
u16 Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times);
#endif

