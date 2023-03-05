//Library to control the button for toggling GUI/Joystick mode
#include <avdweb_Switch.h>
//Joystick X and Y axis variable initialization
int xValue = 0 ;
int yValue = 0 ; 
int bValue = 0 ;
//Setting Pin Config for 3 buttons(Reset/continious-step/Joystick-GUI mode)
const int buttonPinReset = 2;
const int buttonPinForContinousMode = 3;
const int buttonPinForControlMode = 12;

//Pin Configurations for Vertical and Horizontal motors
int driverDirVertical = 6;
int driverPulVertical = 7;
int driverDirHorizontal = 8;
int driverPulHorizontal = 9;

// Stepper Motor Parameters

//pulse per revolution = steps per revolution
int PPROfVerticalMotor = 6400; 
//pulse per revolution = steps per revolution
int PPROfHorizontalMotor = 6400;
//angle in degrees
int requiredAngleForVerticalMotor = 45;
int requiredAngleForHorizontalMotor = 45;

//Current Position of Motor
int currentPosOfVerticalMotorInSteps = 0;
int currentPosOfHorizontalMotorInSteps = 0;
int currentPosOfVerticalMotorInDegrees = 0;
int currentPosOfHorizontalMotorInDegrees = 0;

// 24v Proximity sensor varibale declartion
int sensorState1;
int sensorState2;
int sensorState3;
int sensorState4;

//Pin configurations for 4 24v Sensors
int sensorOnePin = 10;
int sensorTwoPin = 11;
int sensorThreePin = 4;
int sensorFourPin = 5;

//Stepper Motor default direction init
boolean setDir = HIGH;  
boolean setDirUp = HIGH;
boolean setDirDown = LOW;
boolean setDirLeft = HIGH;
boolean setDirRight = LOW;

# define DEBUG = false
//RESET flag to identify normal or reset stage
bool RESET = false;
// To toggle between step change or continious change of motor
bool CONTINIOUS_MODE = false;
// To change between GUI and Hardware control mode (mode = 0 (GUI), mode = 1(Joystick))
bool mode = 1;
#define PUSHBUTTON 12
//Phase delay for Stepper motor
int pd = 200;

// Center Position by default
int controllerPositionCode = 0;

// State variables init to handle continous press of joystick
int currentState = 10;
int previousState = 20;

//GUI/Joystick button init
Switch button = Switch(PUSHBUTTON);

void setup()  
{ 
  pinMode(buttonPinReset, INPUT);
  pinMode(buttonPinForContinousMode, INPUT);

  pinMode(sensorOnePin, INPUT);
  pinMode(sensorTwoPin, INPUT);
  pinMode(sensorThreePin, INPUT);
  pinMode(sensorFourPin, INPUT);
  
  pinMode(driverPulVertical,OUTPUT);
  pinMode(driverDirVertical,OUTPUT);
  pinMode(driverPulHorizontal,OUTPUT);
  pinMode(driverDirHorizontal,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //Interupts for reset and continous/step mode change functionality
  attachInterrupt(digitalPinToInterrupt(buttonPinReset),initiateReset,CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPinForContinousMode),toggleContiniousMode,FALLING);

  Serial.begin(9600) ;
} 

