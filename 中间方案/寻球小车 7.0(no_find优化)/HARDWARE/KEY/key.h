#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#include "my_include.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 



#define KeyDn    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键0
#define KeyLt    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键1
#define KeyMd    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取按键0
#define KeyRt    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键1
#define KeyUp    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键0


#define Boma1    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读取按键0
#define Boma2    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)//读取按键1
#define Boma3    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//读取按键0
#define Boma4    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//读取按键1


#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键2 
 

#define KEY0_PRES	1		//KEY0  
#define KEY1_PRES	2		//KEY1 
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);//IO初始化
uint8 Key_scan(void);  	//按键扫描函数		
void KYE3_SHOW(void);

#endif
