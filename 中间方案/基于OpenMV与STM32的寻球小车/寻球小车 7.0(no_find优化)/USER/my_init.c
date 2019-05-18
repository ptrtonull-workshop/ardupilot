
#include "my_include.h"
#include "delay.h"


extern float C_P; //cameraP
extern float C_D; //cameraD
uint16 integer_read=0;//整数、小数部分（用于flash）
uint16 decimal_read=0;

void init()
{
		SystemInit(); 	 
	  delay_init();
		BEEP_Init();
   	OLED_Init();
	  OLED_Fill(0x00);
	 	LED_Init();		  	//初始化与LED连接的硬件接口

		KEY_Init();
		/******** TIM1产生舵机控制信号 PWM ************/
	 	TIM1_PWM_Init(20000-1,72-1);//分频/  72M/72=1MHz
	 //定时器
	 	TIM3_Int_Init(1000-1,840-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数100次为10ms  
	 	//Tout=((arr+1)*(psc+1))/Ft us.

		uart_init(115200);

		//STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)integer_read,sizeof(integer_read));
		//STMFLASH_Read(FLASH_SAVE_ADDR+2*1024,(u16*)decimal_read,sizeof(decimal_read));
	 // C_P=integer_read+((float)decimal_read*0.1);


}		





