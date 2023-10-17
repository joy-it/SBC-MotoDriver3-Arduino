# C++ Package for the PCA9634 8-bit Fm+ I2C-bus LED driver 

This library provides a C++ package for the PCA9634 8-bit Fm+ I2C-bus LED driver.
See **https://joy-it.net/products/SBC-MotoDriver3** for more details.

Derived from the pca9634-lib (**https://github.com/NickvanTholen/pca9634-lib**) by Nickvan Tholen (**https://github.com/NickvanTholen**).

## Behaviour considered to be a pass
As long as the microcontroller or single board computer still finds the I2C addresses of the PCA9634 (0x03, 0x15 and 0x70, 0x15 is the changeable I2C address), it is considered a pass. If the connected motors still rotate when prompted to rotate, it is also considered to be a pass.

## Behaviour considered to be a fail
As soon as the I2C addresses can no longer be found or the connected motors no longer rotate when prompted to do so.

# Basic functionality

## Initialization
You can use the constructor to change the I2C address and the oe pin used for communication with the PCA.
```python
# The constructor "SBCMotoDriver3 <your_name>" takes 2 parameters.
# SBCMotoDriver3 <your_name>(address, oe_pin)
# The parameter address allows to define the I2C address of the product
# The parameter oe_pin allows to define the pin to which the enable pin of the PCA is connected
# Change the I2C address depending on how the resistors on the board are set.
# default call of function with A1 = 1 | A2 = 0 | A3 = 1 | A4 = 0 | A5 = 1 | A6 = 0 | A7 = 0, resulting in I2C address 0x15 | oe pin = 4
SBCMotoDriver3 MotoDriver(0x15, 17)
```

## Soft Reset
The `soft_reset` function allows you to do a Software Reset of the PCA.
```python
# The function "soft_reset" takes no parameters.
# soft_reset()
MotoDriver.soft_reset()
```

## Starting communication
The function `begin` can be used to start the communication between a microcontroller or single board computer and the PCA with the previously defined address and oe pin.
```python
# The function "begin" takes no parameters.
# begin()
MotoDriver.begin()
```

## Enable
With the function `enabled(...)` you can choose if you want to enable or disable **THE ENTIRE OUTPUT** of the MotoDriver.
```python
# The function "enabled" takes 1 parameter.
# enabled(state)
# The parameter state allows to define if the PCA output should be anabled or not
# default call of function with state = True
MotoDriver.enabled(True)
```

## On
The `on(...)` function allows to switch on a single channel specified by the user with maximum value.
```python
# The function "on" takes 1 parameter.
# on(pin)
# The parameter pin allows to define which pin will be switched on at max value
# default call of function with 0
MotoDriver.on(0)
```

## Off
The `on(...)` function allows to switch off a single channel specified by the user.
```python
# The function "off" takes 1 parameter.
# off(pin)
# The parameter pin allows to define which pin will be switched off
# default call of function with 0
MotoDriver.off(0)
```

## All On
With the function `allOn(...)` you can choose between **all even**, **all odd** and **all** channels. The defined channels are then switched on with the maximum value.
```python
# The function "allOn" takes 2 parameters.
# allOn(forward, backward)
# The parameter forward allows to switch on all even channels
# The parameter backwards allows to switch on all odd channels
# When both parameters are combined false then all channels will be switched on
# default call of function with forward = False | backward = False
MotoDriver.allOn(False, False)
```

## All Off
With the function `allOff` you can switch off all outputs or set all outputs to 0.
```python
# The function "allOff" takes no parameters.
# allOff()
MotoDriver.allOff()
```

## Fade In
The function `fadeIn(...)` allows to fade in each of the 8 outputs to a certain value over a certain time.
```python
# The function "fadeIn" takes 3 parameters.
# fadeIn(pin, timer, brightness)
# The parameter pin allows to define the pin that will be faded out
# The parameter timer allows to set the time with which the delay in the function will be calculated
# The parameter brightness allows to define the value to which the prior defined pin will be faded out to
# default call of function with Channel = 0 | time in s = 0 | value to fade to = 0
MotoDriver.fadeIn(0, 0, 0)
```

## Fade Out
The function `fadeOut(...)` allows to fade out each of the 8 outputs to a certain value over a certain time.
```python
# The function "fadeOut" takes 3 parameters.
# fadeOut(pin, timer, brightness)
# The parameter pin allows to define the pin that will be faded out
# The parameter timer allows to set the time with which the delay in the function will be calculated
# The parameter brightness allows to define the value to which the prior defined pin will be faded out to
# default call of function with Channel = 0 | time in s = 0 | value to fade to = 0
MotoDriver.fadeOut(0, 0, 0)
```

## PWM
The `pwm(...)` function allows to set any user defined channel to any user defined pwm value.
```python
# The function "pwm" takes 2 parameter.
# pwm(pin, value)
# The parameter pin allows to define the pin which will be set to value
# The parameter value allows to define the pwm value of the specified pin.
# default call of function with Channel = 0 | value = 0
MotoDriver.pwm(0, 0)
```

## LED Status
The `ledStatus(...)` function returns the current status information of the channel specified by the user.
```python
# The function "ledStatus" takes 1 parameter.
# ledStatus(pin)
# The parameter pin allows to define the pin which will be read.
# default call of function with Channel = 0
MotoDriver.ledStatus(0)
```

## PWM Status
The `pwmStatus(...)` function returns the current pwm information of the channel specified by the user.
```python
# The function "pwmStatus" takes 1 parameter.
# pwmStatus(pin)
# The parameter pin allows to define the pin which will be read.
# default call of function with Channel = 0
MotoDriver.pwmStatus(0)
```


# Stepper functionality

## Stepper Speed
With the function `StepperSpeed(...)` you can set the desired speed and the maximum number of steps based on the used stepper motor.
```python
# The function "StepperSpeed" takes 2 parameters.
# StepperSpeed(speed, steps)
# The parameter speed allows to define the amount of revolutions per minute.
# The parameter steps allows to define the maximum amount of steps per 1 revolution of the stepper motor.
# default call of function with Rpm = 30 | Maximum amount of steps = 2048
MotoDriver.StepperSpeed(30, 2048)
```

## Stepper
With the function `Stepper(...)` you can let the stepper motor move a user specified number of steps on the pins specified by the user.
```python
# The function "Stepper" takes 5 parameters.
# Stepper(stepAmount, pin1, pin2, pin3, pin4)
# The parameter stepAmount allows to define how many steps the stepper motor should make.
# The parameter pin1 allows to define the first pin where the stepper motor is connected to.
# The parameter pin2 allows to define the second pin where the stepper motor is connected to.
# The parameter pin3 allows to define the third pin where the stepper motor is connected to.
# The parameter pin4 allows to define the fourth pin where the stepper motor is connected to.
# default call of function with Steps = 2000 | pin1 = 4 | pin2 = 5 | pin3 = 6 | pin4 = 7
MotoDriver.Stepper(2000, 4, 5, 6, 7)
```

## License

MIT
