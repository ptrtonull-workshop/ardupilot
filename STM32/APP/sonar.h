#ifndef __SONAR_H__
#define __SONAR_H__
#include "stm32f10x.h"
#include "stdbool.h"

#define	TRIG_PORT      GPIOA		//TRIG,PA4    
#define	TRIG_PIN       GPIO_Pin_4    
#define	ECHO_PORT      GPIOA		//ECHO,PA7  
#define	ECHO_PIN       GPIO_Pin_7   
#define COUNT_TIMER    TIM4			//计数定时器

extern int distance;
void Sonar_Init(void);
void UltrasonicWave_StartMeasure(void);
#endif 
