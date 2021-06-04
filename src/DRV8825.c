/*
	Copyright (c) 2021 Victor Salvi

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
/** 
 * @file DRV8825.c
 * @version 1.0.0
 * @author Victor Henrique Salvi
 * 
 * DRV8825 Stepper Driver interface for Arduino.
 *
 */

#include "DRV8825.h"
#include <stdlib.h>
#include "Arduino.h"

//values to set the stepping mode by the datasheet
uint8_t STEPMODE[STEPMODE_N][3] = {{0,0,0}, //FULLSTEP
								   {1,0,0}, //HALFSTEP
								   {0,1,0}, //MICROSTEP4
								   {1,1,0}, //MICROSTEP8
								   {0,0,1}, //MICROSTEP16
								   {1,0,1}  //MICROSTEP32
								   }; //LSB - MSB

t_DRV8825 *DRV8825_New (uint8_t pin_dir, uint8_t pin_step, uint8_t pin_enable, uint8_t pin_reset, uint8_t pin_sleep, uint8_t pin_error, enum e_StepMode mode, uint8_t pin_mode0, uint8_t pin_mode1, uint8_t pin_mode2) {
	
	t_DRV8825 *driver = NULL;
	
	driver = malloc(sizeof(t_DRV8825));
	if( driver == NULL) {
		return NULL;
	}
	
	driver->pin_dir = pin_dir;
	driver->pin_step = pin_step;
	driver->pin_enable = pin_enable;
	driver->pin_reset = pin_reset;
	driver->pin_sleep = pin_sleep;
	driver->pin_error = pin_error;
	driver->pin_mode0 = pin_mode0;
	driver->pin_mode1 = pin_mode1;
	driver->pin_mode2 = pin_mode2;
	driver->state_mode = mode;
	driver->state_dir = DRV8825_CW;
	driver->state_enabled = 1;
	driver->state_sleeping = 0;
	
	if(driver->pin_dir != 0) {
		pinMode(driver->pin_dir, OUTPUT);
		digitalWrite(driver->pin_dir, driver->state_dir);
	}
	if(driver->pin_step != 0) {
		pinMode(driver->pin_step, OUTPUT);
		digitalWrite(driver->pin_step, LOW);
	}
	if(driver->pin_enable != 0) {
		pinMode(driver->pin_enable, OUTPUT);
		digitalWrite(driver->pin_enable, LOW);
	}
	if(driver->pin_reset != 0) {
		pinMode(driver->pin_reset, OUTPUT);
		digitalWrite(driver->pin_reset, HIGH);
	}
	if(driver->pin_sleep != 0) {
		pinMode(driver->pin_sleep, OUTPUT);
		digitalWrite(driver->pin_sleep, HIGH);
	}
	if(driver->pin_error != 0) {
		pinMode(driver->pin_error, INPUT);
	}
	
	DRV8825_SetMode (driver, mode);
	
	return driver;
}

uint8_t DRV8825_Step (t_DRV8825 *driver, enum e_StepDir dir){
	
	if(driver == NULL) {
		return 0;
	}
	
	if(driver->state_enabled == 0) {
		return 0;
	}
	
	if(driver->state_sleeping == 0) {
		return 0;
	}
	
	if(DRV8825_HasError (driver) == 1) {
		return 1;
	}
	
	if(driver->state_dir != dir) {
		driver->state_dir = dir;
		if(driver->pin_dir != 0) {
			digitalWrite(driver->pin_dir, dir); //the values for the CW and CCW literals are the state of the dir pin (HIGH or LOW)
			//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
		}
	}
	
	//the DRV8825 steps at the rising edge of the step pin
	digitalWrite(driver->pin_step, HIGH);
	digitalWrite(driver->pin_step, LOW);
	
	return 0;
}

void DRV8825_Enable (t_DRV8825 *driver){
	
	if(driver != NULL) {
		
		if(driver->state_enabled == 0) {
			
			driver->state_enabled = 1;
		
			if(driver->pin_enable != 0) {
				//enable is active LOW
				digitalWrite(driver->pin_enable, LOW);
				//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
			}
		}
	}
}

void DRV8825_Disable (t_DRV8825 *driver){
	
	if(driver != NULL) {
		
		if(driver->state_enabled == 1) {
			
			driver->state_enabled = 0;
		
			if(driver->pin_enable != 0) {
				//enable is active LOW
				digitalWrite(driver->pin_enable, HIGH);
				//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
			}
		}
	}
}

void DRV8825_Reset (t_DRV8825 *driver){
	
	if(driver != NULL) {
				
		if(driver->pin_reset != 0) {
			//sleep is active LOW
			digitalWrite(driver->pin_reset, LOW);
			digitalWrite(driver->pin_reset, HIGH);
		}
	}
}

void DRV8825_Sleep (t_DRV8825 *driver){
	
	if(driver != NULL) {
		
		if(driver->state_sleeping == 0) {
			
			driver->state_sleeping = 1;
		
			if(driver->pin_sleep != 0) {
				//sleep is active LOW
				digitalWrite(driver->pin_sleep, LOW);
			}
		}
	}
}

void DRV8825_Wake (t_DRV8825 *driver){
	
	if(driver != NULL) {
		
		if(driver->state_sleeping == 1) {
			
			driver->state_sleeping = 0;
		
			if(driver->pin_sleep != 0) {
				//sleep is active LOW
				digitalWrite(driver->pin_sleep, HIGH);
				delayMicroseconds(1700); //the wakeup time is 1.7ms max
			}
		}
	}
}

uint8_t DRV8825_HasError (t_DRV8825 *driver){
	
	if(driver != NULL) {
					
		if(driver->pin_sleep != 0) {
			//fault is active LOW
			if(digitalRead(driver->pin_sleep) == LOW) {
				return 1;
			}
		}
	}
	
	return 0;
}

void DRV8825_SetMode (t_DRV8825 *driver, enum e_StepMode mode){
	
	if(driver != NULL) {
					
		driver->state_mode = mode;
		
		if(driver->pin_mode0 != 0) {
			digitalWrite(driver->pin_mode0, STEPMODE[mode][0]);
		}
		if(driver->pin_mode1 != 0) {
			digitalWrite(driver->pin_mode1, STEPMODE[mode][1]);
		}
		if(driver->pin_mode2 != 0) {
			digitalWrite(driver->pin_mode2, STEPMODE[mode][2]);
		}
	}
}
