#include "my_include.h"
#include "oled.h"

uint8 SIZE1=0;	 	//数组长度
uint8 SIZE2=0;	 	//数组长度

extern unsigned char JMU_LOGO[];
extern unsigned char ETA_LOGO1[];
extern unsigned char ETA_LOGO2[];
extern unsigned char F8X16[];
extern int16 ser_duty;
extern MENU_LIST_e MENU;
extern CAR_STATUS_e car_mode;
extern moty_duty run_duty;
extern uint8 row;  // LCD 行
extern uint8 unit; //每次加减的权重
extern float C_P;  //cameraP
extern float C_D;  //cameraD
extern uint8 control_data[];
extern uint8 ball_colcor;
extern uint16 integer;//整数、小数部分（用于flash）
extern uint16 decimal;
extern uint8 BEEP_ON_OFF; //蜂鸣器开关

void Flash_Key(void);
void Mode_ON_OFF_Key(void);
void flash_OLED(void);
void flash_updata(void);
void car_run_OLED(void);
void Picture_OLED(void);
void Mode_ON_OFF(void);

void Init_Show()
{
		OLED_ShowCHinese1(30,2,20);//集美大学
		OLED_ShowCHinese1(50,2,21);
		OLED_ShowCHinese1(70,2,22);//
		OLED_ShowCHinese1(90,2,23);

		delay_ms(150);
		buzzer_bibi(3,1);
		delay_ms(500);
		delay_ms(500);
  	OLED_Clear(); 
		OLED_DrawBMP(0,0,110,7,ETA_LOGO1);
		delay_ms(500);
		//OLED_DrawBMP(0,0,110,7,ETA_LOGO2);
	  OLED_Clear(); 
}


void menu_define(void) //菜单定义
{
	uint8 mode=0;
	static uint8 change_value=0;

	mode=Boma1*1+Boma2*2;//+Boma2*2;
	if(change_value!=mode)
	{
		OLED_Clear();
	}
	change_value=mode;

	switch(mode)
	{
		case 0:
		{
			MENU=picture;
			break;
		}
		case 1:
		{
			MENU=car_run;
			break;
		}
		case 2:
		{
			MENU=mode_ON_OFF;
			break;
		}
		case 3:
		{
			MENU=flash;
			break;
		}
	}
}

void Boma_OLED() // 读取拨码并显示在OLED上
{
	menu_define();
	if(MENU==picture)
	{
		Picture_OLED();
		Mode_ON_OFF_Key();
	}
	if(MENU==car_run)   
	{
		car_run_OLED();
		Mode_ON_OFF_Key();
		return;
	}
	if(MENU==flash)
	{
		flash_OLED();
		Flash_Key();
		return;
		
	}
	if(MENU==mode_ON_OFF)
	{
		Mode_ON_OFF();
    Mode_ON_OFF_Key();
		return;
		
	}
}


void car_run_OLED()
{
	
		OLED_ShowCHinese1(0,0,26+ball_colcor);
		OLED_ShowCHinese1(30,0,24);
		OLED_ShowCHinese1(50,0,25);
		OLED_ShowString1(70,0,":");	
		Dis_Num(0,42,control_data[5],2);//显示当前距离cm
		OLED_ShowString1(100,0,"cm");
	
		OLED_P6x8Str(0,7,"(   ,   )");	
		OLED_P6x8Num_8bit(7,7,(uint8)(control_data[3]*100/130));//显示当前x
		OLED_P6x8Num_8bit(30,7,(uint8)(control_data[4]/4));//显示当前y
	
		OLED_ShowString1(0,2,"ser_duty");
		if(ser_duty>=0)
		{
			OLED_ShowString1(80,2,"+");
		}
		else
		{
			OLED_ShowString1(80,2,"-");
		}

		Dis_Num(2,45,my_abs(ser_duty),3);  
		
		Dis_Num(4,0,run_duty.Speed_Duty_R,4);  
		Dis_Num(4,44,run_duty.Speed_Duty_L,4);  
		if(car_mode==run) 
		{
			OLED_ShowString1(70,6,"run    "); 

		}
		else if(car_mode==stop) 
		{             
			OLED_ShowString1(70,6,"stop   ");

		}
		else if(car_mode==finding_L) 
		{             
			OLED_ShowString1(70,6,"L_find  ");
		}
		else if(car_mode==finding_R) 
		{             
			OLED_ShowString1(70,6,"R_find  ");
		}
}


