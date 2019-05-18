#ifndef channel_h
#define channel_h
#include "stm32f10x.h"

#define ch1 GPIO_Pin_0//PA0，TIM2通道1
#define ch2 GPIO_Pin_1//PA1，TIM2通道2
#define ch3 GPIO_Pin_2//PA2，TIM2通道3
#define ch4 GPIO_Pin_3//PA3，TIM2通道4
#define ch5 GPIO_Pin_6//PA6，TIM3通道1

void channel_init(void);

#endif
