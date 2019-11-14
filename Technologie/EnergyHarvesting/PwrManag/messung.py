# visa_device.py
# 29.3.2017, N. Ramagnano

'''
    Description
    -----------
    This class implements the minimal functionality of a VISA device.
'''

from __future__ import division

try:
    import visa
except ImportError:
    print('visa not found')
    exit(-1)

class VISA_Device:
    
    # verbose level
    # 0: off
    # 1: errors only
    # 2: errors + warnings
    # 3: errors + warnings + general information
    verbose = 3
    
    # --------------------------------------------------------------------------
    def __init__(self, resource_name):
        self._resource_name = resource_name
        self._rm = visa.ResourceManager()
        self._inst = None
        print(self._rm.list_resources())
  
    # --------------------------------------------------------------------------
    def print_error(self, message):
        if self.verbose >= 1:
            print('ERROR [%s]: %s' % (self._resource_name, message))
    
    # --------------------------------------------------------------------------
    def print_warning(self, message):
        if self.verbose >= 2:
            print('WARNING [%s]: %s' % (self._resource_name, message))
    
    # --------------------------------------------------------------------------
    def print_info(self, message):
        if self.verbose >= 3:
            print('INFO [%s]: %s' % (self._resource_name, message))
    
    # --------------------------------------------------------------------------
    def is_connected(self):
        if self._inst is not None:
            return True
        return False
    
    # --------------------------------------------------------------------------
    def connect(self):
        '''
            This function tries to open the resource with the given name.
        '''
        if not self.is_connected():
            try:
                self._inst = self._rm.open_resource(self._resource_name)
                self.id() # try to communicate with the device
            except:
                self.print_error('could not connect the device')
                self._inst = None
                return False
            else:
                self.print_info('device connected')
        else:
            self.print_warning('device already connected')
            
        return True
    
    # --------------------------------------------------------------------------
    def disconnect(self):
        if self.is_connected():
            self._inst.close()
            self._inst = None
            self.print_info('device disconnected')
        else:
            self.print_warning('device already disconnected')
            
    
    # --------------------------------------------------------------------------
    def id(self):
        if self.is_connected():
            return self._inst.query('*IDN?').strip()
        return None
    
if __name__ == '__main__':
    
    from pylab import *
    import time
    
    import serial
    import struct
    import datetime
    
    #lux-Messung
    baudrate = 9600 #whatever baudrate you are listening to
    com_port2 = 'COM16' #replace with your com port path

    sniffer_PCE = serial.Serial(com_port2, baudrate, timeout=1)
    sniffer_PCE.reset_input_buffer()

    #Spannung
    v1 = VISA_Device('USB0::0x0AAD::0x00FF::019137691::INSTR')
    v2 = VISA_Device('USB0::0x2A8D::0x0101::MY57502226::INSTR')
    v3 = VISA_Device('USB0::0x2A8D::0x0101::MY57502216::INSTR')

    print(v1.connect())
    print(v2.connect())
    print(v3.connect())
    print(v1.id())
    print(v2.id())
    print(v3.id())
    
    v1._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    v1._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    v1._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    v1._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')
    
    v2._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    v2._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    v2._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    v2._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')
    
    v3._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    v3._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    v3._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    v3._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')
    
    nsync = 0
    file = open('entladeverhalten1.txt', 'w')
    file.write('t, t_rel, V_sys, V_bat, V_in, lux\n')
    t_ref = time.time()
    while True:
        try:
            t = time.time()
            
            sniffer_PCE.write(serial.to_bytes('\x11'.encode('utf-8')))
            data_PCE = sniffer_PCE.read(19); 

            lux = data_PCE[10]*100 + data_PCE[11]
            
            V_sys = float(v1._inst.query('READ?'))
            V_bat = float(v2._inst.query('READ?'))
            V_in = float(v3._inst.query('READ?'))
                       
            s = '{}, {:f}, {:f}, {:f}, {:f}, {:f}\n'.format(str(datetime.datetime.now()), t-t_ref, V_sys, V_bat, V_in, lux)
            file.write(s)
            print(s)
        
            #time.sleep(0.2)
            
            
        except (KeyboardInterrupt, SystemExit):
            print('interrupted')
            break
    file.close()
    
    v1.disconnect()
    v2.disconnect()
    v3.disconnect()
    