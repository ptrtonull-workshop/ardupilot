#2018.8.2
import sensor, image, time , pyb
from pyb import UART
from pyb import Timer
from pyb import LED
import json

led = pyb.LED(3) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
thresholds = [(27, 67, 19, 91, 45, 76), # 红色
              #(21, 75, 3, -38, 34, 68), # 绿色
              (27, 90, -3, -28, 31, 125),
              (0, 30, 0, 64, -128, 0)]  # generic_blue_thresholds
threshold_index = 1 # 0 for red, 1 for gre9en, 2 for blue


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)   #320*240
sensor.skip_frames(time = 100)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()
uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1000) # 使用给定参数初始化

def tick(timer):            # we will receive the timer object when being called
    global data
    if blobs:
        print("Find")
        print('you send:',output_str)
        uart.write(data)


tim = Timer(4, freq=10)      # create a timer object using timer 4 - trigger at 1Hz
tim.callback(tick)          # set the callback to our tick function


def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

def Uart_Receive():   #UART接收 改变框小球的颜色阈值
    global threshold_index
    if uart.any():
        temp_data = uart.readchar()
        if temp_data==0:   #红色
           threshold_index=0

           print(temp_data,threshold_index)
        elif temp_data==1:
           threshold_index=1
           print(temp_data,threshold_index)


while(True):
    clock.tick()
    img = sensor.snapshot()
    Uart_Receive()
    blobs = img.find_blobs([thresholds[threshold_index]])
    if blobs:
        print("YES")

        max_blob = find_max(blobs)
        b = max_blob[0] #方框元组
        L = (max_blob[2]+max_blob[3])/2
        l=int(1000/L)
        #x_error = max_blob[5]-img.width()/2   #求横向偏差
        x_error = max_blob[5]-img.width()/2

        img.draw_rectangle(max_blob[0:4])        # 画矩形
        img.draw_cross(max_blob[5], max_blob[6]) # 画十字

        #发送 小球的(x,y,l,n)
        #x为横坐标，y为纵坐标，l为粗略的距离，n为小球颜色(0：红 1：绿)
        output_str="%d,%d,%d,%d" % (max_blob.cx(),max_blob.cy(),l,threshold_index) #10进制字符包
        checkout=0xAA+0x55+0x07+int(max_blob.cx()/2)+max_blob.cy()+l+threshold_index
        data = bytearray([0xAA,0x55,0x07,int(max_blob.cx()/2),max_blob.cy(),l,threshold_index,0x00,0x00,checkout])#转成16进制
        #uart.write(data)
        time.sleep(1)
        led.on()
    else:
        print("NO FIND")
        data = bytearray([0xAA,0x55,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x06])
        uart.write(data)
        led.off()



