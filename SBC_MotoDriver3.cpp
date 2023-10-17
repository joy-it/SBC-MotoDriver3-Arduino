/*
    SBC_MotoDriver3.cpp - Library for SBC-MotoDriver3 with the NXP PCA9634 chip.
    https://joy-it.net/de/products/SBC-MotoDriver3
    Created by L0L0-tack, 1 June, 2023.
    Released for Joy-IT.
    Last modified, 29 June, 2023.
    Library originally created by Nick van Tholen: https://github.com/NickvanTholen/pca9634-lib
*/
#include "Arduino.h"
#include "SBC_MotoDriver3.h"

// Constructor //

// Initialize the Library
SBCMotoDriver3::SBCMotoDriver3(uint8_t adress, uint8_t oePin) {
  _oepin = oePin;
  _addr = adress;
  pinMode(oePin, OUTPUT);
}

// Public //

// Uses a I2C adress reset
void SBCMotoDriver3::softReset() {
  Wire.beginTransmission(0x03);
  Wire.write(0xA5);
  Wire.write(0x5A);
  Wire.endTransmission();
}

// Sets up the whole chip and I2C connection
void SBCMotoDriver3::begin() {
  digitalWrite(_oepin, LOW);
  Wire.begin();
  delay(10);
  writeReg(0x00, 0x01);
  delayMicroseconds(500);
  writeReg(0x01, 0x14);
  delay(10);
}

// Writes to a register
uint8_t SBCMotoDriver3::writeReg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission();
}

// Reads a register
uint8_t SBCMotoDriver3::readReg(uint8_t reg) {
  Wire.beginTransmission(_addr);
  Wire.write(reg & 0x1F);
  Wire.endTransmission();
  Wire.requestFrom(_addr, (uint8_t)1);
  return Wire.read();
}

// Uses the output enable pin to enable and disable all channels, doesn't effect the previous state.
void SBCMotoDriver3::enabled(bool state) {
  if (state == true) {
    digitalWrite(_oepin, LOW);
  } else {
    digitalWrite(_oepin, HIGH);
  }
}

// Turn a channel on
void SBCMotoDriver3::on(uint8_t pin) {
  pinType(1, pin);
  chanPwm(pin, 255);
}

// Turn a channel off
void SBCMotoDriver3::off(uint8_t pin) {
  pinType(0, pin);
  chanPwm(pin, 0);
}

// Turn all channels on
void SBCMotoDriver3::allOn(bool forward, bool backward) {
  if (forward == true) {
    int pin = 0;
    for (int i = 0; i < 4; i++) {
      pwm(pin, 255);
      pin += 2;
    }
  } else if (backward == true) {
    int pin = 1;
    for (int i = 0; i < 4; i++) {
      pwm(pin, 255);
      pin += 2;
    }
  } else {
    pinType(1, 0, true);
    for (int i = 0; i < 8; i++) {
      chanPwm(i, 255);
    }
  }
}

// Turn all channels off
void SBCMotoDriver3::allOff() {
  pinType(0, 0, true);
  for (int i = 0; i < 8; i++) {
    chanPwm(i, 0);
  }
}

// Fade in for single led channel
void SBCMotoDriver3::fadeIn(uint8_t pin, int time, uint8_t brightness) {
  pinType(2, pin);
  int interval = time / brightness;
  for (int i = 0; i <= brightness; i++) {
    chanPwm(pin, i);
    delay(interval);
  }
  if (brightness == 255) {
    pinType(1, pin);
  }
}

// Fade out for single led channel
void SBCMotoDriver3::fadeOut(uint8_t pin, int time, uint8_t brightness) {
  uint8_t regValue;
  pinType(2, pin);
  regValue = readReg(pin + 2);
  int interval = time / (regValue - brightness);
  for (int i = regValue; i >= brightness; i--) {
    chanPwm(pin, i);
    delay(interval);
  }
  if (brightness == 0) {
    pinType(0, pin);
  }
}

// Pwm function for each individual channel
void SBCMotoDriver3::pwm(uint8_t pin, uint8_t value) {
  pinType(2, pin);
  chanPwm(pin, value);
}

// Check if a led is Off, On or PWM modus
uint8_t SBCMotoDriver3::ledStatus(uint8_t pin) {
  uint8_t regValue;
  bool first, second;
  if (pin < 4) {
    regValue = readReg(LEDOUT0);
    first = bitRead(regValue, (pin * 2));
    second = bitRead(regValue, (pin * 2 + 1));
  } else {
    pin -= 4;
    regValue = readReg(LEDOUT1);
    first = bitRead(regValue, (pin * 2));
    second = bitRead(regValue, (pin * 2 + 1));
  }
  if (!first && !second) {
    return 0;
  } else if (first && !second) {
    return 1;
  } else if (!first && second) {
    return 2;
  }
}

// Check the PWM value of a channel
uint8_t SBCMotoDriver3::pwmStatus(uint8_t pin) {
  return readReg(pin + 2);
}

// Set the speed and the max amount of steps for the stepper motor
void SBCMotoDriver3::StepperSpeed(int speed, int steps) {
  numberSteps = steps;
  stepDelay = (60 * 1000 * 1000 / steps / speed);
}

