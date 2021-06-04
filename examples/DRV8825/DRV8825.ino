/**
 * DRV8825
 * 
 * This example shows how to use the library to interface with a stepper driver.
 *  
 * @version 1.1.0
 * @author Victor Henrique Salvi
 */
#include "DRV8825.h"
#include <stdint.h>

//pin mapping
const uint8_t pin_dir = 3;
const uint8_t pin_step = 4;
const uint8_t pin_enable = 5;
const uint8_t pin_reset = 6;
const uint8_t pin_sleep = 7;
const uint8_t pin_error = 8;
const uint8_t pin_mode0 = 9;
const uint8_t pin_mode1 = 10;
const uint8_t pin_mode2 = 11;

t_DRV8825 *driver; //driver is the variable that you will need to pass to the other functions

void setup() {

	pinMode(LED_BUILTIN, OUTPUT);

	driver = DRV8825_New (pin_dir, pin_step, pin_enable, pin_reset, pin_sleep, pin_error, DRV8825_FULLSTEP, pin_mode0, pin_mode1, pin_mode2); //creates the driver object

}

void loop() {
	
	uint8_t n; 
	
/* stepping and direction */	
	for (n = 0; n < 200; n++) { //do 200 steps clockwise
		DRV8825_Step (driver, DRV8825_CW);
		delay(15); //waits 15ms between steps
	}
	
	for (n = 0; n < 200; n++) { //do 200 steps counter clockwise
		DRV8825_Step (driver, DRV8825_CCW);
		delay(15); //waits 15ms between steps
	}

/* enabling and disabling the driver */	
	DRV8825_Disable (driver); //disables the driver

	delay(3000);
	
	DRV8825_Enable (driver); //enables the driver
	
/* checking the driver health */
	if(DRV8825_HasError (driver)) { //Checks if the driver is reporting an error. If true will blink the led for half second
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
	}
	
}