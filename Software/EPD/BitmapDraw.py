# -*- coding: utf-8 -*-
"""
Created on Sat Nov  9 16:52:49 2019

@author: crenda
"""
import numpy as np
from PIL import Image, ImageDraw, ImageFont

img = Image.new('RGBA', (1200, 825), 'white')    
idraw = ImageDraw.Draw(img)
text = "High Tatras"
h = np.arange(105,900,180)
header = 105
gainsbro = (220,220,220)
darkgray = (169,169,169)
h = []
font = ImageFont.truetype("arial.ttf", size=18)
print(h)
for i in h:
    print(i)
    idraw.rectangle((0, i, 1200, i+90), fill=(220,220,220))
    idraw.text((10, 10), text, font=font)
    idraw.line((0, i, 1200, i), fill=(0, 0, 0), width=10)
    idraw.line((0, i+90, 1200, i+90), fill=(0, 0, 0), width=10)
 
img.save('rectangle.bmp')