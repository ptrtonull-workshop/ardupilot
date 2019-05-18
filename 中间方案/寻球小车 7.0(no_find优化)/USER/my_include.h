#ifndef _MY_INCLUDE_H
#define _MY_INCLUDE_H

#include "my_init.h"
#include "oled.h"
#include "timer.h"
#include "pwm.h"
#include "led.h"
#include "control.h"
#include "debug.h"
#include "beep.h"
#include "key.h"


#include "stmflash.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"	
// * 数据类型声明
// 
#define FLASH_SAVE_ADDR  0x08011800  	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define ON    1
#define OFF   0
#define Left  0//0左1右
#define Right 1

typedef struct D  //结构体
{
	int16 Speed_Duty_L;
	int16 Speed_Duty_R;
}moty_duty;


typedef enum   //枚举
{
	finding_R=4,
	finding_L=3,
	run=2,
	stop=1,
	error=0,
}
CAR_STATUS_e;  //车子状态 

typedef enum
{
	mode_ON_OFF=0,
	car_run=1,
	flash=2,   
	picture=3,

}
MENU_LIST_e; //菜单


int16 my_abs(int16 number);
void buzzer_once(void);
void buzzer_bibi(uint8 t,uint8 l);
void buzzer_ring(void);

#endif

