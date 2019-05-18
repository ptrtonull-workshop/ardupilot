#ifndef _public_H
#define _public_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32F10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

void delay_us(u32 i);
void delay_ms(u32 i);
void delay_s(int i);

#endif
