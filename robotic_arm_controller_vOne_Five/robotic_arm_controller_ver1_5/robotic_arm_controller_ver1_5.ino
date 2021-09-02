#include <Servo.h>
#define ARRAY_LENGTH 5
#define SECONDS 10

// instantiating servo objects & creating an array of servos
Servo servos[5];

// instantiating integer array of flex sensor pin locations
// just replace 'flexPinN' with the pin value
int flexPins[] = {A0, A1, A2, A3, A5};
int flexLowVals[ARRAY_LENGTH];
int flexHighVals[ARRAY_LENGTH];
bool useCalibration = false;

void setup() {
  Serial.begin(9600);

  // attaching each servo object from servos array to a digital pin location of a servo motor
  servos[0].attach(3);
  servos[1].attach(5);
  servos[2].attach(6);
  servos[3].attach(9);
  servos[4].attach(10);

  if (useCalibration){
    // skeptical at the moment if the array names will provide
    // the right values needed for for-loops inside loop()
    Serial.println("Robotic Arm Controller Calibration");
    Serial.println("Flex fingers, and hold the position...");
    for (int i = 0; i < ARRAY_LENGTH; i++){
      flexLowVals[i] = analogRead(flexPins[i]);
      delay(1000);
    }
    Serial.println("**Flex Calibration Complete**");
    Serial.println("Extend fingers, and hold the position...");
    for (int i = 0; i < ARRAY_LENGTH; i++){
      flexHighVals[i] = analogRead(flexPins[i]);
      delay(1000);
    }
    Serial.println("**Extend Calibration Complete**");
    Serial.println("Robotic Arm Controller is now calibrated.");
  }
}

void loop() {
  // instantiating iterator for for-loops
  int i = 0;
  
  // array of five integers will hold the flex sensor input for each sensor,
  int flexVals[5];

  // array of five integers will hold the position each servo motor will move to
  int servoPositions[5];

  // setting each flexVal variable to the output from analogRead result from each flex sensor
  for (i = 0; i < ARRAY_LENGTH; i++){
    flexVals[i] = analogRead(flexPins[i]);
  }

  // mapping the output from each flex sensor (roughly ranging from 300 to 530) to the degrees
  // a servo motor can rotate (0 to 180)
  int flexValConverter; // an arbitrary variable used to store a reversed ordered number
  for (i = 0; i < ARRAY_LENGTH; i++){
    if (useCalibration){
      flexValConverter = map(flexVals[i], flexLowVals[i], flexHighVals[i], flexHighVals[i], flexLowVals[i]);
      servoPositions[i] = map(flexValConverter, flexLowVals[i], flexHighVals[i], 0, 180);
      servoPositions[i] = constrain(servoPositions[i], 0, 180);
    } else {
      // hard-coded flex sensor values
      flexValConverter = map(flexVals[i], 220, 400, 400, 220);
      servoPositions[i] = map(flexValConverter, 220, 400, 0, 180);
      servoPositions[i] = constrain(servoPositions[i], 0, 180);    
    }
  }

  // map function does not actually constrain x to toLow and toHigh
  // Note: a ratio is kept to make sure x follows the pattern of toLow and toHigh
  // constrain function will make sure x is within the low and high range
  for (i = 0; i < ARRAY_LENGTH; i++){
    servoPositions[i] = constrain(servoPositions[i], 0, 180);
  }

  // for debugging: seeing the change in analogRead output from each flex sensor
  String flexString;
  String servoString;
  for (i = 0; i < ARRAY_LENGTH; i++){
    flexString = "flex_sensor " + String(i+1) + ": "; // the respective flex sensor in question
    Serial.print(flexString);
    Serial.print(flexVals[i]);
    servoString = "\tservo_motor " + String(i+1) + ": "; // the respective flex sensor in question
    Serial.print(servoString);
    Serial.println(servoPositions[i]);
  }
  Serial.println("");

  // sending a signal to each servo indicating the rotation the servo motor should move to
  for (i = 0; i < ARRAY_LENGTH; i++){
    servos[i].write(servoPositions[i]);
  }

  // to help enhance the readability of Serial Monitor when Arduino is plugged into a computer
  delay(750);
}
