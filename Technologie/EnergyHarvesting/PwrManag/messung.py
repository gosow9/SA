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
       
    v1 = VISA_Device('ASRL1::INSTR')
    v2 = VISA_Device('USB0::0x2A8D::0x0101::MY57502226::INSTR')
    v3 = VISA_Device('USB0::0x2A8D::0x0101::MY57502216::INSTR')

    print(v1.connect())
    print(v2.connect())
    print(v3.connect())
    print(v1.id())
    print(v2.id())
    print(v3.id())
      
    print(v1._inst.write('READ?'))
    #print(v2._inst.query('READ?'))
    #print(v3._inst.query('READ?'))
    
    
    