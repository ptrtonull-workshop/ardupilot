#include "my_include.h"

volatile CAR_STATUS_e car_mode=stop;  //车子状态  
volatile MENU_LIST_e MENU =picture; //菜单
//volatile是一种类型修饰符。volatile 的作用 是作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接读值。

moty_duty run_duty={0};//两轮ftm输出速度
float C_P=4; //cameraP
float C_D=5; //cameraD

int16 ser_duty=0;
int16 x_error=0;
int16 last_x_error=0; 
uint8 row=1;  // LCD 行
uint8 unit=1; //每次加减的权重
uint8 ball_colcor=0x01;
uint16 integer=0,decimal=0;//整数、小数部分（用于flash）
uint8 BEEP_ON_OFF=0;
uint8 buzzer_time=0;//蜂鸣器响时间
uint8 buzzer_count=0;//蜂鸣器计数
uint8 buzzer_number=0;//蜂鸣器计数
uint8 lase_x=0;  //上一次的横坐标
uint8 out_edge=0;//出界


int16 my_abs(int16 number)
{
	return (number<0?-number:number);
}

void buzzer_once()
{
	buzzer_time=5;
	buzzer_number=5;
	buzzer_count=1;

}  
void buzzer_bibi(uint8 t,uint8 l)
{
	buzzer_time=l*5;
	buzzer_number=l*5;
	buzzer_count=t*2;
}  
void buzzer_ring()
{
	if(buzzer_count>=1)
	{  
		if(buzzer_number>=1)
		{	
          if(buzzer_count%2==1)   {BEEP=1;}
          else                    {BEEP=0;}
          buzzer_number--;	
		}
		else
		{
          buzzer_number=buzzer_time;
          buzzer_count--;
		}
	}
	else
	{	
		BEEP=0;
		buzzer_time=0;
		buzzer_count=0;
	}
}
