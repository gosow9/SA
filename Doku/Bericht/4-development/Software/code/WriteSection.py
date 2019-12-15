import serial
import time


baudrate = 115200
com_port = 'COM14'

device = serial.Serial(com_port, baudrate, writeTimeout=0)


data = [[1, 'xyz1\0', 'abc1\0']]

for i in range(len(data)):
    if len(data[i][1])<16:
        t = 14-len(data[i][1])
        data[i][1] = data[i][1]+'\0'*t
        
t_start = time.time()

for i in range(len(data)):
    device.write([data[i][0]])
    device.write(bytes(data[i][1], 'utf-8'))
    device.write(bytes(data[i][2]+'\n', 'utf-8'))
    device.flush()
    time.sleep(1)

 
print('elapsed time: {:.3f}'.format(time.time() - t_start))


device.close()