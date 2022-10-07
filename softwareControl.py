import sys
sys.path.append("/usr/local/lib/python3.7/site-packages/")
import serial.tools.list_ports

class control:

    def __init__(self) -> None:
        self.ports = serial.tools.list_ports.comports()
        self.serialInst = serial.Serial()
        self.portsList = []
        self.setup()

    def printAllPorts(self):
        for onePort in self.ports:
            self.portsList.append(str(onePort))
            print(str(onePort))

    def setup(self):
        self.serialInst.baudrate = 9600
        self.serialInst.port = "/dev/cu.usbmodem141201"
        self.serialInst.open()

    def sendCommand(self,command):
        print("COMMAND SENT: ",command)
        #self.serialInst.write(command.encode('utf-8'))
