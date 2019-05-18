#include "public.h"

/***************************
*功能：延时us函数
*要求：i<1864*1000
****************************/
void delay_us(u32 i)
{
	u32 temp;	
	SysTick->LOAD=9*i;		 
	SysTick->CTRL=0X01;		 
	SysTick->VAL=0;		   	 
	do
	{
		temp=SysTick->CTRL;		   
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	 
	SysTick->CTRL=0;	
	SysTick->VAL=0;	
}

/***************************
*功能：延时ms函数
*要求：i<1864
***************************/
void delay_ms(u32 i)
{
	u32 temp;
	SysTick->LOAD=9000*i;	 
	SysTick->CTRL=0X01;		
	SysTick->VAL=0;			
	do
	{
		temp=SysTick->CTRL;	   
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	
	SysTick->CTRL=0;	
	SysTick->VAL=0;		
}

/*----延时s函数----*/
void delay_s(int i)
{
	int x;
	for(x=0;x<i;x++)
		delay_ms(1000);
}
