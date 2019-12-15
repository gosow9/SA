import serial
import time

baudrate = 115200
com_port = 'COM14'

device = serial.Serial(com_port, baudrate, writeTimeout=0)

#[location, fach(13 lang + \0), dozent (3 lang +\0)]
data = [[0, chr(4), '\0\0\0\0'],[10 , 'WsComm1\0', 'MAH\0'], [20 , 'WsComm1\0', 'MAH\0'],
        [30 , 'WsComm1\0', 'MAH\0'],[35 , 'WsComm1\0', 'MAH\0']]

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
    #time.sleep(1)

print('elapsed time: {:.3f}'.format(time.time() - t_start))

#print(device.read(16))

device.close()