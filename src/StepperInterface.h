/*
	**************************************************
	** DRV8825 Stepper Driver interface for Arduino **
	**************************************************

	By Victor Henrique Salvi (victorsvi@gmail.com)
	
	Functionality:
		Its a interface to use the DRV8825 (H-Bridge bipolar Stepper Motor Controller) IC as a stepper motor driver.
		Its responsible to set and monitor the configurations, status and commands to the IC.
		
		This interface is not intended to handle when to step (calculate step frequency to rotate at a defined speed).
	
	Changelog:
		08/05/2019:
			Frist version.

*/

#ifndef STEPPERITERFACE_H
#define STEPPERITERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define STEPMODE_N 6

/* Data Types */

/* The DRV8825 supports microstepping */
enum e_StepMode {FULLSTEP = 0, HALFSTEP, MICROSTEP4, MICROSTEP8, MICROSTEP16, MICROSTEP32};

enum e_StepDir {DRV8825_CCW = 0, DRV8825_CW};

struct s_DRV8825 {
	uint8_t pin_dir;
	uint8_t pin_step;
	uint8_t pin_enable;
	uint8_t pin_reset;
	uint8_t pin_sleep;
	uint8_t pin_error;
	uint8_t pin_mode0;
	uint8_t pin_mode1;
	uint8_t pin_mode2;
	enum e_StepMode state_mode;
	enum e_StepDir state_dir;
	uint8_t state_enabled;
	uint8_t state_sleeping;
};

typedef struct s_DRV8825 t_DRV8825;

/* Functions */

/* Creates a new of instance a controler 
 * If a pin is not used it should be set to 0. Note that the pin_step is mandatory.
 * If the mode is being set in software, it's recommended to connect the 3 mode pins
 * The default state is wake and enabled
 */
t_DRV8825 *DRV8825_New (uint8_t pin_dir, uint8_t pin_step, uint8_t pin_enable, uint8_t pin_reset, uint8_t pin_sleep, uint8_t pin_error, enum e_StepMode mode, uint8_t pin_mode0, uint8_t pin_mode1, uint8_t pin_mode2);

/* Makes the stepper step one time in the given direction 
 * Returns 0 if the IC isn't reporting an error or 1 otherwise
 */
uint8_t DRV8825_Step (t_DRV8825 *stepper, enum e_StepDir dir);

/* Changes the state of the IC to enabled. When enabled, the IC will recognize the step commands */
void DRV8825_Enable (t_DRV8825 *stepper);

/* Changes the state of the IC to disabled. When enabled, the IC will ignore any step commands */
void DRV8825_Disable (t_DRV8825 *stepper);

/* Resets the IC. At microstepping modes, also resets the the current row in coil current table */
void DRV8825_Reset (t_DRV8825 *stepper);

/* Sets the IC to a low power mode */
void DRV8825_Sleep (t_DRV8825 *stepper);

/* Wakes the IC from a low power mode */
void DRV8825_Wake (t_DRV8825 *stepper);

/* Returns 0 if the IC isn't reporting an error or 1 otherwise 
 * The IC can report an error state if its protection circuits are triggered (Overcurrent Protection, Undervoltage Lockout or Thermal Shutdown)
 */
uint8_t DRV8825_HasError (t_DRV8825 *stepper);

/* Sets the indexing mode for the stepper */
void DRV8825_SetMode (t_DRV8825 *stepper, enum e_StepMode mode);


#ifdef __cplusplus
}
#endif

#endif