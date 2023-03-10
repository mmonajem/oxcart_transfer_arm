import sys
sys.path.append("/usr/local/lib/python3.7/site-packages/")
import serial.tools.list_ports

class control:
    '''
        This class is responsible for providing helping functions
        to communicate between the PC and Arduino over serial when
        the system is set in GUI mode.
    '''

    def __init__(self) -> None:
        self.ports = serial.tools.list_ports.comports()
        self.serialInst = serial.Serial()
        self.portsList = []
        self.setup()

    def printAllPorts(self):
        '''
            This function prints all the ports of the 
            PC for serial communication between arduino
            and PC.
            Args:
                Does not accept any arguments 
            Returns:
                Does not return anything
        '''
        for onePort in self.ports:
            self.portsList.append(str(onePort))
            print(str(onePort))

    def setup(self):
        '''
            This function setups the serial communication 
            parameter like baudrate and portId

            TODO: automate port selection instead of static definition

            Args:
                Does not accept any arguments 
            Returns:
                Does not return anything
        '''
        self.serialInst.baudrate = 9600
        self.serialInst.port = "/dev/cu.usbmodem141201"
        self.serialInst.open()

    def sendCommand(self,command):
        '''
            This function is responsible for writing/Sending
            the command from the GUI(PC) to the arduino through
            serial write.
            Args:
                command: command to be written onto serial (type: string)
            Return:
                Does not return anaything
            Format:
                command: RESTART 
        '''
        print("COMMAND SENT: ",command)
        self.serialInst.write(command.encode('utf-8'))
