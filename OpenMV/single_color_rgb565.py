import sensor, image, time
from pyb import Pin
from pyb import LED

move_left     = Pin('P1', Pin.OUT_PP)
move_right    = Pin('P2', Pin.OUT_PP)
move_forword  = Pin('P3', Pin.OUT_PP)
move_back     = Pin('P4', Pin.OUT_PP)

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(30, 100, 15, 127, 15, 127),  # generic_red_thresholds
              (30, 100, -64, -8, -32, 32),  # generic_green_thresholds
              (0, 50, 0, 64, -80, 0),       # generic_blue_thresholds
              (0,40,-15,40,-14,20)]         # black

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)  #160x120
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)         # must be turned off for color tracking
sensor.set_auto_whitebal(False)     # must be turned off for color tracking
clock = time.clock()
led2 = LED(2)#green,get x,y dispaly
led3 = LED(3)#run display

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

while(True):
    clock.tick()
    img = sensor.snapshot()

    threshold_index = 3         #chose color

    for blob in img.find_blobs([thresholds[threshold_index]], pixels_threshold=200, area_threshold=200, merge=True):
        led2.toggle()
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        if   threshold_index == 0:
            color_bar=(255,0,0) #red
            color_text="red"
        elif threshold_index == 1:
            color_bar=(0,255,0) #green
            color_text="green"
        elif threshold_index == 2:
            color_bar=(0,255,0) #blue
            color_text="blue"
        elif threshold_index == 3:
            color_bar=(0,0,0)   #black
            color_text="black"
        img.draw_string(blob.x() - 2, blob.y() - 8, color_text,color=color_bar)

        #move left or right
        if blob.cx() <=40:
            move_right.high()
        elif blob.cx()>=120:
            move_left.high()
        else:
            move_right.low()
            move_left.low()
        #move forword or back
        if blob.cy() <=30:
            move_back.high()
        elif blob.cy()>=90:
            move_forword.high()
        else:
            move_back.low()
            move_forword.low()

    print(clock.fps())
    led3.toggle()

