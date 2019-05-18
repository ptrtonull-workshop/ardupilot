#ifndef yaokongqi_h
#define yaokongqi_h
#include "stm32f10x.h"

#define LED2 GPIO_Pin_14	//核心板上的LED D2，连接G14，高电平亮
#define D0 GPIO_Pin_9		//PG9,电平输入信号D0
#define D1 GPIO_Pin_10		//PG10,电平输入信号D1

extern int key_status[2];
void yaokongqi_init(void);
void key_test(void);
void LED_display(void);
	
#endif
