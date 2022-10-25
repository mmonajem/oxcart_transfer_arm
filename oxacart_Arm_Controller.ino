int xValue = 0 ;
int yValue = 0 ; 
int bValue = 0 ;
const int Pin=2;

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
int sensorTwoPin = 3;
int sensorThreePin = 4;
int sensorFourPin = 5;

boolean setDir = HIGH;
boolean setDirUp = HIGH;
boolean setDirDown = LOW;
boolean setDirLeft = HIGH;
boolean setDirRight = LOW;

bool RESET = false;

bool mode = 1;


int pd = 200;

// Center Position by default
int controllerPositionCode = 0;

int millisbetweenSteps = 50;

//int numberOfSteps = 800;

int currentState = 10;
int previousState = 20;



void readSerial(){
  
  if (Serial.available() > 0 ){

          String msg = Serial.readString();

          if (msg == "LEFT"){
            Serial.println("LEFT");
              controllerPositionCode = 1;
            }

          else if(msg == "RIGHT") {
            Serial.println("RIGHT");
            controllerPositionCode = 2;

            }

            else if(msg == "UP") {
            Serial.println("UP");
            controllerPositionCode = 3;
            blinkLed();
            //driveVerticalMotorUp();
            }

            else if(msg == "DOWN") {
                Serial.println("DOWN");
                controllerPositionCode = 4;
            
            }

            else if(msg == "RESET") {
                Serial.println("RESET");
                controllerPositionCode = 5;
                //blinkLed();
                initiateReset();
            
            }

            else if(msg == "GUI") {
                Serial.println("GUI Mode Set");
                mode = 0;
                //blinkLed();
            
            }

            else if(msg == "HW") {
                Serial.println("Joystick Mode Set");
                mode = 1;
                //blinkLed();
                
            
            }

            else {
              Serial.println("Wrong Command");
              controllerPositionCode = 6;
              }

           programFlow(controllerPositionCode);
  
    
    }

    else {
      Serial.println("serial not available");
      }
  
  }
int calculateRotationSteps(int ppr,int requiredDegreeRotation)
{
  float oneStepToDegrees = (float(360)/float(6400));
  //Confirm this later. Int or round operation?
  
  int numberOfStepsForRequiredDegreeRotation = (requiredDegreeRotation/oneStepToDegrees);
  Serial.println("numberOfStepsForRequiredDegreeRotation");
  Serial.println(numberOfStepsForRequiredDegreeRotation);
  return numberOfStepsForRequiredDegreeRotation;
  
  
  }


void revMotor(){
  setDir = !setDir;
  }


void resetVerticalMotor(){
  while(digitalRead(sensorOnePin) != HIGH){
    //change this to 1 later
       rotateVerticalMotor(setDirUp);
    
    }
  
  }

 void resetHorizontalMotor(){
  while(digitalRead(sensorThreePin)!= HIGH)
  {
   
    rotateHorizontalMotor(setDirLeft);
         
  }
  
  }

 void initiateReset(){
  RESET = true;
  
  }
  
void  resetMotorPosition(){

  //Serial.println("reset called");
  resetVerticalMotor();
  resetHorizontalMotor();
  
  }

void rotateVerticalMotor(bool dir){
  digitalWrite(driverDirVertical,dir);
  digitalWrite(driverPulVertical,HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPulVertical,LOW);
  delayMicroseconds(pd);
  
  }

void rotateHorizontalMotor(bool dir){
  digitalWrite(driverDirHorizontal,dir);
  digitalWrite(driverPulHorizontal,HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPulHorizontal,LOW);
  delayMicroseconds(pd);
  
  }
void setup()  
{ 
  
  pinMode(8,INPUT); 
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(driverPulVertical,OUTPUT);
  pinMode(driverDirVertical,OUTPUT);
  pinMode(driverPulHorizontal,OUTPUT);
  pinMode(driverDirHorizontal,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2),initiateReset,RISING);
  digitalWrite(8,HIGH); 
  Serial.begin(9600) ;
} 

