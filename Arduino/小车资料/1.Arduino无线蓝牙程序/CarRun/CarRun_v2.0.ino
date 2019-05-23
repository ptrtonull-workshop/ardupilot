//============================启航科技===========================、
/*前进  按下发出 ONA  松开ONF
  后退：按下发出 ONB  松开ONF
  左转：按下发出 ONC  松开ONF
  右转：按下发出 OND  松开ONF
  停止：按下发出 ONE  松开ONF
  
  蓝牙程序功能是按下对应的按键执行操，松开按键就停止
*/

#include <IRremote.h>


int Left_motor_go=8;       //左电机前进(IN1)
int Left_motor_back=9;     //左电机后退(IN2)
int Right_motor_go=10;     // 右电机前进(IN3)
int Right_motor_back=11;   // 右电机后退(IN4)

int Trig = 4;              //超声波模块：发射端
int Echo = 7;              //超声波模块：接收端


IRsend irsend;

        //POWER:FD00FF    run
        //VOL+:FD807F     left
        //STOP:FD40BF     right
        //<<:  FD20DF     back
        //Play:FDA05F     brake
        // >>: FD609F


void setup()
{
  //初始化电机驱动IO为输出方式
   Serial.begin(9600);
  pinMode(Left_motor_go,OUTPUT);       // PIN 8 (无PWM)
  pinMode(Left_motor_back,OUTPUT);     // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);      // PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);    // PIN 11 (PWM)

  pinMode(Trig,OUTPUT);                //PIN 4
  pinMode(Echo,INPUT);                 //PIN 7
  

  
}
void run()
{
  
  irsend.sendNEC(0xFD00FF,32);         //红外发送信息
  
  digitalWrite(Right_motor_go,HIGH);   // 右电机前进
  digitalWrite(Right_motor_back,LOW);     
  //analogWrite(Right_motor_go,150);   //PWM比例0~255调速，左右轮差异略增减（可以测试时用）
  //analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);     // 左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0);      //PWM比例0~255调速，左右轮差异略增减
  //analogWrite(Left_motor_back,150);
  //delay(time * 100);                 //执行时间，可以调整  

}

void brake()         //刹车，停车
{
  
  irsend.sendNEC(0xFDA05F,32);
  
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);

  
}

void left()
{
  irsend.sendNEC(0xFD807F,32);
  
  digitalWrite(Right_motor_go,HIGH);    // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,150); 
  //analogWrite(Right_motor_back,0);    //PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);      //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,0); 
  //analogWrite(Left_motor_back,0);     //PWM比例0~255调速
  //delay(time * 100);                  //执行时间，可以调整  
}



void spin_left(int time)                //左转(左轮后退，右轮前进)
{
  
  digitalWrite(Right_motor_go,HIGH);    // 右电机前进
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,200); 
  //analogWrite(Right_motor_back,0);    //PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);     //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,200); 
  //analogWrite(Left_motor_back,0);     //PWM比例0~255调速
  //delay(time * 100);                  //执行时间，可以调整  
}


void right()
{
  
  irsend.sendNEC(0xFD40BF,32);
  digitalWrite(Right_motor_go,LOW);     //右电机后退
  digitalWrite(Right_motor_back,LOW);
  //analogWrite(Right_motor_go,0); 
  // analogWrite(Right_motor_back,0);   //PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);      //左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0); 
  //analogWrite(Left_motor_back,150);   //PWM比例0~255调速
  //delay(time * 100);                  //执行时间，可以调整  

}

void spin_right(int time)               //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);     //右电机后退
  digitalWrite(Right_motor_back,HIGH);
  //analogWrite(Right_motor_go,0); 
  //analogWrite(Right_motor_back,200);  //PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);      //左电机前进
  digitalWrite(Left_motor_back,HIGH);
  //analogWrite(Left_motor_go,0); 
  //analogWrite(Left_motor_back,200);   //PWM比例0~255调速
  //delay(time * 100);                  //执行时间，可以调整    
}

void back()
{
  irsend.sendNEC(0xFD20DF,32);
  digitalWrite(Right_motor_go,LOW);     //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  //analogWrite(Right_motor_go,0);
  //analogWrite(Right_motor_back,150);  //PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);     //左轮后退
  digitalWrite(Left_motor_back,LOW);
  //analogWrite(Left_motor_go,150);
  //analogWrite(Left_motor_back,0);     //PWM比例0~255调速
  //delay(time * 100);                  //执行时间，可以调整  

}



void loop()
{ 
  
  getstr=Serial.read(); 

  if(getstr=='A') 
  { 
    Serial.println("go forward!"); 
    run();
  } 
  else if(getstr=='B'){ 
   Serial.println("go back!"); 
   back();
  } 
  else if(getstr=='C'){ 
   Serial.println("go left!");
   left();
  } 
  else if(getstr=='D'){ 
    Serial.println("go right!");
    right(); 
  } 
   else if(getstr=='F'){ 
    Serial.println("Stop!");
    brake();    
  } 
   else if(getstr=='E'){ 
    Serial.println("Stop!");
    brake();
  } 

  //给Trig发送一个低高低的短时间脉冲,触发测距
  digitalWrite(Trig, LOW); //给Trig发送一个低电平
  delayMicroseconds(2);    //等待 2微妙
  digitalWrite(Trig,HIGH); //给Trig发送一个高电平
  delayMicroseconds(10);    //等待 10微妙
  digitalWrite(Trig, LOW); //给Trig发送一个低电平
  
  temp = float(pulseIn(Echo, HIGH)); //存储回波等待时间,
  //pulseIn函数会等待引脚变为HIGH,开始计算时间,再等待变为LOW并停止计时
  //返回脉冲的长度
  
  //声速是:340m/1s 换算成 34000cm / 1000000μs => 34 / 1000
  //因为发送到接收,实际是相同距离走了2回,所以要除以2
  //距离(厘米)  =  (回波时间 * (34 / 1000)) / 2
  //简化后的计算公式为 (回波时间 * 17)/ 1000
  
  cm = (temp*17 )/1000; //把回波时间换算成cm
  if(cm < 10)
  {
          //引入beep的相关内容
  }

}
