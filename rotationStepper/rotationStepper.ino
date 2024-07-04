/*
TEST FILE FOR THE ROTATION STEPPER MOTOR: used for unit testing
*/


#include <Bonezegei_DRV8825.h>


#define motorForward 1
#define motorReverse 0

#define motorDirPin 8
#define motorStepPin 9
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
    Serial.println(stepsBetweenVials);
    bottlesFilled++;
    delay(2000);
  }
}

// tests the stepper rotating between the flush position and all the vial positions

void fillSequence() {

  while (bottlesFilled != totalDays) {
    //only rotates if the plate is at the home position
    while (!homeButtonHit()) {
      flushRun();
    }
    // Rotates the plate to the next vial position
    delay(3000);
    for (int i = 0; i < stepsToRotate;i++){
      stepperRound.step(motorForward, i);

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
  stepperRound.setSpeed(8000);
  stepsToRotate = stepsBetweenVials * (bottlesFilled + 1);
}

void loop() {
  delay(5000);
  // put your main code here, to run repeatedly:
  fillSequence();
  //bottlesFilled = 0;
  //flushRun();
  testIndividualRotation();
  delay(5000);
}
