# DRV8825

DRV8825 Stepper Driver interface for Arduino.

### Features

- Its a interface to use the DRV8825 (H-Bridge bipolar Stepper Motor Controller) IC as a stepper motor driver;
- Its responsible to set and monitor the configurations, status and commands to the IC.

### Limitations

- This interface is not intended to handle when to step (calculate step frequency to rotate at a defined speed).

## How to use

### Instalation 

You can instal the library by:
1. ~~Open the Library Manager inside Arduino IDE, find for "MatrixKeypad" and click install;~~ (Not published yet)
2. Go to _Releases_, download the .zip file, import the file inside Arduino IDE. 

### Developing the Code

To interface with the DRV8825, you will need to:
1. Define what functionality you will need (what pins you need to interface) and the pin mapping;
2. Hardwire the pins that will not be set by software.
3. create a pointer variable of the type t_DRV8825;
4. call DRV8825_New in "setup()" to initialize the driver object;
5. call DRV8825_Step whenever the motor needs to step.

You can check out this [example skecth](../master/examples/DRV8825/DRV8825.ino).

## Documentation

Read the documentation [here](../master/docs/api.md)
