import serial
import time


baudrate = 115200
com_port = 'COM15'

device = serial.Serial(com_port, baudrate, writeTimeout=0)
#device.reset_output_buffer()
#device.reset_input_buffer()


loc = '0'
fach = 'WsComm1Engl-v1'
dozent = 'MAH'

t_start = time.time()

device.write(bytes(loc+'\0', 'utf-8'))
device.write(bytes(fach+'\0', 'utf-8'))
device.write(bytes(dozent+'\0', 'utf-8'))
device.flush() 

 
print('elapsed time: {:.3f}'.format(time.time() - t_start))

device.close()