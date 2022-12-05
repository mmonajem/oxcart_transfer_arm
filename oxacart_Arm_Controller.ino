#include <avdweb_Switch.h>
int xValue = 0 ;
int yValue = 0 ; 
int bValue = 0 ;
const int buttonPinReset = 2;
const int buttonPinForContinousMode = 3;
const int buttonPinForControlMode = 12;

//Motor Configurations
int driverDirVertical = 6;
int driverPulVertical = 7;

int driverDirHorizontal = 8;
int driverPulHorizontal = 9;

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

int sensorState1;
int sensorState2;
int sensorState3;
int sensorState4;

int sensorOnePin = 10;
int sensorTwoPin = 11;
int sensorThreePin = 4;
int sensorFourPin = 5;

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
// To change between GUI and Hardware control mode
bool mode = 1;
#define PUSHBUTTON 12

int pd = 200;

// Center Position by default
int controllerPositionCode = 0;

int millisbetweenSteps = 50;

int currentState = 10;
int previousState = 20;

Switch button = Switch(PUSHBUTTON);

void setup()  
{ 
  
  //pinMode(8,INPUT); 
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

  attachInterrupt(digitalPinToInterrupt(buttonPinReset),initiateReset,CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPinForContinousMode),toggleContiniousMode,FALLING);

  Serial.begin(9600) ;
} 

void readSerial(){
  
  if (Serial.available() > 0 )
  {

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
                Serial.println("GUI Mode Set");
                mode = 0;
            
            }

            else if(msg == "HW") 
            {
                Serial.println("Joystick Mode Set");
                mode = 1;
            }

            else if(msg == "continous") 
            {
                Serial.println("Continious Rotation Mode Set");
                CONTINIOUS_MODE = true;
            }
            else if(msg == "step") 
            {
                Serial.println("Step Rotation Mode Set");
                CONTINIOUS_MODE = false;
            }

            else 
            {
              Serial.println("Wrong Command");
              controllerPositionCode = 6;
            }

           programFlow(controllerPositionCode);
  
    
  }

  else 
  {
      Serial.println("serial not available");
  }
  
  }


bool safeToProceed()
{
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
  float oneStepToDegrees = (float(360)/float(6400));  
  int numberOfStepsForRequiredDegreeRotation = (requiredDegreeRotation/oneStepToDegrees);
  Serial.println("numberOfStepsForRequiredDegreeRotation");
  Serial.println(numberOfStepsForRequiredDegreeRotation);
  return numberOfStepsForRequiredDegreeRotation;
  
  
}

void revMotor()
{
  setDir = !setDir;
}


void resetVerticalMotor()
{
  while(digitalRead(sensorOnePin) != HIGH)
    {
       rotateVerticalMotor(setDirUp);
    
    }
  
}

void resetHorizontalMotor()
{
  while(digitalRead(sensorThreePin)!= HIGH)
  {
   
    rotateHorizontalMotor(setDirLeft);
         
  }
  
}

void initiateReset()
{
  if (safeToProceed() == true)
  {
    RESET = true;
  }
}

void toggleContiniousMode()
{
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
    if(sensorValue==LOW)
    { 
      // if (DEBUG == true) 
      // {
      //   Serial.println("No Object detected by Sensor 1");
      // }
    }
    else
    {
      // if (DEBUG == true) 
      // {
      //   Serial.println("Object Detected by Sensor 1");
      // }
    }
    return sensorValue;

}

int readSensor2()
{
  //Green - 2 Vertical Precondition
  int sensorValue = digitalRead(sensorTwoPin);
  if(sensorValue==LOW) 
  {  
    //Serial.println("No Object detected by Sensor 2");
  }
  else
  {
    //Serial.println("Object Detected by Sensor 2");
  }
  return sensorValue;

}

