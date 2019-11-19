import matplotlib.pyplot as plt
import numpy as np
import serial
import time

baudrate = 115200
com_port = 'COM15'

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

#an comport senden
f_out = [int(x[i]+x[i+1]*16) for i in np.arange(0, np.size(x), 2)]


data_bytes = [i.to_bytes(1, 'little') for i in f_out]

device = serial.Serial(com_port, baudrate, writeTimeout=0)

device.reset_output_buffer()

t_start = time.time()

buf = 0

for b in range(len(data_bytes)):
    buf = buf+1
    device.write(data_bytes[b])
    if buf%1 == 0:
       device.flush()
       
device.write(b'fertig')
device.flush()    
print('elapsed time: {:.3f}'.format(time.time() - t_start))

#t_start = time.time()
#print(device.write(bytes(f_out)))
#print('waiting:', device.out_waiting)
#device.flush()
#print('elapsed time: {:.3f}'.format(time.time() - t_start))

#time.sleep(2)
device.close()