#include "sonar.h"
#include "public.h"

int distance;
int overcount;
int key = 0 ;
void Sonar_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructer;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
    //PA4TRAG
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;						//PA4接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  	//设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	               	//初始化外设GPIO 

	//PA7ECHO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 		//设为下拉输入
	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				    
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);					//初始化GPIOA7
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);	//绑定中断线PA7
	//中断初始化
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);	
	
	/*定时器TIM4初始化*/
	TIM_DeInit(TIM4);
	TIM_TimeBaseInitStructer.TIM_Period=999;						//定时周期为1000
	TIM_TimeBaseInitStructer.TIM_Prescaler=71; 						//分频系数72
	TIM_TimeBaseInitStructer.TIM_ClockDivision=TIM_CKD_DIV1;		//不分频
	TIM_TimeBaseInitStructer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructer);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);						//开启更新中断
	TIM_Cmd(TIM4,ENABLE);											//打开定时器
		
	//设置ECHO中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  	NVIC_Init(&NVIC_InitStructure);  	
	//设置TIMER更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
}

void UltrasonicWave_StartMeasure(void)
{
	*(__IO uint16_t *) (EXTI_RTSR_TR7) = 1;	//打开EXTI7上升沿触发
	*(__IO uint16_t *) (EXTI_FTSR_TR7) = 0;	//关闭EXTI7下降沿触发
	
	overcount=0;
	key=0;
	GPIO_SetBits(TRIG_PORT,TRIG_PIN);	//送>10US的高电平
	delay_us(10);
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	
	TIM_SetCounter(COUNT_TIMER,0);		//定时器计数置0
}



void EXTI9_5_IRQHandler(void)		//ECHO接收中断程序
{
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{		          
		if(key == 0)
		{
			TIM_Cmd(COUNT_TIMER, ENABLE); 		//打开定时器
			
			*(__IO uint16_t *) (EXTI_RTSR_TR7) = 0;	//关闭EXTI7上升沿触发
			*(__IO uint16_t *) (EXTI_FTSR_TR7) = 1;	//打开EXTI7下降沿触发
			
			distance = TIM_GetCounter(COUNT_TIMER);						
			EXTI_ClearITPendingBit(EXTI_Line7); 
			key = 1;
		}	
		else
		{
			distance = (TIM_GetCounter(TIM4)+overcount*1000)/58.0;
		}
	}

}

void TIM4_IRQHandler(void)   
{
    if (TIM_GetITStatus(COUNT_TIMER, TIM_IT_Update) != RESET)  	//检查TIM4更新中断发生与否
	{
		TIM_ClearITPendingBit(COUNT_TIMER, TIM_IT_Update);  	//清除TIMx更新中断标志 
		GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
		overcount++;
	}
}