int readSensor3()
{
    // Rotation Sensor
    int sensorValue = digitalRead(sensorThreePin);
    if(sensorValue==LOW)
    {  
      // if (DEBUG == true) 
      // {
      //   Serial.println("No Object detected by Sensor 3");
      // }
    }
    else
    { 
      // if (DEBUG == true)
      // {
      //   Serial.println("Object Detected by Sensor 3");
      // }
    }
    return sensorValue;
}

  
  int readSensor4()
  {
    //Green - 4 Horizontal Precondition   
    int sensorValue = digitalRead(sensorFourPin);
    if(sensorValue==LOW)
    {  
      // if (DEBUG == true) 
      // {
      //   Serial.println("No Object detected by Sensor 4");
      // }
    }
    else
    {
      // if (DEBUG == true) 
      // {
      
      //   Serial.println("Object Detected by Sensor 4");
      // }
    }
    return sensorValue;

  }
  

 void programFlow(int controllerPositionCode)
 {
    //confirm which sensors are initial state sensors
   
    if (safeToProceed() == true)
    {
      // if (DEBUG == true) 
      // {
      //   Serial.println("Drive Motor");
      // }
      if (CONTINIOUS_MODE == false)
      {
        driveMotor(controllerPositionCode);
      }

      else if(CONTINIOUS_MODE == true) 
      {
        driveMotorInContiniousMode(controllerPositionCode);
        
      }
    }
    else 
    {
      // if (DEBUG == true)
      // {
      //   Serial.println("Not Safe do not proceed");
      // }

    }

  
  
  }
void decideDirection(int xValue,int yValue)
{
  if(xValue >=550 && xValue <=800 )
  {
          if(yValue  >=380 && yValue <=550)
          {
                // if (DEBUG == true) 
                // {
                //   Serial.print("Rotate Left");
                //   Serial.print("\n");
                // }
                controllerPositionCode = 1;
          } 
  }
  else if(xValue >=200 && xValue <300 )
  {
          if(yValue >=380 && yValue <=550)
          {       
                // if (DEBUG == true) 
                // {
                //   Serial.print("Rotate Right");
                //   Serial.print("\n");
                  
                // }
                controllerPositionCode = 2;
          } 
  }
   
  else if(yValue >=200 && yValue <300 )
  {
          if(xValue >=488 && xValue <=550)
          {
                // if (DEBUG == true) 
                // {
                //   Serial.print("Go Up");
                //   Serial.print("\n");
                  
                // }
                controllerPositionCode = 3;
          } 
  }

  else if(yValue >=550 && yValue <=800 )
  { 
          if(xValue >=490 && xValue <=550)
          {
                // if (DEBUG == true) 
                // {
                //     Serial.println("Go Down");
                // }
                controllerPositionCode = 4;

      
          } 
   }

  else
  {
        // if (DEBUG == true) 
        // {
        //   Serial.println("Center Position");
          
        // }
        controllerPositionCode = 0;
    }
    
  if (CONTINIOUS_MODE == false) 
  {
        currentState = controllerPositionCode;
        if (currentState != previousState)
        {
          // if (DEBUG == true) 
          // {
          //   Serial.print("Call program flow\n");
          // }
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
    // if (DEBUG == true) 
    // {
    //   Serial.print("Call program flow in CM Mode \n");
    // }
    programFlow(controllerPositionCode);
    
    
    }
    
}

void readCommand(){
  xValue = analogRead(A0);  
  yValue = analogRead(A1);
  // if (DEBUG == true) 
  // { 
  //   Serial.println(xValue);
  //   Serial.println(yValue);
  // }
  decideDirection(xValue,yValue);
  } 


void driveHorizontalMotorLeft()
{
  // if (DEBUG == true) 
  // {
  //   Serial.println("Drive Motor Left");
  // }
  bool dir = setDirLeft;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps += steps; 
  rotateHorizontalMotorXDegrees(dir,steps);
  
}

void driveHorizontalMotorRight()
{
  // if (DEBUG == true) 
  // {
  //   Serial.println("Drive Motor RIGHT");
  // }
  bool dir = setDirRight;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps -= steps;
  rotateHorizontalMotorXDegrees(dir,steps);
}

void driveVerticalMotorUp()
{
  // if (DEBUG == true) 
  // {
  //   Serial.println("Drive Motor UP");
  // }
  bool dir = setDirUp;
  int steps = calculateRotationSteps(PPROfVerticalMotor,requiredAngleForVerticalMotor);
  Serial.println("steps:");
  Serial.println(steps);
  currentPosOfVerticalMotorInSteps += steps; 
  rotateVerticalMotorXDegrees(dir,steps);
}
  
void driveVerticalMotorDown()
{
  // if (DEBUG == true) 
  // {
  //   Serial.println("Drive Motor DOWN");
  // }
  bool dir = setDirDown;
  int steps = calculateRotationSteps(PPROfVerticalMotor,requiredAngleForVerticalMotor);
  currentPosOfVerticalMotorInSteps -= steps; 
  rotateVerticalMotorXDegrees(dir,steps);
}


void driveMotor(int controllerPositionCode){
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
        resetMotorPosition();
        RESET = false;
  }
  if (mode == 0)
  {
    
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