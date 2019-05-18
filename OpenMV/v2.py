# -*- coding: utf8 -*-
import sensor, image, time
from pyb import Pin,LED

move_left     = Pin('P1', Pin.OUT_PP)
move_right    = Pin('P2', Pin.OUT_PP)
move_forword  = Pin('P3', Pin.OUT_PP)
move_back     = Pin('P4', Pin.OUT_PP)

color_thresholds = [(30, 100,  15, 127,  15, 127),  # generic_red_thresholds
                    (30, 100, -64,  -8, -32,  32),  # generic_green_thresholds
                    (0,   50,   0,  64, -80,   0),  # generic_blue_thresholds
                    (0,   90, -15,  10,  -10, 10)]  # black
threshold_index = 3                                 #chose color

line_thresholds = [(50,0,50,120,  "L"),
                   (110,0,110,120,"R"),
                   (0,35,160,35,  "U"),
                   (0,85,160,85,  "D")]

sensor.reset()
#sensor.set_pixformat(sensor.RGB565)
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)  #160x120
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)         # must be turned off for color tracking
sensor.set_auto_whitebal(False)     # must be turned off for color tracking
clock = time.clock()
led2 = LED(2)                       #green,get x,y dispaly
led3 = LED(3)                       #run display

def uart3_sent_img(image_origin):
    origin_img = image_origin
    import image
    from pyb import UART
    uart = UART(3,921600,bits=8, parity=None, stop=1, timeout_char=1000,timeout=1000)
    formate = origin_img.format()
    pic = origin_img.compress(quality=60)

    uart.write("$7$"+str(pic.width()))
    uart.write(str(pic.height()))
    uart.write(str(formate))
    uart.write(str(pic.size())+"\n")

    uart.write("$")                         #图片头帧
    uart.write(pic)                         #图片
    uart.write("$")                         #图片尾帧
    return("OK")

while(True):
    img = sensor.snapshot()
    for blob in img.find_blobs([color_thresholds[threshold_index]],
                                pixels_threshold=30, area_threshold=50,
                                merge=True):
        led2.toggle()
        img.draw_rectangle(blob.rect())

        #move left or right
        if blob.cx() >=(110):
            move_right.high()
            img.draw_string(110,60,'R')
        elif blob.cx()<=(50):
            move_left.high()
            img.draw_string(40,60,'L')
        else:
            move_right.low()
            move_left.low()

        #move forword or back
        if blob.cy() >=(85):
            move_back.high()
            img.draw_string(80,85,'B')
        elif blob.cy()<=(35):
            move_forword.high()
            img.draw_string(80,25,'F')
        else:
            move_back.low()
            move_forword.low()

    #draw section
    for item in line_thresholds:
        img.draw_line(item[0:4])
        img.draw_string(item[0],item[1],item[4])

    uart3_sent_img(img) #send image to PC
    led3.toggle()

