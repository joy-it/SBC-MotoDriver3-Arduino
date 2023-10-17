// Import all required libraries
#include <Wire.h>
#include <SBC_MotoDriver3.h>

// Initialization of the board with I2C address 0x15 and oe_pin 17
SBCMotoDriver3 MotorDriver(0x15, 4);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  // Pull the oe_pin low to activate the board
  MotorDriver.enabled(true);
  // Starts the I2C communication
  MotorDriver.begin();
  // Switch off all outputs
  MotorDriver.allOff();
  // Define the RPM and the maximum number of steps for the stepper motor
  MotorDriver.StepperSpeed(60, 2048);
}

// Main Loop
void loop() {
  Serial.println("Normal usage");
  // Switch on all even outputs
  MotorDriver.allOn(true, false);
  delay(1000);
  MotorDriver.allOff();
  delay(1000);
  // Switch on all odd outputs
  MotorDriver.allOn(false, true);
  delay(1000);
  MotorDriver.allOff();
  delay(1000);
  // Switch on a specific output
  MotorDriver.on(0);
  delay(2000);
  // Switch off a specific output
  MotorDriver.off(0);
  delay(500);
  //  A specific output is faded in to a specific value over a specific 
  MotorDriver.fadeIn(0, 20000, 250);
  MotorDriver.fadeOut(0, 20000, 0);
  delay(1000);
  // Sets the Pwm value of a specific output
  MotorDriver.pwm(0, 199);
  delay(1000);
  // Read the status of the specified output
  Serial.println(MotorDriver.ledStatus(0));
  Serial.println(MotorDriver.pwmStatus(0));
  delay(2000);
  MotorDriver.allOff();
  delay(1000);
  /*Serial.println("Stepper");
  // Let the stepper motor move the desired number of steps on the desired pins at the previously set speed.
  MotorDriver.Stepper(2000, 4, 5, 6, 7);
  delay(1000);*/
}
