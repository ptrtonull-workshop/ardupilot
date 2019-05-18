/***************************************************
 *这是openmv接收程序，初始化，功能
 ****************************************************/

#include "openmv.h"
#include "public.h"
#include "apm_function.h"

u8 openmv_status[4]={0};													

void openmv_init(void)												
{
	GPIO_InitTypeDef GPIO_InitStructure;								//GPIO初始化函数参数结构体																	//初始化系统时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);				//打开GPIOE时钟
	
	//======GPIOE8，9，10，11下拉输入======//
	GPIO_InitStructure.GPIO_Pin=IN1|IN2|IN3|IN4;						//管脚
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;							//下拉输入
	GPIO_Init(IN_PORT,&GPIO_InitStructure);
}

void openmv_test(void)
{
	int t;
	if(GPIO_ReadInputDataBit(IN_PORT, IN1) == 1 || GPIO_ReadInputDataBit(IN_PORT, IN2) == 1 ||
	   GPIO_ReadInputDataBit(IN_PORT, IN3) == 1 || GPIO_ReadInputDataBit(IN_PORT, IN4) == 1)
	{
		delay_ms(5);
		if(GPIO_ReadInputDataBit(IN_PORT, IN1) == 1)
			openmv_status[0]=1;
		else
			openmv_status[0]=0;
		if(GPIO_ReadInputDataBit(IN_PORT, IN2) == 1)
			openmv_status[1]=1;
		else
			openmv_status[1]=0;
		if(GPIO_ReadInputDataBit(IN_PORT, IN3) == 1)
			openmv_status[2]=1;
		else
			openmv_status[2]=0;
		if(GPIO_ReadInputDataBit(IN_PORT, IN4) == 1)
			openmv_status[3]=1;
		else
			openmv_status[3]=0;
	}
	else
	{
		for(t=0;t<4;t++)
		{
			openmv_status[t]=0;
		}
	}
}



