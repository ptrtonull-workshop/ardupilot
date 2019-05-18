/***************************************************
 *这是5路PWM输出文件，包括GPIO初始化，定时器初始化
 ****************************************************/

#include "channel.h"
#include "public.h"
#include "apm_function.h"

void CH_IO_INIT()																			//通道输出IO口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;													//GPIO初始化函数参数结构体
	//SystemInit();																			//初始化系统时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);				//打开GPIOA时钟,复用时钟
	
	/*---------TIM2通道1，2，3，4对应PA0,PA1,PA2,PA3对应ch1,ch2,ch3,ch4----------*/
	/*---------TIM3通道1对应PA6对应ch5----------*/
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;											//最大输出频率
	GPIO_InitStructure.GPIO_Pin=ch1|ch2|ch3|ch4|ch5;										//管脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;											//复用推挽输出模式
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//初始化				
}

void CH_TIM_INIT()																			//通道定时器初始化
{
	TIM_TimeBaseInitTypeDef TIM_TimebaseInitStructure;										//定时器初始化参数结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;													//PWM_OUT初始化参数结构体
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);					//打开TIM2,3时钟
	/*----------------------TIM2,3基本设置-----------------------------*/
	TIM_TimebaseInitStructure.TIM_Period=20000-1;											//PWM频率1M/20000=50Hz
	TIM_TimebaseInitStructure.TIM_Prescaler=72-1;											//预分频72-1，时钟为72M/72=1M
	TIM_TimebaseInitStructure.TIM_ClockDivision=0x0;										//时钟分割
	TIM_TimebaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;							//向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimebaseInitStructure);										//定时器初始化
	TIM_TimeBaseInit(TIM3,&TIM_TimebaseInitStructure);	
	/*---------------------TIM2,3PWM输出设置---------------------------*/
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;											//PWM1模式
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;								//开启OC输出
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;									//输出极性
	TIM_OCInitStructure.TIM_Pulse=1500;														//跳变值，即：占空比=TIM_Pulse/TIM_Period
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);													//定时器2通道1初始化
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);													//定时器2通道2初始化
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);													//定时器2通道3初始化
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);													//定时器2通道4初始化
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);										//使能TIM2在CCR1上的预装在寄存器
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);										//使能TIM2在CCR1上的预装在寄存器
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);										//使能TIM2在CCR2上的预装在寄存器
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);										//使能TIM2在CCR3上的预装在寄存器
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);										//使能TIM2在CCR4上的预装在寄存器
	TIM_Cmd(TIM2,ENABLE);																	//定时器使能
	TIM_Cmd(TIM3,ENABLE);	
	//TIM_SetCompare3(TIM2,1500);															//设置跳变值1000~2000,改变占空比5%~10%
}

void channel_init(void)
{
	/*---初始化IO口，定时器以及通道输出PWM*/
	CH_IO_INIT();
	CH_TIM_INIT();
	change_ch_value(CH1,1500);
	change_ch_value(CH2,1500);
	change_ch_value(CH3,1000);
	change_ch_value(CH4,1500);
	change_ch_value(CH5,1000);
}


