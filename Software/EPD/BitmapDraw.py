# -*- coding: utf-8 -*-
"""
Spyder Editor

Dies ist eine temporäre Skriptdatei.
"""

import matplotlib.pyplot as plt
import numpy as np
import serial
import time

# Parameter
on = 50 # Standard deviation of white noise

# Load image
name = 'Kalender.png'

f = plt.imread('E-LINK-TCON-DEMO/DemoPIC/{}'.format(name))

# Tranform image to grayscale and float
if f.ndim > 2:
    f = np.mean(f,axis=2)
f = np.array(f, dtype=float)
(M, N) = f.shape

f = np.array(np.round(f/255*15), dtype=int)
x = np.reshape(f, -1)

#als txt-file
#f_out = [hex(x[i]+x[i+1]*16) for i in np.arange(0, np.size(x), 2)]
#
#with open('{}'.format(name.strip('bmp')+'c'), 'w') as file:
#    s = 'const unsigned char {}[]={}'.format(name.strip('.bmp'), '{')
#    file.write(s)
#    s = ','.join([str(i) for i in f_out])
#    file.write(s)
#    file.write('};')


with open('{}'.format(name.strip('png')+'c'), 'w') as file:
    s = 'const unsigned char {}[]={}'.format(name.strip('.png'), '{')
    file.write(s)
    s = ','.join([str(i) for i in f_out])
    file.write(s)
    file.write('};')
=======
#an comport senden
f_out = [int(x[i]+x[i+1]*16) for i in np.arange(0, np.size(x), 2)]


data_bytes = [i.to_bytes(1, 'little') for i in f_out]

baudrate = 115200
com_port = 'COM17'

device = serial.Serial(com_port, baudrate, writeTimeout=0)

device.reset_output_buffer()

#for b in data_bytes:
#    device.write(b)
#    device.write(b'\n')
#    time.sleep(0.01)

t_start = time.time()
print(device.write(bytes(f_out)))
print('waiting:', device.out_waiting)
device.flush()
print('elapsed time: {:.3f}'.format(time.time() - t_start))

time.sleep(2)
device.close()
    