# Image Processing Routines
#
# (C) Johannes Hantsch 2019
#

import sensor, image, time, struct, pyb, utime

# FACE TRACK #################################################################

# init cam for face tracking
def facetrack_camInit():
    print("INIT Facetrack")
    # Reset sensor
    sensor.reset()
    sensor.set_contrast(3)
    sensor.set_gainceiling(16)
    sensor.set_framesize(sensor.VGA)
    sensor.set_windowing((200, 200))
    sensor.set_pixformat(sensor.GRAYSCALE)
    sensor.skip_frames(time = 2000)

# Load Cascade
face_cascade = image.HaarCascade("frontalface", stages=25)

# First set of keypoints
kpts1 = None

def facetrack_findFace():
    global kpts1
    global img
    global face_cascade

    while (kpts1 == None):
        img = sensor.snapshot()
        img.draw_string(0, 0, "Looking for a face...")
        # Find faces
        objects = img.find_features(face_cascade, threshold=0.5, scale=1.25)
        if objects:
            # Expand the ROI by 31 pixels in every direction
            face = (objects[0][0]-31, objects[0][1]-31,objects[0][2]+31*2, objects[0][3]+31*2)
            # Extract keypoints using the detect face size as the ROI
            kpts1 = img.find_keypoints(threshold=10, scale_factor=1.1, max_keypoints=100, roi=face)
            # Draw a rectangle arod the first face
            img.draw_rectangle(objects[0])

    # Draw keypoints
    print(kpts1)
    img.draw_keypoints(kpts1, size=24)
    img = sensor.snapshot()
    time.sleep(2000)

    # FPS clock
    clock = time.clock()
    return

def facetrack_run():
    global kpts1
    global img
    global x
    global y
    global x_pos
    global y_pos

    clock = time.clock()

    clock.tick()
    img = sensor.snapshot()
    # Extract keypoints using the detect face size as the ROI
    kpts2 = img.find_keypoints(max_keypoints=150, threshold=10, normalized=True)

    if (kpts2):
        # Match the first set of keypoints with the second one
        c = image.match_descriptor(kpts1, kpts2, threshold=85)
        # If more than 10% of the keypoints match draw the matching set
        if (c[2]>25):
            img.draw_cross(c[0], c[1], size=5)
            img.draw_string(0, 10, "Match %d%%"%(c[2]))

            x = c[0]
            y = c[1]
    utime.sleep_ms(300)

# COLOR TRACK #################################################################

def colortrack_camInit():
    global colorThresholds
    global clock

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_auto_gain(False) # must be turned off for color tracking
    sensor.set_auto_whitebal(False) # must be turned off for color tracking
    sensor.set_windowing((200, 200))

    # Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
    # thresholds for general red/green things
    colorThresholds = [(30, 100, 15, 127, 15, 127), # generic_red_thresholds
                        (30, 100, -64, -8, -32, 32)] # generic_green_thresholds

    clock = time.clock()

def colortrack_run():
    global colorThresholds
    global clock
    global x
    global y

    x = 333
    y = 333

    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs(colorThresholds, pixels_threshold=100, area_threshold=100, merge=True):
        if blob.code() == 3: # r/g code == (1 << 1) | (1 << 0)
            x = blob.cx()
            y = blob.cy()
            img.draw_rectangle(blob.rect())
            img.draw_cross(x, y)

# READ CODE #################################################################

def codereader_camInit():
    global clock
    global payload

    payload = "waiting for code"

    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_auto_gain(False) # must turn this off to prevent image washout...
    clock = time.clock()

def codereader_run():
    global payload
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = (255, 0, 0))
        payload = code.payload();
        print(code)

# TRACK CIRCLE #################################################################

def circletrack_camInit():
    global clock

    sensor.reset()
    #sensor.set_pixformat(sensor.RGB565) # grayscale is faster

    sensor.set_pixformat(sensor.GRAYSCALE)
    sensor.set_framesize(sensor.QVGA)
    sensor.set_windowing((200, 200))
    sensor.skip_frames(time = 2000)
    clock = time.clock()

def circletrack_run():
    global clock
    global x
    global y

    x = 333
    y = 333

    img = sensor.snapshot().lens_corr(1.8)

    for c in img.find_circles(threshold = 3500, x_margin = 10, y_margin = 10, r_margin = 10,
            r_min = 2, r_max = 100, r_step = 2):
        img.draw_circle(c.x(), c.y(), c.r(), color = (255, 0, 0))

        x = c.x()
        y = c.y()