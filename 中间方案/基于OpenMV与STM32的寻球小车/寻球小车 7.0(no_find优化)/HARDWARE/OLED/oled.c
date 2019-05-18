/*********************************************************************************
// * 文件名  ：oled.c
// * 描述    ：oled模块配置         
// * 实验平台：HT-Hawk开源飞控
// * 作者 ：张建胜
// * 功能    ：OLED显示页面设置        
// * 实验平台：STM32F103ZET6
// * 实验日期：2016年7月23日
**********************************************************************************/
#include "my_include.h"
#include "my_oledfont.h"

u16 tempI1,tempI2,tempI3,tempI4,tempV1,tempV2,tempV3,tempV4;

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 

#define X_WIDTH 128
#define Y_WIDTH 64
//======================================

/**************************实现函数********************************************
*函数原型:    void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
*功　　能:    OLED写入数据/命令
输入参数：    dat  要输入的数据
              cmd  1:命令  0：数据
输出参数：    无
*******************************************************************************/
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{	
    uint8_t i;			  
    if(cmd)   OLED_RS_Set();
    else      OLED_RS_Clr();		  
    for(i=0;i<8;i++)
    {			  
	    OLED_SCLK_Clr();
	    if(dat&0x80)   OLED_SDIN_Set();
	    else           OLED_SDIN_Clr();
	    OLED_SCLK_Set();
	    dat<<=1;   
    }				 		 
    OLED_RS_Set();   	  
} 

void OLED_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
}       

void OLED_Init(void)        
{
  GPIO_InitTypeDef OLED_GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);					//关闭JTAG功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);			//关闭JTAG功能
	
	RCC_APB2PeriphClockCmd(OLED_RCCENA|OLED_RCCENB, ENABLE); 	
  OLED_GPIO_InitStructure.GPIO_Pin = OLED_DC|OLED_SCL|OLED_SDA|OLED_RST;
	OLED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	OLED_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OLED_GPIOA, &OLED_GPIO_InitStructure);
	GPIO_Init(OLED_GPIOB, &OLED_GPIO_InitStructure);
	
	OLED_SCLK_Set();  	
	OLED_RST_Clr();
  OLED_DLY_ms(50);
  OLED_RST_Set();
  OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(0X80,OLED_CMD); //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0
	OLED_WR_Byte(0x40|0X00,OLED_CMD); //设置显示开始行 [5:0],行数.													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC8,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x40,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 

  OLED_Fill(0x00);  //初始清屏
} 

void OLED_WrDat(unsigned char data)
{
	unsigned char i=8;

    OLED_DC_OH;
	  __NOP();
    OLED_D0_OL;
    __NOP();
  while(i--)
  {
    if(data&0x80)
	  {OLED_D1_OH;}
    else
	  {OLED_D1_OL;}
	  OLED_D0_OH;
    
    __NOP();
            
    OLED_D0_OL;    
    data<<=1;    
  }
}
void OLED_WrCmd(unsigned char cmd)
{
	unsigned char i=8;
	
   OLED_DC_OL;
   OLED_D0_OL;

   __NOP();
    
   while(i--)
   {
    if(cmd&0x80)
	  {OLED_D1_OH;}
    else
	  {OLED_D1_OL;}

    OLED_D0_OH;
    __NOP();
            
    OLED_D0_OL;   
    cmd<<=1;;   
  } 	
	//LCD_CS=1;
}
void OLED_Set_Pos(unsigned char x, unsigned char y)
{ 
  OLED_WrCmd(0xb0+y);
  OLED_WrCmd(((x&0xf0)>>4)|0x10);
  OLED_WrCmd(x&0x0f);//|0x01);
} 
void OLED_Fill(unsigned char bmp_data) //写数据比如写0x00为清屏 0xff为满频
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_data);
	}
}
void OLED_CLS(void)
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}

void adjust(unsigned char a)
{
  OLED_WrCmd(a);	//指令数据0x0000~0x003f  
}

