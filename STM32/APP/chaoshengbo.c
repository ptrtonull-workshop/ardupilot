/*********************************************************
 *这是超声波测距文件，包括GPIO初始化，定时器初始化，和中断设置
 ********************************************************/
 
#include "chaoshengbo.h"
#include "public.h"

u16 overcount=0;

/*----中断设置----*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructer;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructer.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructer.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructer.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructer.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructer);
}

/*-----端口初始化，定时器初始化-----*/
void sr04_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructer;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	/*TRIG触发信号*/
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructer.GPIO_Pin=Trig;
	GPIO_Init(GPIOB,&GPIO_InitStructer);

	/*ECOH回响信号*/
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructer.GPIO_Pin=Echo;
	GPIO_Init(GPIOB,&GPIO_InitStructer);

	/*定时器TIM4初始化*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseInitStructer.TIM_Period=999;						//定时周期为1000
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; 						//分频系数72
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;		//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructer);

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);						//开启更新中断
	NVIC_Config();
	TIM_Cmd(TIM4,DISABLE);											//关闭定时器使能
}


/*-------超声波测量触发信号-------*/
void trig_signal(void)
{
	GPIO_WriteBit(GPIOB,Trig,Bit_SET);
	delay_us(20);
	GPIO_WriteBit(GPIOB,Trig,Bit_RESET);
}

/*-------测量函数-----------------*/
float get_hight(void)
{
	float length=0,sum=0;
	u16 i=0;
	/*测5次数据计算一次平均值*/
	while(i!=5)
	{
		TIM4->CNT=0;  											//将TIM2计数寄存器的计数值清零
		trig_signal();
		
		while(GPIO_ReadInputDataBit(GPIOB,Echo)==RESET);		//等待回响信号
		TIM_Cmd(TIM4,ENABLE);									//回响信号到来，开启定时器计数
		i+=1; 													//每收到一次回响信号+1，收到5次就计算均值
		while(GPIO_ReadInputDataBit(GPIOB,Echo)==SET);			//回响信号消失
		TIM_Cmd(TIM4,DISABLE);									//关闭定时器

		length=(TIM_GetCounter(TIM4)+overcount*1000)/58.0;		//通过回响信号计算距离,单位cm
		sum=length+sum;
		overcount=0;  											//中断溢出次数清零
		delay_ms(100);
	}
	return length=sum/5;												//距离作为函数返回值
}


/*中断，当回响信号很长时，计数值溢出后重复计数，用中断来保存溢出次数*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)				//是否为定时器4更新中断
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);				//清除中断标志
		overcount++;
	}
}

