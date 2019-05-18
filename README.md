# ardupilot
本项目的逻辑如下：

![原理](https://i.loli.net/2019/05/18/5cdfb2d58fa6686315.png)

所以我们的项目分为四个文件夹：

| MCU     | 作用                     | 功能                                 |
| ------- | ------------------------ | ------------------------------------ |
| Arduino | 用来存放小车的程序       | 1、接收来自遥控器的命令              |
|         |                          | 2、能够对命令进行执行，驱动小车前进  |
|         |                          | 3、能够把命令通过无线的方式传到STM32 |
| STM32   | 整合信息，输出命令       | 1、接收来自Arduino、OpenMV的信息     |
|         |                          | 2、对接收的信息进行处理，转化为命令  |
|         |                          | 3、控制APM进行一系列动作             |
| OpenMV  | 得出小车与飞机的相对信息 | 1、识别小车的偏离程度                |
|         |                          | 2、传递命令信息给STM32               |
| APM     | 真正控制飞机             | 1、接收来自STM32的命令信息           |
|         |                          | 2、控制飞机                          |

## APM

相关教程：

- [APM飞控学习之路：1 无人机的分类与发展](https://github.com/ptrtonull-workshop/ardupilot/blob/master/APM/blog/1/index.md)
- [APM飞控学习之路：2 四旋翼的工作原理与系统组成](https://github.com/ptrtonull-workshop/ardupilot/blob/master/APM/blog/2/index.md)
- [APM飞控学习之路：3 APM系统介绍与开发环境搭建](https://github.com/ptrtonull-workshop/ardupilot/blob/master/APM/blog/3/index.md)

- [APM飞控学习之路：4 源码裁剪与下载](https://github.com/ptrtonull-workshop/ardupilot/blob/master/APM/blog/4/index.md)
- [APM飞控学习之路：5 串口概述与收发调试](https://github.com/ptrtonull-workshop/ardupilot/blob/master/APM/blog/5/index.md)

