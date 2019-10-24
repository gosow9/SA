import serial
import time
import struct
import datetime


baudrate = 9600 #whatever baudrate you are listening to
com_port2 = 'COM5' #replace with your com port path


sniffer_PCE = serial.Serial(com_port2, baudrate, timeout=1)
time.sleep(1)
sniffer_PCE.reset_input_buffer()
f = open('measuredData.txt', 'w')

while 1:
  try:
    sniffer_PCE.write(serial.to_bytes('\x11'.encode('utf-8')))
    data_PCE = sniffer_PCE.read(19); 
    #~ print(','.join([('%02X'%x) for x in data_PCE])+'\n')
    #~ print(data_PCE[10:12])
    data_PCE_int = data_PCE[10]*100 + data_PCE[11]
    print(data_PCE_int)
    #f.write('%d\n'%data_PCE_int)
    f.write('{:d}, {}\n'.format(data_PCE_int, str(datetime.datetime.now())))
    #~ lux = ''.join([('%02x'%X) for X in data_PCE[10:12]])
    #~ print(lux)
    time.sleep(1)
  except (KeyboardInterrupt, SystemExit):
      print('interrupted')
      exit()
      f.close()