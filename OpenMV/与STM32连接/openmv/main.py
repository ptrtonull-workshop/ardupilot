# Remote Control Demonstration
#
# OpenMV CAM controlled with UART/MCU
#
# (C) Johannes Hantsch 2018
#
# P4 = TXD
# P5 = RXD
#

import sensor, image, time, struct, pyb, utime
from pyb import UART
# import ipd sources
import ipd

# Parameters #################################################################

color_code_mode = 1 # 0 == Disabled, 1 == Enabled, 2 == Color Codes Only, 3 == Mixed

max_blocks = 1000
max_blocks_per_signature = 1000
min_block_area = 20

uart_baudrate = 9600  #设置uart波特率

# INTERNAL SETUP ##############################################################

# 开发板上的LED定义

led_red = pyb.LED(1)
led_green = pyb.LED(2)
led_blue = pyb.LED(3)

led_red.off()
led_green.off()
led_blue.off()

# Link Setup

uart = UART(3, uart_baudrate, timeout_char = 1000)  #第一个3代表串口，第二个代表波特率，第三个代表超时时间

def uart_write(data): #定义一个写数据的函数
    uart.write(data)

def uart_available(): #定义一个串口可行性函数，可以的话返回
    return uart.any()

def uart_readByte():         #读取一个字符
    return uart.readchar()

# 摄像头初始化 #################################################################

def def_camInit():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 2000)
    img = sensor.snapshot()

# 图像发送 #################################################################

def send_image():
    img = sensor.snapshot() #.compress(quality=30)
    #uart.write(struct.pack("<l", img.size()))
    uart.write(img)

# 状态机 ##############################################################

FSM_STATE_IDLE = 1
FSM_STATE_COLOR = 2  #定义颜色
FSM_STATE_FACE = 3
FSM_STATE_CODE = 4
FSM_STATE_LED_CONTROL = 5

fsm_state = FSM_STATE_IDLE
last_byte = 0

def_camInit()

def parse_byte(byte):
    global fsm_state
    global last_byte

    if fsm_state == FSM_STATE_IDLE:
        print("IDLE")
        def_camInit()
        if byte == 0xFF: fsm_state = FSM_STATE_COLOR
        elif byte == 0xFE: fsm_state = FSM_STATE_FACE
        elif byte == 0xFD: fsm_state = FSM_STATE_CODE
        elif byte == 0xFC: fsm_state = FSM_STATE_LED_CONTROL

    elif fsm_state == FSM_STATE_COLOR:
        print("COLOR")
        # do something
        if byte == 0xAB: fsm_state = FSM_STATE_IDLE
        elif byte == 0xAA:
            ipd.circletrack_camInit()
            uart.write("Waiting for color blob...\r")
            while(True):
                ipd.circletrack_run()
                uart.write("X:%03d\r"%ipd.x)
                uart.write("Y:%03d\r"%ipd.y)
                if uart_available() != 0:
                    if uart_readByte() == 0xAB:
                        fsm_state = FSM_STATE_IDLE
                        break

    elif fsm_state == FSM_STATE_FACE:
        print("FACE")
        # do something
        if byte == 0xAB: fsm_state = FSM_STATE_IDLE
        elif byte == 0xAA:
            ipd.facetrack_camInit()
            uart.write("Searching a face...\r")
            ipd.facetrack_findFace()
            while(True):
                ipd.facetrack_run()
                uart.write("X:%03d\r"%ipd.x)
                uart.write("Y:%03d\r"%ipd.y)
                if uart_available() != 0:
                    if uart_readByte() == 0xAB:
                        fsm_state = FSM_STATE_IDLE
                        break

    elif fsm_state == FSM_STATE_CODE:
        print("CODE")
        # do something
        if byte == 0xAB: fsm_state = FSM_STATE_IDLE
        elif byte == 0xAA:
            ipd.codereader_camInit()
            # set values to zero (no track)
            uart.write("X:000\r")
            uart.write("Y:000\r")
            while(True):
                ipd.codereader_run()
                uart.write("QR:%s\r"%ipd.payload)
                if uart_available() != 0:
                    if uart_readByte() == 0xAB:
                        fsm_state = FSM_STATE_IDLE
                        break

    elif fsm_state == FSM_STATE_LED_CONTROL:
        print("LED")
        if byte == 0x00: fsm_state = FSM_STATE_IDLE
        elif byte == 0xAA: led_red.on()
        elif byte == 0xAB: led_red.off()

    last_byte = byte


# MAIN LOOP ##############################################################

clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    # UART Recieve Byte
    for i in range(uart_available()):
        print("RX: ",uart_readByte())
        parse_byte(uart_readByte())

