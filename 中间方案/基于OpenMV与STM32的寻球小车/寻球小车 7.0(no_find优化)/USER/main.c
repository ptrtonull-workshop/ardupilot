//测试
//2018.8.27
//舵机边值  1300
//舵机中值  1450
//舵机边值  1600

#include "sys.h"
#include "my_include.h"

int main(void)
{
		init();			//系统初始化
		Init_Show();//初始化界面
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		TIM_Cmd(TIM3,ENABLE); //使能定时器3
		
	  while(1){  		
			Boma_OLED();	
			KYE3_SHOW();	
		} 
 }
