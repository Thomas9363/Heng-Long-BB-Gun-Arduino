// Using APP to control a BB gun on a pan-tilt setup

#include <Servo.h>  // Include the Servo library for controlling servos

Servo panServo, tiltServo;  // Create instances for pan and tilt servos
const int pan = 9;  // Pin for pan servo
const int tilt = 4; // Pin for tilt servo
const int Laser = 12; // Pin for laser
const int MOTOR_PIN = 3;  // Pin for motor
const int SWITCH_PIN = 7; // Pin for limit switch

int pan_servopos, tilt_servopos; // Variables to store servo positions
int dataIn[4] = {255, 0, 100, 100}; // Array to store information (255, button, X, Y)
int in_byte = 0; // Variable to store incoming byte
int array_index = 0; // Index for the dataIn array
int motorState = LOW; // State of the motor (LOW = off, HIGH = on)
int switchState = 1;  // State of the limit switch (1 = open, 0 = closed)

void setup() {
  Serial.begin(19200); // Initialize serial communication
  panServo.attach(pan); // Attach the pan servo to its pin
  tiltServo.attach(tilt); // Attach the tilt servo to its pin
  panServo.write(90); // Set initial pan servo position to 90 degrees
  tiltServo.write(90); // Set initial tilt servo position to 90 degrees
  pinMode(Laser, OUTPUT); // Set laser pin as an output
  pinMode(MOTOR_PIN, OUTPUT); // Set motor pin as an output
  digitalWrite(Laser, LOW); // Turn off the laser
  pinMode(SWITCH_PIN, INPUT); // Set limit switch pin as an input
  digitalWrite(SWITCH_PIN, HIGH); // Enable internal pull-up resistor for the limit switch
}

void loop() {
  if (Serial.available() > 0) { // Check if there's data available from serial
    in_byte = Serial.read(); // Read the incoming byte
    if (in_byte == 255) { // Check if the byte is the start indicator
      array_index = 0; // Reset array index
    }
    dataIn[array_index] = in_byte; // Store the incoming byte in the dataIn array
    array_index = array_index + 1; // Increment the array index
  }

  if (dataIn[1] == 1) digitalWrite(Laser, HIGH); // Turn on the laser if laser button is pressed
  else digitalWrite(Laser, LOW); // Turn off the laser if laser button is not pressed

  if (dataIn[1] == 2) { // Check if fire button is pressed
    Serial.print("FIRE   ");
    motorState = !motorState; // Toggle the motor state
    while (switchState == 1) { // While limit switch is open
      digitalWrite(MOTOR_PIN, motorState); // Turn the motor on/off
      switchState = digitalRead(SWITCH_PIN); // Read the state of the limit switch
      Serial.println(switchState);
      if (switchState == 0) { // When the limit switch is closed
        motorState = !motorState; // Toggle the motor state again
        digitalWrite(MOTOR_PIN, motorState); // Turn the motor off
      }
    }
    switchState = 1; // Reset the limit switch state to open
  }

  // Map servo readings and move the servos accordingly
  pan_servopos = map(dataIn[2], 0, 200, 160, 20);
  panServo.write(pan_servopos);
  tilt_servopos = map(dataIn[3], 0, 200, 70, 110);
  tiltServo.write(tilt_servopos);
  
  // Uncomment the following lines to print received data to serial monitor
  /*
  Serial.print(dataIn[0]);
  Serial.print(", button:");
  Serial.print(dataIn[1]);
  Serial.print(", X:");
  Serial.print(dataIn[2]);
  Serial.print(", Y:");
  Serial.print(dataIn[3]);
  Serial.print(",");
  Serial.println(in_byte);
  */
}