void SetStartColumn(unsigned char d)
{
	OLED_WrCmd(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
						//   Default => 0x00
	OLED_WrCmd(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
						//   Default => 0x10
}

void SetAddressingMode(unsigned char d)	
{
	OLED_WrCmd(0x20);			// Set Memory Addressing Mode
	OLED_WrCmd(d);			//   Default => 0x02
						//     0x00 => Horizontal Addressing Mode
						//     0x01 => Vertical Addressing Mode
						//     0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
	OLED_WrCmd(0x21);			// Set Column Address
	OLED_WrCmd(a);			//   Default => 0x00 (Column Start Address)
	OLED_WrCmd(b);			//   Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char a, unsigned char b)
{
	OLED_WrCmd(0x22);			// Set Page Address
	OLED_WrCmd(a);			//   Default => 0x00 (Page Start Address)
	OLED_WrCmd(b);			//   Default => 0x07 (Page End Address)
}

  void SetStartLine(unsigned char d)
{
	OLED_WrCmd(0x40|d);			// Set Display Start Line
						//   Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char d)
{
	OLED_WrCmd(0x81);			// Set Contrast Control
	OLED_WrCmd(d);			//   Default => 0x7F
}

void Set_Charge_Pump(unsigned char d)
{
	OLED_WrCmd(0x8D);			// Set Charge Pump
	OLED_WrCmd(0x10|d);			//   Default => 0x10
						//     0x10 (0x00) => Disable Charge Pump
						//     0x14 (0x04) => Enable Charge Pump
}

void Set_Segment_Remap(unsigned char d)
{
	OLED_WrCmd(0xA0|d);			// Set Segment Re-Map
						//   Default => 0xA0
						//     0xA0 (0x00) => Column Address 0 Mapped to SEG0
						//     0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void Set_Entire_Display(unsigned char d)
{
	OLED_WrCmd(0xA4|d);			// Set Entire Display On / Off
						//   Default => 0xA4
						//     0xA4 (0x00) => Normal Display
						//     0xA5 (0x01) => Entire Display On
}

void Set_Inverse_Display(unsigned char d)
{
	OLED_WrCmd(0xA6|d);			// Set Inverse Display On/Off
						//   Default => 0xA6
						//     0xA6 (0x00) => Normal Display
						//     0xA7 (0x01) => Inverse Display On
}

void Set_Multiplex_Ratio(unsigned char d)
{
	OLED_WrCmd(0xA8);			// Set Multiplex Ratio
	OLED_WrCmd(d);			//   Default => 0x3F (1/64 Duty)
}

void Set_Display_On_Off(unsigned char d)
{
	OLED_WrCmd(0xAE|d);			// Set Display On/Off
						//   Default => 0xAE
						//     0xAE (0x00) => Display Off
						//     0xAF (0x01) => Display On
}

void SetStartPage(unsigned char d)
{
	OLED_WrCmd(0xB0|d);			// Set Page Start Address for Page Addressing Mode
						//   Default => 0xB0 (0x00)
}

void Set_Common_Remap(unsigned char d)
{
	OLED_WrCmd(0xC0|d);			// Set COM Output Scan Direction
						//   Default => 0xC0
						//     0xC0 (0x00) => Scan from COM0 to 63
						//     0xC8 (0x08) => Scan from COM63 to 0
}

void Set_Display_Offset(unsigned char d)
{
	OLED_WrCmd(0xD3);			// Set Display Offset
	OLED_WrCmd(d);			//   Default => 0x00
}

void Set_Display_Clock(unsigned char d)
{
	OLED_WrCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	OLED_WrCmd(d);			//   Default => 0x80
						//     D[3:0] => Display Clock Divider
						//     D[7:4] => Oscillator Frequency
}

void Set_Precharge_Period(unsigned char d)
{
	OLED_WrCmd(0xD9);			// Set Pre-Charge Period
	OLED_WrCmd(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void Set_Common_Config(unsigned char d)
{
	OLED_WrCmd(0xDA);			// Set COM Pins Hardware Configuration
	OLED_WrCmd(0x02|d);			//   Default => 0x12 (0x10)
						//     Alternative COM Pin Configuration
						//     Disable COM Left/Right Re-Map
}

void Set_VCOMH(unsigned char d)
{
	OLED_WrCmd(0xDB);			// Set VCOMH Deselect Level
	OLED_WrCmd(d);			//   Default => 0x20 (0.77*VCC)
}

void Set_NOP(void)
{
	OLED_WrCmd(0xE3);			// Command for No Operation
}
 
//==============================================================
//函数名：LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void OLED_P6x8Str(u8 x,u8 y,u8 ch[])//小字符串
{
  u8 c=0,i=0,j=0;
  OLED_Set_Pos(x,y);      
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>126){x=0;y++;}    
  	for(i=0;i<6;i++)     
  	  OLED_WrDat(F6x8[c][i]);  
  	x+=6;
  	j++;
  }
}
//==============================================================
//函数名：LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================  
void OLED_P8x16Str(u8 x,u8 y,u8 ch[])//大字符串
{
  u8 c=0,i=0,j=0;
        
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  OLED_WrDat(F8X16[c*16+i]);
  	OLED_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  OLED_WrDat(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}
//输出汉字字符串
void OLED_P14x16Str(unsigned char x,u8 y,unsigned char ch[])
{
	unsigned char wm=0,ii = 0;
	unsigned int adder=1; 
	
	while(ch[ii] != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(F14x16_Idx[wm] > 127)
  	{
  		if(F14x16_Idx[wm] == ch[ii])
  		{
  			if(F14x16_Idx[wm + 1] == ch[ii + 1])
  			{
  				adder = wm * 14;
  				break;
  			}
  		}
  		wm += 2;			
  	}
  	if(x>118){x=0;y++;}
  	OLED_Set_Pos(x , y); 
  	if(adder != 1)// 显示汉字					
  	{
  		OLED_Set_Pos(x , y);
  		for(wm = 0;wm < 14;wm++)               
  		{
  			OLED_WrDat(F14x16[adder]);	
  			adder += 1;
  		}      
  		OLED_Set_Pos(x,y + 1); 
  		for(wm = 0;wm < 14;wm++)          
  		{
  			OLED_WrDat(F14x16[adder]);
  			adder += 1;
  		}   		
  	}
  	else			  //显示空白字符			
  	{
  		ii += 1;
      OLED_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  				OLED_WrDat(0);
  		}
  		OLED_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{   		
  				OLED_WrDat(0);	
  		}
  	}
  	x += 14;
  	ii += 2;
	}
}
//输出汉字和字符混合字符串
void OLED_Print(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char ch2[3];
	unsigned char ii=0;        
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			OLED_P8x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];	
			ch2[1] = '\0';			//字母占一个字节
			OLED_P8x16Str(x , y , ch2);	//显示字母
			x += 8;
			ii+= 1;
		}
	}
} 

void Dis_Char(unsigned char y,unsigned char x,unsigned char asc) 
{
  unsigned char ch2[2];
	x=x*8;        
	ch2[0] = asc;	
	ch2[1] = '\0';			//字母占一个字节
	OLED_P6x8Str(x , y , ch2);	//显示字母
}
void Dis_String(unsigned char y, unsigned char x, unsigned char ch[])
{
	unsigned char ch2[3];
	unsigned char ii=0;
	x=x*8;        
	while(ch[ii] != '\0')
	{
		if(ch[ii] > 127)
		{
			ch2[0] = ch[ii];
	 		ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			OLED_P14x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		}
		else
		{
			ch2[0] = ch[ii];	
			ch2[1] = '\0';			//字母占一个字节
			OLED_P6x8Str(x , y , ch2);	//显示字母
			x += 8;
			ii+= 1;
		}
	}
}

void Dis_Num(unsigned char y, unsigned char x, unsigned int num,unsigned char N) 
{
		unsigned char j=0;
		unsigned char n[6]={0};
		x=x*8;
		n[0]=(num/10000)%10; 
		n[1]=(num/1000)%10;
		n[2]=(num/100)%10;
		n[3]=(num/10)%10;
		n[4]=num%10;
		//  n[6]='\0';			   
		for(j=0;j<5;j++) n[j]=n[j]+16+32;
		OLED_P8x16Str(x,y,&n[5-N]);//从ACSII码表中读取字节，然后写入液晶
}

void Dis_Float(unsigned char Y,unsigned char X,double real,unsigned char N) 
{
		unsigned char   i_Count=1;
		unsigned char   n[12]={0};
		long   j=1;  
		int    real_int=0;
		double decimal=0;
		unsigned int   real_decimal=0;
		if(real<0)
		{
		  real_int=(int)(-real);
		}
		else
		{
		  real_int=(int)real;
		}
		decimal=real-real_int;
		real_decimal=decimal*1e4;
		while(real_int/10/j!=0)
		{
		  j=j*10;i_Count++;  
		} 
		n[0]=(real_int/10000)%10; 
		n[1]=(real_int/1000)%10;
		n[2]=(real_int/100)%10;
		n[3]=(real_int/10)%10;
		n[4]=(real_int/1)%10; 
		n[5]='.';
		n[6]=(real_decimal/1000)%10;
		n[7]=(real_decimal/100)%10;
		n[8]=(real_decimal/10)%10;
		n[9]=real_decimal%10;
		n[6+N]='\0'; 
		for(j=0;j<10;j++) n[j]=n[j]+16+32;
		if(real<0) 
		{		 
		  i_Count+=1;
		  n[5-i_Count]='-';
		}
		n[5]='.';
		n[6+N]='\0';   
		OLED_P8x16Str(X,Y,&n[5-i_Count]); 
}

void Dis_Float2(unsigned char Y,unsigned char X,double real,unsigned char N1,unsigned char N2) 
{
		unsigned char   i_Count=1;
		unsigned char   n[12]={0};
		long   j=1;  
		unsigned int   real_int=0;
		double decimal=0;
		unsigned int   real_decimal=0;
		X=X*8;
		real_int=(int)real;
		//Dis_Num(2,0,real_int,5);
		decimal=real-real_int;
		real_decimal=decimal*1e4;
		//Dis_Num(2,6,real_decimal,4);
		while(real_int/10/j!=0)
		{
			j=j*10;i_Count++;  
		} 
		n[0]=(real_int/10000)%10; 
		n[1]=(real_int/1000)%10;
		n[2]=(real_int/100)%10;
		n[3]=(real_int/10)%10;

		n[5]='.';
		n[6]=(real_decimal/1000)%10;
		n[7]=(real_decimal/100)%10;
		n[8]=(real_decimal/10)%10;
		n[9]=real_decimal%10;
		n[6+N2]='\0'; 
		for(j=0;j<10;j++) n[j]=n[j]+16+32;
		n[5]='.';
		n[6+N2]='\0';   
		OLED_P8x16Str(X,Y,&n[5-N1]); 
}

void Draw_Logo(void)
{ 	
		unsigned int ii=0;
		unsigned char x,y;       

		for(y=0;y<8;y++)
		{
			OLED_Set_Pos(0,y);				
			for(x=0;x<128;x++)
				{      
					OLED_WrDat(JMU_LOGO[ii++]);	    	
				}
		}
} 

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	
				//if(j>=128)j=0;				
	    }
	}
}
void OLED_P8x16Num_8bit(unsigned char x,unsigned char y,unsigned char Number)
{
		unsigned char ch[3]={0};
		int n;
		for(n=2;n>=0;n--)
		{
			ch[n]=Number%10+'0';
			Number=Number/10;
		}
		OLED_P8x16Str(x,y,ch);
}

