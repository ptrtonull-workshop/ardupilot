#ifndef chaoshengbo_h
#define chaoshengbo_h
#include "stm32f10x.h"

#define Trig GPIO_Pin_7		//PB7,Trig
#define Echo GPIO_Pin_6		//PB6,ECHO

void sr04_init(void);
float get_hight(void);

#endif
