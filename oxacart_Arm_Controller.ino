int xValue = 0 ;
int yValue = 0 ; 
int bValue = 0 ;
const int Pin=2;

int sensorState1;
int sensorState2;
int sensorState3;
int sensorState4;

int driverPul = 7;
int driverDir = 6;

boolean setDir = HIGH;
boolean setDirUp = HIGH;
boolean setDirDown = LOW;
boolean setDirLeft = HIGH;
boolean setDirRight = LOW;

int pd = 200;

// Center Position by default
int controllerPositionCode = 0;

int millisbetweenSteps = 50;

int numberOfSteps = 800;

int currentState = 10;
int previousState = 20;

void revMotor(){
  setDir = !setDir;
  }

void setup()  
{ 
  
  pinMode(8,INPUT); 
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(driverPul,OUTPUT);
  pinMode(driverDir,OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(2),revMotor,RISING);
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

void rotateMotorXDegrees(bool dir)
{
  digitalWrite(driverDir,dir);
  for (int n = 0; n < numberOfSteps;n++ )
  {
      
      digitalWrite(driverPul,HIGH);
      delayMicroseconds(pd);
      digitalWrite(driverPul,LOW);
      delayMicroseconds(pd);
      //delay(millisbetweenSteps);
  }
  
}
/*
void driveMotor(){
  digitalWrite(driverDir,setDir);
  digitalWrite(driverPul,HIGH);
  delayMicroseconds(pd);
  digitalWrite(driverPul,LOW);
  delayMicroseconds(pd);
  }
*/

int readSensor1(){
  // Vertical Sensor
  int sensorValue = digitalRead(2);
    if(sensorValue==LOW){ 
        Serial.println("no Object by s1");
    }
    else{
        Serial.println("Object Detected by s1");
    }
  return sensorValue;

  }

 int readSensor2(){
  //Green - 2 Vertical Precondition
  int sensorValue = digitalRead(3);
    if(sensorValue==LOW){  
        Serial.println("no Object by s2");
    }
    else{
        Serial.println("Object Detected by s2");
    }
   return sensorValue;

  }


  int readSensor3(){
    // Rotation Sensor
  int sensorValue = digitalRead(4);
    if(sensorValue==LOW){  
        Serial.println("no Object by s3");
    }
    else{
        Serial.println("Object Detected by s3");
    }
    return sensorValue;
  }

  
  int readSensor4(){
  //Green - 4 Horizontal Precondition   
  int sensorValue = digitalRead(5);
    if(sensorValue==LOW){  
        Serial.println("no Object by s4");
    }
    else{
        Serial.println("Object Detected by s4");
    }
    return sensorValue;

  }
  

 void programFlow(int controllerPositionCode){
  //confirm which sensors are initial state sensors
  sensorState2 = readSensor2();
  sensorState4 = readSensor4();
  if (sensorState2 == HIGH && sensorState4 == HIGH ){
    Serial.print("Drive Motor");
     Serial.print("\n");
      driveMotor(controllerPositionCode);
      
    }
  else {
     Serial.print("Not Safe do not proceed");
     Serial.print("\n");

    }

  
  
  }
void decideDirection(int xValue,int yValue){
  if(xValue >=550 && xValue <=800 ){
    if(yValue >=500 && yValue <=550){
      Serial.print("Rotate Left now");
      Serial.print("\n");
      controllerPositionCode = 1;
      } 
    }
   else if(xValue >=200 && xValue <500 ){
    if(yValue >=500 && yValue <=550){
      Serial.print("Rotate Right now");
      Serial.print("\n");
      controllerPositionCode = 2;

      } 
    }
   
   else if(yValue >=200 && yValue <500 ){
    if(xValue >=500 && xValue <=550){
      Serial.print("Go Up");
      Serial.print("\n");
     controllerPositionCode = 3;
      //rotateMotorXDegrees(setDirUp);
      } 
    }

   else if(yValue >=550 && yValue <=800 ){
    if(xValue >=500 && xValue <=550){
      Serial.print("Go Down");
      Serial.print("\n");
     controllerPositionCode = 4;
     //rotateMotorXDegrees(setDirDown);

      } 
    }

    else{
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
      else if(currentState == 0){
        Serial.print("\n");
        }
     else{
        Serial.print("Dont hold back the joystick\n");
        Serial.print("\n");
        }
     previousState = currentState;
      
    
    
    }

 
  
void readControl(){
  xValue = analogRead(A0);  
  yValue = analogRead(A1);  
  bValue = digitalRead(8);  
  //printResults(xValue,yValue);
  decideDirection(xValue,yValue);
  //readSensor1(); // Vertical Sensor
  //readSensor2();   //Green - 2 Vertical Precondition
  //readSensor3(); // Rotation Sensor
  //readSensor4(); //Green - 2 Horizontal Precondition
  } 

void driveHorizontalMotorLeft()
{
  Serial.print("drive left");
  Serial.print("\n");
  rotateMotorXDegrees(setDirLeft);
  
}

void driveHorizontalMotorRight()
{
  Serial.print("drive right");
  Serial.print("\n");
  rotateMotorXDegrees(setDirRight);
}

void driveVerticalMotorUp()
{
  Serial.print("drive up");
  Serial.print("\n");
  rotateMotorXDegrees(setDirUp);
  
}
void driveVerticalMotorDown()
{
  Serial.print("drive down");
  Serial.print("\n");
  rotateMotorXDegrees(setDirDown);
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



void loop() 
{ 
  readControl();
  //driveMotor();
  //rotateMotorXDegrees(setDirUp);
  //delay(1000);  
  //rotateMotorXDegrees(setDirDown);
  //delay(1000); 
  delay(100);
  
}
