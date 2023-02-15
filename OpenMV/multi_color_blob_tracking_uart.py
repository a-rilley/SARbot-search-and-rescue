# Phase C - Detect blocked path and miner
# Uses UART Serial to relay flags to main microcontroller
# Code derived from OpenMV - Multi Color Blob Tracking Example
# Modified to suit needs

import sensor, image, time, math, pyb

from pyb import UART

uart = UART(3, 115200, timeout_char=1000)                         # init with given baudrate
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1000) # init with given parameters

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...

thresholds_1 = [(15, 100, -15, 80, -128, -25), # generic_blue_thresholds
                (15, 100, 40, 127, 15, 70)    # generic_red_thresholds
                ]      # generic_yellow_thresholds  (15, 100, -9, 9, 45, 128)

thresholds_2 = [(10, 100, -64, -8, -25, 32)]   # generic_green_thresholds


# You may pass up to 16 thresholds above. However, it's not really possible to segment any
# scene with 16 thresholds before color thresholds start to overlap heavily.

sensor.reset()
sensor.set_brightness(+3)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.set_vflip(True)
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. Don't set "merge=True" becuase that will merge blobs which we don't want here.

while(True):
    clock.tick()
    img = sensor.snapshot()

    #For detecting the miner blocked path
    for blob in img.find_blobs(thresholds_1, pixels_threshold=300, area_threshold=300, merge=False):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        # These values are stable all the time.
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        uart.writechar(1)


    # For detecting the miner
    for blob in img.find_blobs(thresholds_2, pixels_threshold=300, area_threshold=300, merge=False):
        # These values depend on the blob not being circular - otherwise they will be shaky.
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
            img.draw_line(blob.minor_axis_line(), color=(0,0,255))
        # These values are stable all the time.
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)
        uart.writechar(2)

    char = int(uart.readchar())
    pyb.delay(1)


    # Debugging
    if char > 0 and char < 3:
        print(char)
