#!/usr/bin/env python
# -*- coding: utf-8 -*-

import Adafruit_GPIO as GPIO
import Adafruit_GPIO.SPI as SPI

import ST7789
import datetime
from time import sleep

from PIL import Image, ImageDraw, ImageFont, ImageColor

import numpy as np

# Raspberry Pi pin configuration:
RST = 25
DC  = 24
LED = 27
SPI_PORT = 0
SPI_DEVICE = 0
SPI_MODE = 0b11
SPI_SPEED_HZ = 40000000


disp = ST7789.ST7789(port = SPI_PORT, spi_speed_hz=SPI_SPEED_HZ, cs = 0, dc=DC, backlight=LED)
disp._spi.mode = SPI_MODE
disp.begin()
#disp.clear()
image = Image.new('RGB', (240,240),(255,0,0))
draw = ImageDraw.Draw(image)
disp.display(image)
sleep(5)
