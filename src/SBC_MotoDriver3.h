/*
    SBC_MotoDriver3.h - Library for SBC-MotoDriver3 with the NXP PCA9634 chip.
    https://joy-it.net/de/products/SBC-MotoDriver3
    Created by L0L0-tack, 1 June, 2023.
    Released for Joy-IT.
    Last modified, 29 June, 2023.
    Library originally created by Nick van Tholen: https://github.com/NickvanTholen/pca9634-lib
*/
#ifndef sbcmotodriver3
#define sbcmotodriver3

#define LED_OFF_ALL 0x00
#define LED_ON_ALL 0x55
#define LED_PWM_ALL 0xAA
#define LEDOUT0 0x0C
#define LEDOUT1 0x0D

#include <Arduino.h>
#include <Wire.h>

class SBCMotoDriver3 {
public:
  int numberSteps = 0;
  int stepDelay = 0;
  int stepsRemaining = 0;
  int stepNumber = 0;
  int lastStepTime = 0;
  int direction;
  int now;
  SBCMotoDriver3(uint8_t adress, uint8_t oePin);
  void softReset();
  void begin();
  uint8_t writeReg(uint8_t reg, uint8_t val);
  uint8_t readReg(uint8_t reg);
  void enabled(bool state);
  void on(uint8_t pin);
  void off(uint8_t pin);
  void allOn(bool forward = false, bool backward = false);
  void allOff();
  void fadeIn(uint8_t pin, int time, uint8_t brightness = 255);
  void fadeOut(uint8_t pin, int time, uint8_t brightness = 0);
  void pwm(uint8_t pin, uint8_t value);
  uint8_t ledStatus(uint8_t pin);
  uint8_t pwmStatus(uint8_t pin);
  void StepperSpeed(int speed, int steps);
  void Stepper(int stepAmount, int pin1, int pin2, int pin3, int pin4);
private:
  void chanPwm(uint8_t channel, uint8_t value);
  void pinType(uint8_t type, uint8_t pin, bool all = false);
  void stepMotor(int thisStep, int pin1, int pin2, int pin3, int pin4);
  uint8_t _addr;
  uint8_t _oepin;
};

#endif
