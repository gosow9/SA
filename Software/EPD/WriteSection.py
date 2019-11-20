import serial
import time


baudrate = 115200
com_port = 'COM14'

device = serial.Serial(com_port, baudrate, writeTimeout=0)

#[[37, '\0', '\0\0\0\0']]=show
#data = [[1, 'xyz1\0', 'abc1\0'], [2, 'xyz2\0', 'abc2\0'], [3, 'xyz3\0', 'abc3\0'], [4, 'xyz4\0', 'abc4\0'], [5, 'xyz5\0', 'abc5\0'],
#        [6, 'xyz6\0', 'abc6\0'], [7, 'xyz7\0', 'abc7\0'], [8, 'xyz8\0', 'abc8\0'], [9, 'xyz9\0', 'abc9\0'], [10, 'xyz10\0', 'bc10\0'],
#        [11, 'xyz1\0', 'bc11\0'], [12, 'xyz2\0', 'bc12\0'], [13, 'xyz3\0', 'bc13\0'], [14, 'xyz4\0', 'bc14\0'], [15, 'xyz5\0', 'bc15\0'],
#        [16, 'xyz6\0', 'bc16\0'], [17, 'xyz7\0', 'bc7\0'], [18, 'xyz8\0', 'bc8\0'], [19, 'xyz9\0', 'bc19\0'], [20, 'xyz0\0', 'bc20\0'],
#        [21, 'xyz1\0', 'bc21\0'], [22, 'xyz2\0', 'bc2\0'], [23, 'xyz3\0', 'bc3\0'], [24, 'xyz4\0', 'bc24\0'], [25, 'xyz5\0', 'bc25\0'],
#        [26, 'xyz6\0', 'bc26\0'], [27, 'xyz7\0', 'bc7\0'], [28, 'xyz8\0', 'bc8\0'], [29, 'xyz9\0', 'bc29\0'], [30, 'xyz0\0', 'bc30\0'],
#        [31, 'xyz1\0', 'bc31\0'], [32, 'xyz2\0', 'bc2\0'], [33, 'xyz3\0', 'bc3\0'], [34, 'xyz4\0', 'bc34\0'], [35, 'xyz5\0', 'bc35\0']]

data = [[1, 'xyz1\0', 'abc1\0']]
#data = [[37, '\0', '\0\0\0\0']]

#template laden
#data =[[0, '\0', '\0\0\0\0']]

#mit \0 auffüllen
for i in range(len(data)):
    if len(data[i][1])<16:
        t = 14-len(data[i][1])
        data[i][1] = data[i][1]+'\0'*t


#print(device.read(1))
        
        
#data senden
t_start = time.time()

for i in range(len(data)):
    device.write([data[i][0]])
    device.write(bytes(data[i][1], 'utf-8'))
    device.write(bytes(data[i][2]+'\n', 'utf-8'))
    device.flush()
    time.sleep(1)

 
print('elapsed time: {:.3f}'.format(time.time() - t_start))

#print(device.read(16))

device.close()