void flash_OLED()
{
		OLED_P6x8Str(0,0,"row:");	

		OLED_P6x8Num_8bit(30,0,row);//显示当前距离cm
		OLED_P6x8Str(80,0,"unit:");	
		OLED_P6x8Num_8bit(110,0,unit);//显示当前距离cm


		OLED_ShowString1(0,2,"C_P");
		OLED_ShowString1(0,4,"C_D");	
		Dis_Float(2,50,C_P,1);		
		Dis_Float(4,50,C_D,1);			
	
}



void Picture_OLED()
{
	if(control_data[3]==0 && control_data[4]==0 && control_data[5]==0)
	{
  	OLED_ShowString1(40,4 ,"No_Find");	
	}
	else 
	{
		OLED_Fill(0x00);
		OLED_ShowString1((uint8)(control_data[3]*100/130),(uint8)(control_data[4]*10/18/8/2) ,"x");	
	}

}

void Mode_ON_OFF()
{
	OLED_ShowCHinese1(0,0,28);
	OLED_ShowCHinese1(16,0,29);
	OLED_ShowCHinese1(32,0,30);
	OLED_ShowCHinese1(48,0,31);
	OLED_ShowString1(70,0,":");
	OLED_ShowCHinese1(90,0,26+ball_colcor);	
	if(BEEP_ON_OFF==1)
	{
		OLED_ShowString1(60,6 ,"BEEP_ON ");	
	}
	else if(BEEP_ON_OFF==0)
	{
		OLED_ShowString1(60,6 ,"BEEP_OFF");	
	}

}


void Flash_Key()
{
	switch(Key_scan())
	{
		case 1: //上
		{
					if(row==1)
					{
						switch(unit)
						{
							case 1:{C_P+=0.10;break;}
							case 2:{C_P+=0.50;break;}	
							case 3:{C_P+=1.00;break;}	
						}
					}
					if(row==2)
					{
						switch(unit)
						{
							case 1:{C_D+=0.10;break;}
							case 2:{C_D+=0.50;break;}	
							case 3:{C_D+=1.00;break;}	
					 }
					}
				//	flash_updata();
					break;
		}
		case 2:
		{
					if(row==1)
					{
						switch(unit)
						{
							case 1:{C_P-=0.10;break;}
							case 2:{C_P-=0.50;break;}	
							case 3:{C_P-=1.00;break;}	
						}
					}
					if(row==2)
					{
						switch(unit)
						{
							case 1:{C_D-=0.10;break;}
							case 2:{C_D-=0.50;break;}	
							case 3:{C_D-=1.00;break;}	
						}
					}
				//	flash_updata();
					break;
		}
		case 3:  //换行
		{
					row+=1;
					if(row>2){row=1;}
					OLED_Clear(); 
					break;
		}
		case 4:   //换行
		{
					row-=1;
					if(row<1){row=2;}
					OLED_Clear();
					break;
		}
		case 5:  //切换权重
		{
			unit+=1;
			OLED_Clear(); 
			if(unit>3) {unit=1;}
			break;
		}
	}
}


void Mode_ON_OFF_Key()
{
	switch(Key_scan())
	{
		case 5:
		{
			ball_colcor++;
			if(ball_colcor>1)ball_colcor=0;

		}
	}
	
}
void flash_updata()
{

		SIZE1=sizeof(integer);
		SIZE2=sizeof(decimal);
		integer=((uint16)(C_P*10))/10;//整数部分
		decimal=((uint16)(C_P*10))%10;//小数部分
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)integer,sizeof(integer));
		STMFLASH_Write(FLASH_SAVE_ADDR+2*1024,(u16*)decimal,sizeof(decimal));
}

/* int unitPlace = n / 1 % 10;
    int tenPlace = n / 10 % 10;
    int hundredPlace = n / 100 % 10;
    int thousandPlace = n / 1000 % 10;*/