void readSerial(){
  /*
    This function is used to establish connection between Arduino and
    PC in GUI mode.
  */
  if (Serial.available() > 0 )
  {
          // Read serial string from PC
          String msg = Serial.readString();

          if (msg == "LEFT")
          {
            Serial.println("LEFT");
            controllerPositionCode = 1;
          }

          else if(msg == "RIGHT")
          {
            Serial.println("RIGHT");
            controllerPositionCode = 2;

          }

          else if(msg == "UP") 
          {
            Serial.println("UP");
            controllerPositionCode = 3;
          }

          else if(msg == "DOWN") 
          {
              Serial.println("DOWN");
              controllerPositionCode = 4;
          }

          else if(msg == "RESET")
          {
              Serial.println("RESET");
              controllerPositionCode = 5;
              initiateReset();
          }

          else if(msg == "GUI") 
          { 
              // GUI Mode change
              Serial.println("GUI Mode Set");
              mode = 0;
          
          }

          else if(msg == "HW") 
          {
            // HW or Joystick mode
              Serial.println("Joystick Mode Set");
              mode = 1;
          }

          else if(msg == "continous") 
          {
            // Continous mode change
              Serial.println("Continious Rotation Mode Set");
              CONTINIOUS_MODE = true;
          }
          else if(msg == "step") 
          {
              // Step Mode change
              Serial.println("Step Rotation Mode Set");
              CONTINIOUS_MODE = false;
          }

          else 
          {
            //Any other command apart from above-mentioned ones
            Serial.println("Wrong Command");
            controllerPositionCode = 6;
          }

          // Call the driver motor action function
           programFlow(controllerPositionCode);
  
    
  }

  else 
  {
      Serial.println("serial not available");
  }
  
  }


bool safeToProceed()
{
  /*
  This function checks the safety condition for motor action.
  No motor action should be taken unless sensors black weights
  are at rest position. The position is given my proximity senor.
  */
  sensorState2 = readSensor2();
  sensorState4 = readSensor4();
  if (sensorState2 == HIGH && sensorState4 == HIGH )
    {
      return true;
    }
  else 
    {
      return false;
    }
}

int calculateRotationSteps(int ppr,int requiredDegreeRotation)
{
  // Calculate the rotation steps based on motor configurations
  float oneStepToDegrees = (float(360)/float(6400));  
  int numberOfStepsForRequiredDegreeRotation = (requiredDegreeRotation/oneStepToDegrees);
  Serial.println("numberOfStepsForRequiredDegreeRotation");
  Serial.println(numberOfStepsForRequiredDegreeRotation);
  return numberOfStepsForRequiredDegreeRotation;
  
  
}


void revMotor()
{
  // Reverse motor direction
  setDir = !setDir;
}


void resetVerticalMotor()
{
  //Triggered when reset button is pressed to reset Vertically
  // rotating motor
  
  while(digitalRead(sensorOnePin) != HIGH)
    {
       rotateVerticalMotor(setDirUp);
    
    }
  
}

void resetHorizontalMotor()
{
  //Triggered when reset button is pressed to reset Horizontally
  // rotating motor
  while(digitalRead(sensorThreePin)!= HIGH)
  {
   
    rotateHorizontalMotor(setDirLeft);
         
  }
  
}

void initiateReset()
{
  // Triggered when reset button is pressed. Sets RESET flag. 
  if (safeToProceed() == true)
  {
    RESET = true;
  }
}

void toggleContiniousMode()
{
  // Toggle continous mode flag
    CONTINIOUS_MODE = !CONTINIOUS_MODE;
}
  
void  resetMotorPosition()
{
  resetVerticalMotor();
  resetHorizontalMotor();
  
}

void rotateVerticalMotor(bool dir)
{
  digitalWrite(driverDirVertical,dir);
  digitalWrite(driverPulVertical,HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPulVertical,LOW);
  delayMicroseconds(pd);
  
}

void rotateHorizontalMotor(bool dir)
{
  digitalWrite(driverDirHorizontal,dir);
  digitalWrite(driverPulHorizontal,HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPulHorizontal,LOW);
  delayMicroseconds(pd);
  
}

void rotateVerticalMotorXDegrees(bool dir,int numberOfSteps)
{
  // Utilized in Step mode. Rotates vertical motor X degrees.
  // X can be configured.
  digitalWrite(driverDirVertical,dir);
  for (int n = 0; n < numberOfSteps;n++ )
  {
      digitalWrite(driverPulVertical,HIGH);
      delayMicroseconds(pd);
      digitalWrite(driverPulVertical,LOW);
      delayMicroseconds(pd);
  }
  
}

