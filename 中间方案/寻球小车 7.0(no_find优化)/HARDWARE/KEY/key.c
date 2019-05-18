#include "key.h"
#include "delay.h"
#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
extern u8 mode_flag;
extern u8 run_flag;
extern void	SBuff_Send(void);	
//按键初始化函数 
//PA15和PC5 设置成输入
void KEY_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA15

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTC时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;//PA15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA15
	

	
	
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!


u8 Key_scan(void)
{	 

		if(KeyUp==0){while (!KeyUp) ;return 1;}

		else if(KeyDn==0){while (!KeyDn) ;return 2;}
		else if(KeyLt==0){while (!KeyLt) ;return 3;}
		else if(KeyRt==0){while (!KeyRt) ;return 4;}
		else if(KeyMd==0){while (!KeyMd) ;return 5;}
    else return 0;// 无按键按下

}



void KYE3_SHOW()
{
	if(KeyMd==0)
	{ 
		delay_ms(10);
		if(KeyMd==0)
		{ 
		  LED1=~LED1; 

		}  
	}
}
