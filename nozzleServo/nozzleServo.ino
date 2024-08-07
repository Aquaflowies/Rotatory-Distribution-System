/*
TEST FILE FOR THE NOZZLE SERVO: for unit testing
*/
#include <Servo.h>
#define nozzleServoPin 3

Servo nozzleServo;

//starting angle for servo
const int servoRotationAngle = 0;
//controls how much the servo has to rotate to move the nozzle
const int servoStartAngle = 90;

//sets up the servo 
void setServoPars(){
  nozzleServo.attach(nozzleServoPin);
  nozzleServo.write(servoStartAngle);
}


//tests movement of the nozzle up and down
void nozzleTest(){
  nozzleServo.write(servoRotationAngle);
  Serial.println("filling position");
  delay(3000);
  nozzleServo.write(servoStartAngle);
  Serial.println("start position");
  delay(3000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setServoPars();
  //pinMode(limitSwitchPin, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  nozzleTest();
}