void rotateHorizontalMotorXDegrees(bool dir,int numberOfSteps)
{
  // Utilized in Step mode. Rotates horizontal motor X degrees.
  // X can be configured.
  digitalWrite(driverDirHorizontal,dir);
  for (int n = 0; n < numberOfSteps;n++ )
  {
      
      digitalWrite(driverPulHorizontal,HIGH);
      delayMicroseconds(pd);
      digitalWrite(driverPulHorizontal,LOW);
      delayMicroseconds(pd);
  }
  
}


int readSensor1()
{
    // Vertical Sensor
    int sensorValue = digitalRead(sensorOnePin);
    return sensorValue;

}

int readSensor2()
{
  //Green - 2 Vertical Precondition
  int sensorValue = digitalRead(sensorTwoPin);
  return sensorValue;

}

int readSensor3()
{
    // Rotation Sensor
    int sensorValue = digitalRead(sensorThreePin);
    return sensorValue;
}

  
  int readSensor4()
  {
    //Green - 4 Horizontal Precondition   
    int sensorValue = digitalRead(sensorFourPin);
    return sensorValue;

  }
  
 void programFlow(int controllerPositionCode)
 {
    // Drive motor functionality
    if (safeToProceed() == true)
    {
      if (CONTINIOUS_MODE == false)
      {
        driveMotor(controllerPositionCode);
      }
      else if(CONTINIOUS_MODE == true) 
      {
        driveMotorInContiniousMode(controllerPositionCode);
        
      }
    }
  }
void decideDirection(int xValue,int yValue)
{
  // This function decides the direction of the joystick
  // using xVlaue(X-axis) and yValue(Y-axis)
  if(xValue >=550 && xValue <=800 )
  {
          if(yValue  >=380 && yValue <=550)
          {
                controllerPositionCode = 1;
          } 
  }
  else if(xValue >=200 && xValue <300 )
  {
          if(yValue >=380 && yValue <=550)
          {       
                controllerPositionCode = 2;
          } 
  }
   
  else if(yValue >=200 && yValue <300 )
  {
          if(xValue >=488 && xValue <=550)
          {  
                controllerPositionCode = 3;
          } 
  }

  else if(yValue >=550 && yValue <=800 )
  { 
          if(xValue >=490 && xValue <=550)
          {
                controllerPositionCode = 4;
          } 
   }

  else
  {
        controllerPositionCode = 0;
    }
    
  if (CONTINIOUS_MODE == false) 
  {
        currentState = controllerPositionCode;
        if (currentState != previousState)
        {
          programFlow(controllerPositionCode);
        }
        else if(currentState == 0)
        {
           Serial.print("\n");
        }
        else
        {
           Serial.println("Dont hold back the joystick\n");
        }
        previousState = currentState;
  }

  else {
    programFlow(controllerPositionCode);
    
    
    }
    
}

void readCommand(){
  // Read command from Joystick
  xValue = analogRead(A0);  
  yValue = analogRead(A1);
  decideDirection(xValue,yValue);
  } 


void driveHorizontalMotorLeft()
{
  bool dir = setDirLeft;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps += steps; 
  rotateHorizontalMotorXDegrees(dir,steps);
  
}

void driveHorizontalMotorRight()
{
  bool dir = setDirRight;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps -= steps;
  rotateHorizontalMotorXDegrees(dir,steps);
}

void driveVerticalMotorUp()
{
  bool dir = setDirUp;
  int steps = calculateRotationSteps(PPROfVerticalMotor,requiredAngleForVerticalMotor);
  currentPosOfVerticalMotorInSteps += steps; 
  rotateVerticalMotorXDegrees(dir,steps);
}
  
void driveVerticalMotorDown()
{
  bool dir = setDirDown;
  int steps = calculateRotationSteps(PPROfVerticalMotor,requiredAngleForVerticalMotor);
  currentPosOfVerticalMotorInSteps -= steps; 
  rotateVerticalMotorXDegrees(dir,steps);
}