void OLED_P6x8Num_8bit(unsigned char x,unsigned char y,unsigned char Number)
{
		unsigned char ch[3]={0};
		int n;
		for(n=2;n>=0;n--)
		{
			ch[n]=Number%10+'0';
			Number=Number/10;
		}
		OLED_P6x8Str(x,y,ch);
}

void OLED_2num(unsigned char x,unsigned char y,unsigned char number)
{
		unsigned char ge,shi;
		ge = number % 10;
		shi = number / 10;
		OLED_Num(x+1,y,ge);
		OLED_Num(x,y,shi);
}

void OLED_3num(unsigned char x,unsigned char y,unsigned int number)
{
		unsigned char ge,shi,bai;
		bai = number / 100;
		shi = number % 100 /10;
		ge  = number % 10;
	
		OLED_Num(x,y,bai);
		OLED_Num(x+1,y,shi);
		OLED_Num(x+2,y,ge);
}

void OLED_4num(unsigned char x,unsigned char y, int number)
{
		unsigned char qian,bai,shi,ge;
		int num =number;
		if(num<0)
		{
			 num=-num;
		}
		qian=num/1000;
		bai=num%1000/100;
		shi=num%100/10;
		ge=num%10;

		OLED_Num(x,y,qian);
		OLED_Num(x+1,y,bai);
		OLED_Num(x+2,y,shi);
		OLED_Num(x+3,y,ge);
}

