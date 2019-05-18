/***************************************************
 *这是控制APM的各种函数
 ****************************************************/
#include "apm_function.h"
#include "public.h"
#include "string.h"
#include "yaokongqi.h"
#include "openmv.h"
#include "chaoshengbo.h"

/*************************************
#define ch1 GPIO_Pin_0//PA0，TIM2通道1
#define ch2 GPIO_Pin_1//PA1，TIM2通道2
#define ch3 GPIO_Pin_2//PA2，TIM2通道3
#define ch4 GPIO_Pin_3//PA3，TIM2通道4
#define ch5 GPIO_Pin_6//PA6，TIM3通道1
**************************************/

/*---------------------------------------------------------------------------------
通道占空比设置，第一个参数为通道CH1~CH5，第二个参数是数值1000~2000对应占空比5%~10%
----------------------------------------------------------------------------------*/
void change_ch_value(char* ch,int x)
{
	if(strcmp(ch,"ch1") == 0)
		TIM_SetCompare1(TIM2,x);
	else if(strcmp(ch,"ch2") == 0)
		TIM_SetCompare2(TIM2,x);
	else if(strcmp(ch,"ch3") == 0)
		TIM_SetCompare3(TIM2,x);
	else if(strcmp(ch,"ch4") == 0)
		TIM_SetCompare4(TIM2,x);
	else if(strcmp(ch,"ch5") == 0)
		TIM_SetCompare1(TIM3,x);
}


void unlock(void)				//解锁飞控
{
	change_ch_value(CH3,1000);	//1000~2000
	change_ch_value(CH4,2000);
	delay_s(5);
	change_ch_value(CH4,1500);
}

void lock(void)					//锁定飞控
{
	change_ch_value(CH3,1000);
	change_ch_value(CH4,1000);
	delay_s(5);
	change_ch_value(CH4,1500);
}

void MOD_ALT_HOLD(void)			//切换到定高模式
{
	change_ch_value(CH5,1500);
	delay_ms(10);
}

void MOD_Stable(void)			//切换到自稳模式（飞控上锁前必须在此模式）
{
	change_ch_value(CH5,1000);
	delay_ms(10);
}


void ALT_HOLD(void)
{
	int overtime=0;
	int fix_ch1 = 1000+1000*0.48;
	int fix_ch2 = 1000+1000*0.48;
	
	unlock();												//解锁
	delay_ms(1000);
	MOD_ALT_HOLD();											//切换到定高模式

	change_ch_value(CH3,1500);
	/**********修正飞机的水平************************/
	change_ch_value(CH2,fix_ch2);							//俯仰方向Y：前小后大
	change_ch_value(CH1,fix_ch1);							//横滚方向X：左小右大
 
	/*********高度保持*******************************/
	while(1)
	{
		key_test();											//按键检测
		openmv_test();										//openmv输出检测
		if(key_status[1] == 1)
			break;
		
		if(get_hight()<60)									//高度小于100cm动作
		{
			change_ch_value(CH3,1000+1000*0.65);			//UP
			if(openmv_status[0] == 1)
				change_ch_value(CH1,fix_ch1-1000*0.2);	//L
			else if(openmv_status[1] == 1)
				change_ch_value(CH1,fix_ch1+1000*0.2);	//R
			if(openmv_status[2] == 1)
				change_ch_value(CH2,fix_ch2+1000*0.2);	//F
			else if(openmv_status[3] == 1)
				change_ch_value(CH2,fix_ch2-1000*0.2);	//B
				
			delay_ms(20);
			change_ch_value(CH3,1500);
			change_ch_value(CH1,fix_ch1);
			change_ch_value(CH2,fix_ch2);
		}
		else if(get_hight()>70)							//高度大于120cm动作
		{
			change_ch_value(CH3,1000+1000*0.45);		//DOWN
			if(openmv_status[0] == 1)
				change_ch_value(CH1,fix_ch1-1000*0.2);	//L
			else if(openmv_status[1] == 1)
				change_ch_value(CH1,fix_ch1+1000*0.2);	//R
			if(openmv_status[2] == 1)
				change_ch_value(CH2,fix_ch2+1000*0.2);	//F
			else if(openmv_status[3] == 1)
				change_ch_value(CH2,fix_ch2-1000*0.2);	//B	
			
			delay_ms(20);
			change_ch_value(CH3,1500);
			change_ch_value(CH1,fix_ch1);
			change_ch_value(CH2,fix_ch2);
		}
		
	}

	/*************降落**********************************/
	while(get_hight() > 20)								//高度大于20cm油门减小降落
	{	
		overtime++;
		change_ch_value(CH3,1000+1000*0.35);			//油门
		if(overtime>1000)
			break;
	}
	
	delay_ms(100);
	change_ch_value(CH3,1000);							//落地后油门最小
	MOD_Stable();										//切换自稳模式
	lock();												//上锁
}	