void driveMotor(int controllerPositionCode){
  /* 
    Control block function for calling corresponding function
    for driving the motor in step mode.
  */
  switch (controllerPositionCode){
      case 1:
        driveHorizontalMotorLeft();
        break;
      case 2:
          driveHorizontalMotorRight();
          break;
      case 3:
          driveVerticalMotorUp();
          break;
      case 4:
          driveVerticalMotorDown();
          break;
      case 0:
        Serial.print("No Action \n");
        break;
      default:
        Serial.print("No Action \n");
        break;
  }
  }

void driveMotorInContiniousMode(int controllerPositionCode){
  /* 
    Control block function for calling corresponding function
    for driving the motor in continious mode.
  */
  switch (controllerPositionCode){
      case 1:
        rotateHorizontalMotor(setDirLeft);
        break;
      case 2:
          rotateHorizontalMotor(setDirRight);
          break;
      case 3:
          rotateVerticalMotor(setDirUp);
          break;
      case 4:
          rotateVerticalMotor(setDirDown);
          break;
      case 0:
        Serial.print("No Action \n");
        break;
      default:
        Serial.print("No Action \n");
        break;
  }
  }

boolean checkDirectionLeft(int xValue, int yValue)
{
    if ((xValue >=550 && xValue <=800 ) && (yValue >=380 && yValue <=550)) 
    {

      return true;
    }
    else
    {
      return false;
    }
}

boolean checkDirectionRight(int xValue, int yValue)
{
  if((xValue >=200 && xValue <300)&& (yValue >=380 && yValue <=550)) 
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean checkDirectionUp(int xValue, int yValue){
  if((yValue >=200 && yValue <300 )&& (xValue >=488 && xValue <=550)) 
  {
          return true;          
          
  }

  else
  {
        return false;
  }
}

boolean checkDirectionDown(int xValue, int yValue)
{
  if((yValue >=550 && yValue <=800 )&& (xValue >=490 && xValue <=550)) 
  {
        return true;
  }
  else
  {
      return false;
  }
}


void controlMotorThroughJoystick()
{
  /* This function rotates the vertical and horizontal motor
  based joystick input. 

  Joystick Input:
    Left: Rotate Horizontal Motor Left
    Right: Rotate Horizontal Motor Right
    Up: Rotate Vertical Motor Up
    Down: Rotate Vertical Motor Down
  */
  xValue = analogRead(A0);  
  yValue = analogRead(A1);
  if (checkDirectionLeft(xValue,yValue) == true)
  {
    if(digitalRead(sensorThreePin)!= HIGH)
    {
          rotateHorizontalMotor(HIGH);
    }
  }

    else if (checkDirectionRight(xValue,yValue) == true)
    {
      if(digitalRead(sensorThreePin)!= HIGH)
      {
          rotateHorizontalMotor(LOW);
      }
    }

      else if (checkDirectionUp(xValue,yValue) == true)
      {
        if(digitalRead(sensorOnePin) != HIGH)
        {
            rotateVerticalMotor(HIGH);
        }
      }
  else if (checkDirectionDown(xValue,yValue) == true)
  {
    if(digitalRead(sensorOnePin) != HIGH)
    {
        rotateVerticalMotor(LOW);
    }
  }
}

void loop() 
{ 
  button.poll();
  if(button.pushed()) 
  {
    mode = !mode;
  }
  if (RESET == true)
  {
      // RESET 
      resetMotorPosition();
      RESET = false;
  }
  if (mode == 0)
  {
    //GUI Mode
    readSerial();
  }
  else if(mode == 1)
  {
    if (CONTINIOUS_MODE == false)
    {
      readCommand();
      delay(100);
    }
    else
    {
      if (safeToProceed() == true) 
      {
        controlMotorThroughJoystick();
      }
    }
  }
  
}