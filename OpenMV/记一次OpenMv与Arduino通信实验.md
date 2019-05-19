## 记一次OpenMv与Arduino通信实验

### 实验目的

验证OpenMV与Arduino的uart的串口通信，从OpenMV发送信息到Arduino，Arduoino做出反映

### LED灯亮

#### 实验连线

我们通过Arduino里的D10这个GPIO口来控制LED灯的闪烁。

![1.png](https://i.loli.net/2019/05/19/5ce0c699387af99361.png)

由于Arduino的电流太大，我们需要给灯接一个220Ω的电阻来保护它，具体的实物图连接如下：

![2.png](https://i.loli.net/2019/05/19/5ce0c7998edda53345.png)

#### 烧录代码

我们可以先试着点亮一个LED，我们为Arduino烧录一下程序：

```c
int ledPin = 10; //定义数字10 接口
void setup()
{
	pinMode(ledPin, OUTPUT);//定义小灯接口为输出接口
}
void loop()
{
    digitalWrite(ledPin, HIGH); //点亮小灯
    delay(1000); //延时1 秒
    digitalWrite(ledPin, LOW); //熄灭小灯
    delay(1000); // 延时1 秒
}
```

#### 观察现象

观察到灯闪亮：

![闪烁](https://s2.ax1x.com/2019/05/19/EXTpTK.gif)

### uart传输

#### 实验连线

先按上面的实验把LED灯连好，我们开始连接OpenMV。

![1](http://book.openmv.cc/assets/06-04-001.jpg)

注意：当我们下载好程序后，记得把Arduino从电脑端拿下来，连接到充电宝上，因为我用的是数据线供电，连着电脑时会占用uart串口。

#### 烧录代码

我们在OpenMV里烧录：

```python
import time 
from pyb import UART 
str="H" 
uart = UART(3, 9600)   //设置串口和波特率，这里的波特率一定要和Arduino的相同
while(True): 
    uart.write(str)  //发送信息
    print(str);  //打印结果
    time.sleep(1000) 
```

我们要在arduino烧录接收代码：

```c
int incomedate = 0;
int ledPin = 10; //定义数字10 接口
void setup() {
  Serial.begin(9600); //设置串口波特率9600
  pinMode(ledPin, OUTPUT);//定义小灯接口为输出接口
}
void loop() {
      
  if (Serial.available() > 0)//串口接收到数据
  {
    incomedate = Serial.read();//获取串口接收到的数据
    if (incomedate == 'H')
    {
      digitalWrite(ledPin, HIGH); //点亮小灯
      delay(1000); //延时1 秒
      digitalWrite(ledPin, LOW); //熄灭小灯
      delay(1000); // 延时1 秒
    }
  }
  delay(1000);
}

```

#### 观察现象

当我们在OpenMV IDE运行程序时，串口终端显示我们已经向Arduino发送了信息

![捕获.png](https://i.loli.net/2019/05/19/5ce0cdec84e0826184.png)

Arduino再次闪烁

![闪烁](https://s2.ax1x.com/2019/05/19/EXTpTK.gif)

### 参考

- [Arduino基础入门篇23—串口通讯](https://www.jianshu.com/p/a102ebeb1f1f)
- [Arduino UNO 点亮LED实验](https://jingyan.baidu.com/article/f006222803ded7fbd3f0c881.html)

- [OpenMV串口通信上](http://book.openmv.cc/MCU/serial1.html)

