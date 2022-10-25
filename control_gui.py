import sys
sys.path.append("/usr/local/lib/python3.7/site-packages/")
from tkinter import *
from softwareControl import control
from functools import partial


class controlGUI():

    def __init__(self):
        self.root = Tk()
        self.obj = control()
        self.root.geometry("450x350")
        self.setLabel()
        self.mode = "GUI"
    
    def setLabel(self):
        self.label = Label(self.root,text = "Control Ready")
        self.label.place(x=160, y=270)

    def setGUILabel(self):
        self.toggle.config(text = "HW")
    def moveUP(self):
        self.label.config(text = "MOVE UP")
        self.obj.sendCommand("UP")
        
    def moveDown(self):
        self.label.config(text = "MOVE DOWN")
        self.obj.sendCommand("DOWN")

    def rotateLeft(self):
        self.label.config(text = "ROTATE LEFT")
        self.obj.sendCommand("LEFT")

    def rotateRight(self):
        self.label.config(text = "ROTATE RIGHT")
        self.obj.sendCommand("RIGHT")

    def reset(self):
        self.label.config(text = "RESET")
        self.obj.sendCommand("RESET")
    
    def changeMode(self):
        if self.mode == "GUI":
            self.label.config(text = "Joystick Mode",fg = "black")
            self.obj.sendCommand("HW")
            self.mode = "HW"
        else:
            self.label.config(text = "GUI Mode ",fg = "green")
            self.obj.sendCommand("GUI")
            self.mode = "GUI"

    def defineHeader(self):
        label = Label(self.root,text = "ATOM PROBE TOMOGRAPHY",font='Helvetica 18 bold')
        label.pack(pady=20)

    def defineSubHeader(self):
        label = Label(self.root,text = "Welcome to the Control Tool",font='Helvetica 18 bold')
        label.pack(pady=10)
    

def main():
    gui = controlGUI()
    gui.defineHeader()
    gui.defineSubHeader()
    
    button1 = Button(gui.root,text = "Move\nUp",command = gui.moveUP)
    button2 = Button(gui.root,text = "Move\nDown",command = gui.moveDown)
    button3 = Button(gui.root,text = "Rotate\nLeft" ,command = gui.rotateLeft)
    button4 = Button(gui.root,text = "Rotate\nRight" ,command = gui.rotateRight)
    button5 = Button(gui.root,text = "RESET" ,command = gui.reset)
    on_button = Button(gui.root, text = "Change\n Mode", bd = 0,
                   command = gui.changeMode)
    on_button.place(x= 380,y= 125)
    button1.place(x=180, y=125)
    button2.place(x=180, y=220)
    button3.place(x=115, y=175)
    button4.place(x=238, y=175)
    button5.place(x=180, y=180)
    gui.root.mainloop()

if __name__ == "__main__":
    main()