void OLED_Num5(unsigned char x,unsigned char y,unsigned int number)
{
		unsigned char wan,qian,bai,shi,ge;
		wan=number/10000;
		qian = number%10000/1000;
		bai=number%1000/100;
		shi=number%100/10;
		ge=number%10;

		OLED_Num(x,y,wan);
		OLED_Num(x+1,y,qian);
		OLED_Num(x+2,y,bai);
		OLED_Num(x+3,y,shi);
		OLED_Num(x+4,y,ge);
}


void OLED_Num(unsigned char x,unsigned char y,unsigned char asc) 
{
    int i=0;
    OLED_Set_Pos(x*6,y);
    for(i=0;i<6;i++)
    {
       OLED_WrDat(F6x8[asc+16][i]);         
    }
}



void OLED_WR_Byte1(u8 dat,u8 cmd)
{	
     u8 i;			  
     if(cmd)
        OLED_DC_OH;
     else
        OLED_DC_OL;
     for(i=0;i<8;i++)
     {			  
	   OLED_D0_OL;
	  if(dat&0x80)
             OLED_D1_OH;
          else
	      OLED_D1_OL;
	  OLED_D0_OH;
	  dat<<=1;
     }				 		    	  
}


void OLED_Clear(void)  
{  
     u8 i,n;
     for(i=0;i<8;i++)
     {  
          OLED_WR_Byte1(0xb0+i,OLED_CMD);
	        OLED_WR_Byte1(0x00,OLED_CMD);
	        OLED_WR_Byte1(0x10,OLED_CMD);
          for(n=0;n<128;n++)
              OLED_WR_Byte1(0,OLED_DATA);
     }
}

    


