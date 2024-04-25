#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI

import ST7789 as TFT
import datetime
from time import sleep

from PIL import Image, ImageDraw, ImageFont, ImageColor

import numpy as np

# Raspberry Pi pin configuration:
# pin and display configurations
# already defined in ./ST7789/ST7789.py
"""
RST = 25
DC  = 24
LED = 27
SPI_PORT = 0
SPI_DEVICE = 0
SPI_MODE = 0b11
SPI_SPEED_HZ = 40000000
"""

disp = TFT.ST7789()

# Initialize display.
disp.begin()

# Clear image buffer
disp.clear()

# Load images
mapimg = Image.open('map.jpg')
teamname = Image.open('img1.jpg')

try:
# Display images on loop
    while True:
        disp.display(teamname)
        sleep(15)
        disp.display(mapimg)
        sleep(15)

except KeyboardInterrupt:
    pass
finally:
    print("Interrupted")
    #clear clears the image buffer and by default sets it to RGB value for black
    disp.clear()
    disp.display()
    
