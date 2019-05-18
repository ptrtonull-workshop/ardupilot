#include "my_include.h"
//定时器3中断服务函数

extern uint8 ball_colcor;
extern uint8 BEEP_ON_OFF;

void TIM3_IRQHandler(void)
{
	static u8 i=0;
	static u8 conut_100ms=0;
  static uint8 change_value=0;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断  5ms中断
	{
		i++;
		conut_100ms++;

		if(conut_100ms>=50)
		{
			LED0=!LED0;
			conut_100ms=0;
			//寻找小球的 色号（0为红，1为绿）
				if(change_value!=ball_colcor)
				{
					USART_SendData(USART1,ball_colcor);
				}
				change_value=ball_colcor;
		}
		if(BEEP_ON_OFF==ON)
		{
			buzzer_ring();
		}
					Car_mode_control();
					PD_control();
					FTM_updata();
		
	
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}



//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	//TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}