void OLED_Set_Pos1(u8 x,u8 y) 
{ 
     OLED_WR_Byte1(0xb0+y,OLED_CMD);//
     OLED_WR_Byte1(((x&0xf0)>>4)|0x10,OLED_CMD);
     OLED_WR_Byte1((x&0x0f)|0x01,OLED_CMD);
}


//x:0~127
//y:0~63
void OLED_ShowChar1(u8 x,u8 y,u8 a)
{      	
     u8 c,i;	
     c=a-' '; 
     if(x>127)
     {
         x=0;
         y=y+2;
     }
     OLED_Set_Pos1(x,y);//
     for(i=0;i<8;i++)
        OLED_WR_Byte1(F8X16[c*16+i],OLED_DATA);
     OLED_Set_Pos1(x,y+1);
     for(i=0;i<8;i++)
	OLED_WR_Byte1(F8X16[c*16+i+8],OLED_DATA);
}

//
void OLED_ShowString1(u8 x,u8 y,u8 *chr)
{
     u8 j=0;
     while (chr[j]!='\0')
     {		
          OLED_ShowChar1(x,y,chr[j]);
	  x+=8;
	  if(x>120)
          {
              x=0;
              y+=2;
          }
	  j++;
     }
}


void OLED_ShowCHinese1(u8 x,u8 y,u8 no)
{      			    
     u8 t;
     OLED_Set_Pos1(x,y);	
     for(t=0;t<16;t++)
        OLED_WR_Byte1(Hzk[2*no][t],OLED_DATA);
     OLED_Set_Pos1(x,y+1);	
     for(t=0;t<16;t++)
	OLED_WR_Byte1(Hzk[2*no+1][t],OLED_DATA);		
}

