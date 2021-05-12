#include "StepperIterface.h"
#include <stdlib.h>
#include "Arduino.h"

uint8_t STEPMODE[STEPMODE_N][3] = {{0,0,0}, //FULLSTEP
								   {1,0,0}, //HALFSTEP
								   {0,1,0}, //MICROSTEP4
								   {1,1,0}, //MICROSTEP8
								   {0,0,1}, //MICROSTEP16
								   {1,0,1}  //MICROSTEP32
								   }; //LSB - MSB

t_DRV8825 *DRV8825_New (uint8_t pin_dir, uint8_t pin_step, uint8_t pin_enable, uint8_t pin_reset, uint8_t pin_sleep, uint8_t pin_error, enum e_StepMode mode, uint8_t pin_mode0, uint8_t pin_mode1, uint8_t pin_mode2) {
	
	t_DRV8825 *stepper = NULL;
	
	stepper = malloc(sizeof(t_DRV8825));
	if( stepper == NULL) {
		return NULL;
	}
	
	stepper->pin_dir = pin_dir;
	stepper->pin_step = pin_step;
	stepper->pin_enable = pin_enable;
	stepper->pin_reset = pin_reset;
	stepper->pin_sleep = pin_sleep;
	stepper->pin_error = pin_error;
	stepper->pin_mode0 = pin_mode0;
	stepper->pin_mode1 = pin_mode1;
	stepper->pin_mode2 = pin_mode2;
	stepper->state_mode = mode;
	stepper->state_dir = DRV8825_CW;
	stepper->state_enabled = 1;
	stepper->state_sleeping = 0;
	
	if(stepper->pin_dir != 0) {
		digitalWrite(stepper->pin_dir, stepper->state_dir);
	}
	if(stepper->pin_step != 0) {
		digitalWrite(stepper->pin_step, LOW);
	}
	if(stepper->pin_enable != 0) {
		digitalWrite(stepper->pin_enable, LOW);
	}
	if(stepper->pin_reset != 0) {
		digitalWrite(stepper->pin_reset, HIGH);
	}
	if(stepper->pin_sleep != 0) {
		digitalWrite(stepper->pin_sleep, HIGH);
	}
	
	DRV8825_SetMode (stepper, mode);
	
	return stepper;
}

uint8_t DRV8825_Step (t_DRV8825 *stepper, enum e_StepDir dir){
	
	if(stepper == NULL) {
		return 0;
	}
	
	if(DRV8825_HasError (stepper) == 1) {
		return 1;
	}
	
	if(stepper->state_dir != dir) {
		stepper->state_dir = dir;
		if(stepper->pin_dir != 0) {
			digitalWrite(stepper->pin_dir, dir);
			//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
		}
	}
	
	//the DRV8825 steps at the rising edge of the step pin
	digitalWrite(stepper->pin_step, HIGH);
	digitalWrite(stepper->pin_step, LOW);
	
	return 0;
}

void DRV8825_Enable (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
		
		if(stepper->state_enabled == 0) {
			
			stepper->state_enabled = 1;
		
			if(stepper->pin_enable != 0) {
				//enable is active LOW
				digitalWrite(stepper->pin_enable, LOW);
				//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
			}
		}
	}
}

void DRV8825_Disable (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
		
		if(stepper->state_enabled == 1) {
			
			stepper->state_enabled = 0;
		
			if(stepper->pin_enable != 0) {
				//enable is active LOW
				digitalWrite(stepper->pin_enable, HIGH);
				//the digitalWrite function takes an average of 6us, which is far more than the setup time before the STEP command (650ns max)
			}
		}
	}
}

void DRV8825_Reset (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
				
		if(stepper->pin_reset != 0) {
			//sleep is active LOW
			digitalWrite(stepper->pin_reset, LOW);
			digitalWrite(stepper->pin_reset, HIGH);
		}
	}
}

void DRV8825_Sleep (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
		
		if(stepper->state_sleeping == 0) {
			
			stepper->state_sleeping = 1;
		
			if(stepper->pin_sleep != 0) {
				//sleep is active LOW
				digitalWrite(stepper->pin_sleep, LOW);
			}
		}
	}
}

void DRV8825_Wake (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
		
		if(stepper->state_sleeping == 1) {
			
			stepper->state_sleeping = 0;
		
			if(stepper->pin_sleep != 0) {
				//sleep is active LOW
				digitalWrite(stepper->pin_sleep, HIGH);
				delayMicroseconds(1700); //the wakeup time is 1.7ms max
			}
		}
	}
}

uint8_t DRV8825_HasError (t_DRV8825 *stepper){
	
	if(stepper != NULL) {
					
		if(stepper->pin_sleep != 0) {
			//fault is active LOW
			if(digitalRead(stepper->pin_sleep) == LOW) {
				return 1;
			}
		}
	}
	
	return 0;
}

void DRV8825_SetMode (t_DRV8825 *stepper, enum e_StepMode mode){
	
	if(stepper != NULL) {
					
		stepper->state_mode = mode;
		
		if(stepper->pin_mode0 != 0) {
			digitalWrite(stepper->pin_mode0, STEPMODE[mode][0]);
		}
		if(stepper->pin_mode1 != 0) {
			digitalWrite(stepper->pin_mode1, STEPMODE[mode][1]);
		}
		if(stepper->pin_mode2 != 0) {
			digitalWrite(stepper->pin_mode2, STEPMODE[mode][2]);
		}
	}
}
