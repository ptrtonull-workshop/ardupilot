#ifndef _CONTROL_H
#define _CONTROL_H


typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */

typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */
typedef long  long          int64;  /* 64 bits */


#define M_I 6
#define M_P 40
#define Servo_Duty_Lf 1600
#define Servo_Duty_Md 1450 
#define Servo_Duty_Rt 1300


void FTM_updata(void);

void Car_mode_control(void);
void PD_control(void);

#endif