void display_page1(void)
{
	     OLED_Clear();
	     OLED_ShowCHinese1(27,0,0);
       OLED_ShowCHinese1(45,0,1);
		   OLED_ShowCHinese1(63,0,2);
       OLED_ShowCHinese1(81,0,3);
 	     OLED_ShowCHinese1(18,2,4);
       OLED_ShowCHinese1(36,2,5);
		   OLED_ShowCHinese1(72,2,6);
       OLED_ShowCHinese1(90,2,7);	
	     OLED_ShowString1(0,4,"I1");
	     OLED_ShowString1(0,6,"V1");
	     OLED_ShowString1(108,4,"A");
	     OLED_ShowString1(108,6,"V");	
	     OLED_ShowString1(54,4,"A");
	     OLED_ShowString1(54,6,"V");		   
	     Dis_Float(4,71,tempI1,2);
	     Dis_String(2,32,"_");
	     Dis_Float(6,71,tempV1,2);	
}

void display_page2(void)
{
	     OLED_Clear();
	     OLED_ShowCHinese1(27,0,0);
       OLED_ShowCHinese1(45,0,1);
		   OLED_ShowCHinese1(63,0,2);
       OLED_ShowCHinese1(81,0,3);
 	     OLED_ShowCHinese1(18,2,4);
       OLED_ShowCHinese1(36,2,5);
		   OLED_ShowCHinese1(72,2,6);
       OLED_ShowCHinese1(90,2,7);	
	     OLED_ShowString1(0,4,"I2");
	     OLED_ShowString1(0,6,"V2");
	     OLED_ShowString1(108,4,"A");
	     OLED_ShowString1(108,6,"V");		
	     OLED_ShowString1(54,4,"A");
	     OLED_ShowString1(54,6,"V");		   
	     Dis_Float(4,71,tempI1,2);
	     
	     Dis_Float(6,71,tempV1,2);	    
}

void display_page3(void)
{
	     OLED_Clear();
	     OLED_ShowCHinese1(27,0,0);
       OLED_ShowCHinese1(45,0,1);
		   OLED_ShowCHinese1(63,0,2);
       OLED_ShowCHinese1(81,0,3);
 	     OLED_ShowCHinese1(18,2,4);
       OLED_ShowCHinese1(36,2,5);
		   OLED_ShowCHinese1(72,2,6);
       OLED_ShowCHinese1(90,2,7);	
	     OLED_ShowString1(0,4,"I3");
	     OLED_ShowString1(0,6,"V3");
	     OLED_ShowString1(108,4,"A");
	     OLED_ShowString1(108,6,"V");		
	     OLED_ShowString1(54,4,"A");
	     OLED_ShowString1(54,6,"V");		   
	     Dis_Float(4,71,tempI1,2);
	     
	     Dis_Float(6,71,tempV1,2);			
}

void display_page4(void)
{
	     OLED_Clear();
	     OLED_ShowCHinese1(27,0,0);
       OLED_ShowCHinese1(45,0,1);
		   OLED_ShowCHinese1(63,0,2);
       OLED_ShowCHinese1(81,0,3);
 	     OLED_ShowCHinese1(18,2,4);
       OLED_ShowCHinese1(36,2,5);
		   OLED_ShowCHinese1(72,2,6);
       OLED_ShowCHinese1(90,2,7);	
	     OLED_ShowString1(0,4,"I4");
	     OLED_ShowString1(0,6,"V4");
	     OLED_ShowString1(108,4,"A");
	     OLED_ShowString1(108,6,"V");	
	     OLED_ShowString1(54,4,"A");
	     OLED_ShowString1(54,6,"V");		   
	     Dis_Float(4,71,tempI1,2);
	     
	     Dis_Float(6,71,tempV1,2);	  
}

void OLED_SetChar(u8 x,u8 y,u8 ch[])
{
  u8 c=0,i=0,j=0;
        
  while (ch[j]!='\0')
  {    
    c =ch[j]-32;
    if(x>120){x=0;y++;}
    OLED_Set_Pos(x,y);    
  	for(i=0;i<8;i++)     
  	  OLED_WrDat(F8X16[c*16+i]);
  	OLED_Set_Pos(x,y+1);    
  	for(i=0;i<8;i++)     
  	  OLED_WrDat(F8X16[c*16+i+8]);  
  	x+=8;
  	j++;
  }
}
