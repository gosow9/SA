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

# ==============================================================================
if __name__ == '__main__':
    
    from pylab import *
    import time
    
    dev_VPV = VISA_Device('USB0::0x0957::0x0607::MY53002139::INSTR')
    dev_VCAP = VISA_Device('USB0::0x0957::0x0607::MY53002139::INSTR')
    dev_VDD = VISA_Device('USB0::0x0957::0x0607::MY53002139::INSTR')

    print(dev_VPV.connect())
    print(dev_VPV.id())
    print(dev_VCAP.connect())
    print(dev_VCAP.id())
    print(dev_VDD.connect())
    print(dev_VDD.id())

    dev_VPV ._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    dev_VPV ._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    dev_VPV ._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    dev_VPV ._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')

    dev_VCAP._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    dev_VCAP._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    dev_VCAP._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    dev_VCAP._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')
    
    dev_VDD._inst.write('SYSTem:COMMunicate:ENABle ON,USB')
    dev_VDD._inst.write('SENSe:VOLTage:DC:IMPedance:AUTO ON')
    dev_VDD._inst.write('SENSe:VOLTage:DC:ZERO:AUTO ON')
    dev_VDD._inst.write('SENSe:VOLTage:DC:RANGE:AUTO OFF')

    nsync = 0
    file = open('data.csv', 'a')
    t_ref = time.time()
    while True:
        try:
            t = time.time()
            #~ data = float(dev._inst.query('MEASURE:VOLTage:DC?'))
            VPV = float(dev_VPV._inst.query('READ?'))
            VCAP = float(dev_VCAP._inst.query('READ?'))
            VDD = float(dev_VDD._inst.query('READ?'))
            #~ currentRange = float(devI._inst.query('SENSe:CURRent:DC:RANGe:UPPer?'))
            
            s = '%.3f,%.6e,%.6e,%.6e\n' % (t, VPV,VCAP,VDD)
            s_print = '%.3f,%.6f,%.6f,%.6f\n' % (t-t_ref, VPV,VCAP,VDD)
            file.write(s)
            if nsync < 120:
                nsync += 1
            else:
                file.flush()
                print('--- file flush ---') 
                nsync = 0
            print(s_print.strip())
            #~ time.sleep(0.1   )
        except (KeyboardInterrupt, SystemExit):
            print('interrupted')
            break
    file.close()
    
    dev_VPV.disconnect()
    dev_VCAP.disconnect()
    dev_VDD.disconnect()