#include "my_include.h"

extern CAR_STATUS_e car_mode;
extern moty_duty run_duty;
extern u8 control_data[MAX_DATA_LENS];
extern float C_P; //cameraP
extern float C_D; //cameraD
extern int16 ser_duty;
extern int16 x_error;
extern int16 last_x_error; 
extern uint8 ball_colcor;
extern uint8 BEEP_ON_OFF;

extern uint8 out_edge;//出界

void Car_mode_control()
{

		if(control_data[3]<50 && control_data[3]!=0) //左出界
		{
			out_edge=Left;
		}
		else if(control_data[3]>110 && control_data[3]!=0) //右出界
		{
			out_edge=Right;
		}

		
		//--------------------车位状态判断-----------------//
		if(out_edge==Left && control_data[4]==0 && control_data[5]==0)
		{
			car_mode=finding_L;
		}
		else if(out_edge==Right && control_data[4]==0 && control_data[5]==0)
		{
			car_mode=finding_R;
		}
		else if (control_data[3]>0 && control_data[4]>0 && control_data[5]>0)
		{
			car_mode=run;
		}
		if(control_data[5]<=12 && control_data[5]>=3)
		{
				LED1=0;
				car_mode=stop;
		}
		else {LED1=1;}
		
		if(Boma4==0)//强制菜单
		{
			car_mode=stop;
		}
		//寻找小球的 色号（0为红，1为绿）	

		if(Boma3==0)
		{
			BEEP_ON_OFF=OFF;
		}
		else {BEEP_ON_OFF=ON;}
}
void FTM_updata()
{
		if(car_mode == run)
		{
				TIM_SetCompare1(TIM1,run_duty.Speed_Duty_R);	//右为  TIM1  CH1
				TIM_SetCompare4(TIM1,run_duty.Speed_Duty_L);  //左为  TIM1  CH4
		}
		else if(car_mode == finding_R)
		{
				TIM_SetCompare1(TIM1,1400);	//右为  TIM1  CH1
				TIM_SetCompare4(TIM1,1400); //左为  TIM1  CH4
		}
		else if(car_mode == finding_L)
		{
				TIM_SetCompare1(TIM1,1500);	//右为  TIM1  CH1
				TIM_SetCompare4(TIM1,1500); //左为  TIM1  CH4
		}
	  else if(car_mode == stop)
		{
				TIM_SetCompare1(TIM1,0);	//右为  TIM1  CH1
				TIM_SetCompare4(TIM1,0);  //左为  TIM1  CH4
		}
}

void PD_control()
{
	last_x_error=x_error;
	x_error=control_data[3]-80;
	ser_duty = C_P*x_error-C_D*(last_x_error-x_error);

	run_duty.Speed_Duty_R=1550-ser_duty;//正为正转
	run_duty.Speed_Duty_L=1350-ser_duty;
	//左边FTM波//限幅	
	run_duty.Speed_Duty_L=run_duty.Speed_Duty_L<1300?1300:run_duty.Speed_Duty_L;
	run_duty.Speed_Duty_L=run_duty.Speed_Duty_L>1600?1600:run_duty.Speed_Duty_L;
	//右边FTM波//限幅	
  run_duty.Speed_Duty_R=run_duty.Speed_Duty_L<1300?1300:run_duty.Speed_Duty_R;
	run_duty.Speed_Duty_R=run_duty.Speed_Duty_L>1600?1600:run_duty.Speed_Duty_R;
}


