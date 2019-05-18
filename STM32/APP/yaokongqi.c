/***************************************************
 *这是遥控器接收程序，包括GPIO初始化，按键检测，按键功能
 *外加一个程序运行显示的LED，LED闪烁证明程序在一直运行
 ****************************************************/

#include "yaokongqi.h"
#include "public.h"
#include "apm_function.h"

int key_status[2]={0};													//2个按键输入状态
int temp=1;

void yaokongqi_init(void)												//遥控器输入IO口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO初始化函数参数结构体																	//初始化系统时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);				//打开GPIOG时钟,复用时钟
	
	/*---------PG9,输入D0----------*/
	//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;					//最大输出频率
	GPIO_InitStructure.GPIO_Pin=D0;										//管脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	/*---------PG10,输入D1---------*/
	GPIO_InitStructure.GPIO_Pin=D1;										//管脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	/*---------PG14，LED2，用作系统正常运行显示----*/
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;						//最大输出频率
	GPIO_InitStructure.GPIO_Pin=LED2;									//管脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;						//推挽输出
	GPIO_Init(GPIOG,&GPIO_InitStructure);		
}

void key_test(void)
{
	int t;
	if(GPIO_ReadInputDataBit(GPIOG, D0) == 1 || GPIO_ReadInputDataBit(GPIOG, D1) == 1)
	{
		delay_ms(5);
		
		/*------keyA------*/
		if(GPIO_ReadInputDataBit(GPIOG, D0) == 1)
			key_status[0]=1;
		else
			key_status[0]=0;
		/*------keyB------*/
		if(GPIO_ReadInputDataBit(GPIOG, D1) == 1)
			key_status[1]=1;
		else
			key_status[1]=0;
	}
	else
	{
		for(t=0;t<2;t++)
		{
			key_status[t]=0;
		}
	}
}

void LED_display(void)
{
	if(temp == 1)
		GPIO_WriteBit(GPIOG, LED2, Bit_SET);
	else
		GPIO_WriteBit(GPIOG, LED2, Bit_RESET);
	temp = -temp;
}