void printResults(int xValue,int yValue){
  Serial.print("X value:  ");
  Serial.print(xValue,DEC);
  Serial.print(",");
  Serial.print("Y value:  ");
  Serial.print(yValue,DEC);
  Serial.print(",");
  Serial.print(!bValue);
  Serial.print("\n");
  
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
      Serial.println("No Object detected by Sensor 1");
    }
    else
    {
      Serial.println("Object Detected by Sensor 1");
    }
    return sensorValue;

}

 int readSensor2()
 {
    //Green - 2 Vertical Precondition
    int sensorValue = digitalRead(sensorTwoPin);
    if(sensorValue==LOW) 
    {  
      Serial.println("No Object detected by Sensor 2");
    }
    else
    {
      Serial.println("Object Detected by Sensor 2");
    }
    return sensorValue;

  }


  int readSensor3()
  {
    // Rotation Sensor
    int sensorValue = digitalRead(sensorThreePin);
    if(sensorValue==LOW)
    {  
      Serial.println("No Object detected by Sensor 3");
    }
    else
    {
      Serial.println("Object Detected by Sensor 3");
    }
    return sensorValue;
  }

  
  int readSensor4()
  {
    //Green - 4 Horizontal Precondition   
    int sensorValue = digitalRead(sensorFourPin);
    if(sensorValue==LOW)
    {  
      Serial.println("No Object detected by Sensor 4");
    }
    else
    {
      Serial.println("Object Detected by Sensor 4");
    }
    return sensorValue;

  }
  

 void programFlow(int controllerPositionCode)
 {
    //confirm which sensors are initial state sensors
    sensorState2 = readSensor2();
    sensorState4 = readSensor4();
    if (sensorState2 == HIGH && sensorState4 == HIGH )
    {
      Serial.print("Drive Motor");
      Serial.print("\n");
      driveMotor(controllerPositionCode);
        
    }
    else 
    {
      Serial.print("Not Safe do not proceed");
      Serial.print("\n");

    }

  
  
  }
void decideDirection(int xValue,int yValue)
{
  if(xValue >=550 && xValue <=800 )
  {
    if(yValue >=495 && yValue <=550)
    {
      Serial.print("Rotate Left");
      Serial.print("\n");
      controllerPositionCode = 1;
    } 
  }
   else if(xValue >=200 && xValue <300 )
   {
    if(yValue >=495 && yValue <=550)
    {
      Serial.print("Rotate Right");
      Serial.print("\n");
      controllerPositionCode = 2;
    } 
  }
   
   else if(yValue >=200 && yValue <300 )
   {
    if(xValue >=490 && xValue <=550)
    {
      Serial.print("Go Up");
      Serial.print("\n");
      controllerPositionCode = 3;
    } 
  }

   else if(yValue >=550 && yValue <=800 )
   { 
    if(xValue >=500 && xValue <=550)
    {
      Serial.print("Go Down");
      Serial.print("\n");
      controllerPositionCode = 4;

    } 
  }

  else
    {
    Serial.print("Center Position");
    Serial.print("\n");
    controllerPositionCode = 0;
    }
    
    currentState = controllerPositionCode;
    if (currentState != previousState)
    {
      Serial.print("Call program flow\n");
      programFlow(controllerPositionCode);
    }
    else if(currentState == 0)
    {
      Serial.print("\n");
    }
    else
    {
      Serial.print("Dont hold back the joystick\n");
      Serial.print("\n");
    }
    
    previousState = currentState;
    }

void readCommand(){
  xValue = analogRead(A0);  
  yValue = analogRead(A1); 
  Serial.println(xValue);
  Serial.println(yValue);

  readSensor1();
  //readSensor2();
  //readSensor3();
  //readSensor4(); 
  decideDirection(xValue,yValue);
  } 

void driveHorizontalMotorLeft()
{
  Serial.print("Drive Motor Left");
  Serial.print("\n");
  bool dir = setDirLeft;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps += steps; 
  rotateHorizontalMotorXDegrees(dir,steps);
  
}

void driveHorizontalMotorRight()
{
  Serial.print("Drive Motor Right");
  Serial.print("\n");
  bool dir = setDirRight;
  int steps = calculateRotationSteps(PPROfHorizontalMotor,requiredAngleForHorizontalMotor);
  currentPosOfHorizontalMotorInSteps -= steps;
  rotateHorizontalMotorXDegrees(dir,steps);
}

void driveVerticalMotorUp()
{
  Serial.print("Drive Motor Up");
  Serial.print("\n");
  bool dir = setDirUp;
  int steps = calculateRotationSteps(PPROfVerticalMotor,requiredAngleForVerticalMotor);
  Serial.println("steps:");
  Serial.println(steps);
  currentPosOfVerticalMotorInSteps += steps; 
  rotateVerticalMotorXDegrees(dir,steps);
}
  
void driveVerticalMotorDown()
{
  Serial.print("Drive Motor Down");
  Serial.print("\n");
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

void blinkLed(){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(1500);                       // wait for half a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
  delay(500);
  }

void loop() 
{ 

  if (RESET == true){
    resetMotorPosition();
    RESET = false;
    }

   if (mode == 0){
    readSerial();
    }
   else if(mode == 1) {
    readCommand();
    delay(100);
    }
  
}