// Based on the speed and the max amount of steps. Decide the direction in which the stepper motor is supposed to turn
void SBCMotoDriver3::Stepper(int stepAmount, int pin1, int pin2, int pin3, int pin4) {
  stepsRemaining = abs(stepAmount);
  if (stepAmount > 0) { direction = 1; }
  if (stepAmount < 0) { direction = 0; }

  while (stepsRemaining > 0) {
    now = micros();
    if ((now - lastStepTime) >= stepDelay) {
      lastStepTime = now;
      if (direction == 1) {
        stepNumber += 1;
        if (stepNumber == numberSteps) {
          stepNumber = 0;
        }
      } else {
        if (stepNumber == 0) {
          stepNumber = numberSteps;
        }
        stepNumber -= 1;
      }
      stepsRemaining -= 1;
      stepMotor(stepNumber % 4, pin1, pin2, pin3, pin4);
    }
  }
}

// Private functions //

// Sets a channel or all channels to different state
void SBCMotoDriver3::pinType(uint8_t type, uint8_t pin, bool all) {
  uint8_t dataType, regValue;
  if (type > 2) {
    type = 0;
  }
  switch (type) {
    case 0:
      if (all) {
        writeReg(LEDOUT0, LED_OFF_ALL);
        writeReg(LEDOUT1, LED_OFF_ALL);
        break;
      } else if (pin < 4) {
        regValue = readReg(LEDOUT0);
        bitClear(regValue, (pin * 2));
        bitClear(regValue, (pin * 2 + 1));
        writeReg(LEDOUT0, regValue);
        break;
      } else if (pin >= 4) {
        pin -= 4;
        regValue = readReg(LEDOUT1);
        bitClear(regValue, (pin * 2));
        bitClear(regValue, (pin * 2 + 1));
        writeReg(LEDOUT1, regValue);
        break;
      }
      break;
    case 1:
      if (all) {
        writeReg(LEDOUT0, LED_ON_ALL);
        writeReg(LEDOUT1, LED_ON_ALL);
        break;
      } else if (pin < 4) {
        regValue = readReg(LEDOUT0);
        bitSet(regValue, (pin * 2));
        bitClear(regValue, (pin * 2 + 1));
        writeReg(LEDOUT0, regValue);
        break;
      } else if (pin >= 4) {
        pin -= 4;
        regValue = readReg(LEDOUT1);
        bitSet(regValue, (pin * 2));
        bitClear(regValue, (pin * 2 + 1));
        writeReg(LEDOUT1, regValue);
        break;
      }
      break;
    case 2:
      if (all) {
        writeReg(LEDOUT0, LED_PWM_ALL);
        writeReg(LEDOUT1, LED_PWM_ALL);
        break;
      } else if (pin < 4) {
        regValue = readReg(LEDOUT0);
        bitClear(regValue, (pin * 2));
        bitSet(regValue, (pin * 2 + 1));
        writeReg(LEDOUT0, regValue);
        break;
      } else if (pin >= 4) {
        pin -= 4;
        regValue = readReg(LEDOUT1);
        bitClear(regValue, (pin * 2));
        bitSet(regValue, (pin * 2 + 1));
        writeReg(LEDOUT1, regValue);
        break;
      }
      break;
  }
}

// Writes a pwm value to a channel
void SBCMotoDriver3::chanPwm(uint8_t channel, uint8_t value) {
  channel += 2;
  writeReg(channel, value);
}

// Based on the direction. Makes the Stepper motor move according to the direction
void SBCMotoDriver3::stepMotor(int thisStep, int pin1, int pin2, int pin3, int pin4) {
  if (direction == 1) {
    if (thisStep == 0) {
      pwm(pin1, 250);
      pwm(pin2, 0);
      pwm(pin3, 250);
      pwm(pin4, 0);
    }
    if (thisStep == 1) {
      pwm(pin1, 0);
      pwm(pin2, 250);
      pwm(pin3, 250);
      pwm(pin4, 0);
    }
    if (thisStep == 2) {
      pwm(pin1, 0);
      pwm(pin2, 250);
      pwm(pin3, 0);
      pwm(pin4, 250);
    }
    if (thisStep == 3) {
      pwm(pin1, 250);
      pwm(pin2, 0);
      pwm(pin3, 0);
      pwm(pin4, 250);
    }
  } else if (direction == 0) {
    if (thisStep == 0) {
      pwm(pin4, 250);
      pwm(pin3, 0);
      pwm(pin2, 250);
      pwm(pin1, 0);
    }
    if (thisStep == 1) {
      pwm(pin4, 0);
      pwm(pin3, 250);
      pwm(pin2, 250);
      pwm(pin1, 0);
    }
    if (thisStep == 2) {
      pwm(pin4, 0);
      pwm(pin3, 250);
      pwm(pin2, 0);
      pwm(pin1, 250);
    }
    if (thisStep == 3) {
      pwm(pin4, 250);
      pwm(pin3, 0);
      pwm(pin2, 0);
      pwm(pin1, 250);
    }
  }
}