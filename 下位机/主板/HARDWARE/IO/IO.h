#ifndef __IO_H
#define __IO_H
#include "sys.h"

#define v GPIO_ReadInputData(GPIOA) 

#define f1 PAin(12)
#define f2 PCin(3)
#define f3 PAin(15)
#define f4 PBin(12)
#define f5 PBin(11)
#define f6 PBin(10)
#define f7 PCin(5)
#define f8 PCin(4)
#define f9 PCin(2)

void IO_Init(void);
#endif
