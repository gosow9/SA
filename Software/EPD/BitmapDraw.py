# -*- coding: utf-8 -*-
"""
Spyder Editor

Dies ist eine temporäre Skriptdatei.
"""

import matplotlib.pyplot as plt
import numpy as np

# Load image
name = 'Kalender.png'

f = plt.imread('E-LINK-TCON-DEMO/DemoPIC/{}'.format(name))

# Tranform image to grayscale and float
if f.ndim > 2:
    f = np.mean(f,axis=2)
f = np.array(f, dtype=float)
(M, N) = f.shape

f = np.array(np.round(f*15), dtype=int)
x = np.reshape(f, -1)

f_out = [hex(x[i]+x[i+1]*16) for i in np.arange(0, np.size(x), 2)]

#als txt-file
#with open('{}'.format(name.strip('bmp')+'c'), 'w') as file:
#    s = 'const unsigned char {}[]={}'.format(name.strip('.bmp'), '{')
#    file.write(s)
#    s = ','.join([str(i) for i in f_out])
#    file.write(s)
#    file.write('};')

#als c-file
with open('{}'.format(name.strip('png')+'c'), 'w') as file:
    s = 'const unsigned char {}[]={}'.format(name.strip('.png'), '{')
    file.write(s)
    s = ','.join([str(i) for i in f_out])
    file.write(s)
    file.write('};')