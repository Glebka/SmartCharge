import serial
import sys
from struct import *

CmdFormat = "<BBxx"

class Device(object):

    def __init__(self, port, baud):
        super(Device, self)
        self._ser = serial.Serial()
        self._ser.port = port
        self._ser.baudrate = baud
        self._ser.open()
        self._ser.timeout = 3

    def read_all(self):
        print("Resp: {}".format(repr(self._ser.read_all())))

    def get_version(self):
        self._ser.write(pack(CmdFormat, 5, 0))
        print("Resp: {}".format(repr(self._ser.read(size=4))))
    
    def get_state(self):
        self._ser.write(pack(CmdFormat, 6, 0))
        print("Resp: {}".format(repr(self._ser.read(size=4))))
        print("State: {}".format(repr(self._ser.read(size=4))))
    
    def charge_enable(self):
        self._ser.write(pack(CmdFormat, 1, 0))
        print("Resp: {}".format(repr(self._ser.read(size=4))))
    
    def charge_disable(self):
        self._ser.write(pack(CmdFormat, 2, 0))
        print("Resp: {}".format(repr(self._ser.read(size=4))))
    
    def close(self):
        self._ser.close()
    

print("Use dev object to interact with device.")
print("Read initial message from device: dev.read_all()")
print("Example: dev.get_version()")
print("Type exit() in console when done.")
print("----")
print("Opening {} with baud {}...".format(sys.argv[1], sys.argv[2]))

dev = Device(sys.argv[1], sys.argv[2])
