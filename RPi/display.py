import ST7789  
from PIL import Image  
from time import sleep  
display=ST7789.ST7789(port=0,cs=0,rst=25,dc=27,backlight=24,spi_speed_hz=4000000)  
display._spi.mode=0  
display.begin() 
image=Image.new('RGB',(240,240),(255,0,255))  #('RGB',(240,240),(r,g,b))
display.display(image)  
sleep(2)   
