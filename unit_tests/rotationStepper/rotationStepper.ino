/*
TEST FILE FOR THE ROTATION STEPPER MOTOR: used for unit testing of the motor controlling the rotation plate
*/


#include <Bonezegei_DRV8825.h>


#define motorForward 1
#define motorReverse 0

#define motorDirPin 29
#define motorStepPin 27
#define limitSwitchPin 7

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 800;
const int vialCount = 32;

Bonezegei_DRV8825 stepperRound(motorDirPin, motorStepPin);

// Controls how much the stepper has to rotate between vials
int stepsToRotate;
int stepsBetweenVials = stepsPerRevRound / vialCount;

const int totalDays = 31;
int bottlesFilled = 0;


//Tests the stepper rotating between each vial
void testIndividualRotation() {
  Serial.println("Testing individual rotation... ");
  while (bottlesFilled != totalDays) {
    stepperRound.step(motorForward, stepsBetweenVials);
    Serial.println(bottlesFilled);
    bottlesFilled++;
    delay(2000);
  }
}

// tests the stepper rotating between the flush position and all the vial positions

void fillSequence() {
  bottlesFilled = 0;
  while (bottlesFilled != totalDays) {
    //only rotates if the plate is at the home position
    while (!homeButtonHit()) {
      flushRun();
    }
    // Rotates the plate to the next vial position
    delay(3000);
    for (int i = 0; i < stepsToRotate; i+= 25){
      stepperRound.step(motorForward,25);
      //delay(70);
    }
    //stepperRound.step(motorForward, stepsToRotate);
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    Serial.print("Total Steps: ");
    Serial.println(stepsToRotate);
    delay(5000);
    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled + 1);
    //moves back to the starting slot
    flushRun();
  }
}


//moves the plate to the flushing slot
void flushRun() {
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()) {
    stepperRound.step(motorReverse, 10);
    delay(50);
  }
}

//checks if the device has rotated to the home/flush position
bool homeButtonHit() {
  //Serial.println(digitalRead(limitSwitchPin) == HIGH);
  return (digitalRead(limitSwitchPin) == LOW);
}

void setup() {
  pinMode(limitSwitchPin, INPUT_PULLUP);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("hi");
  bottlesFilled = 0;
  //microseconds per step. 7500us/step = 10RPM
  stepperRound.setSpeed(4000);
  stepsToRotate = stepsBetweenVials * (bottlesFilled + 1);
}

void loop() {
  fillSequence();
}
