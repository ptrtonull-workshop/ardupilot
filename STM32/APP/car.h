#ifndef car_h
#define car_h
#include "stm32f10x.h"
int CarConstruct;   //车传过来的值，表示控制命令
/*
	0：无命令
  1：向前
	2：向后
	3：向左
	4：向右
*/
//车传过来的值像水流一样，按一定波特率传输过来的
int GetConstruct(void); //接收来自车的运动信息并且返回
#endif
