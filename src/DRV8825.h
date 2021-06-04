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
 * @file DRV8825.h
 * @version 1.0.0
 * @author Victor Henrique Salvi
 * 
 * DRV8825 Stepper Driver interface for Arduino.
 *
 * Features
 * - Its a interface to use the DRV8825 (H-Bridge bipolar Stepper Motor Controller) IC as a stepper motor driver.
 * - Its responsible to set and monitor the configurations, status and commands to the IC.
 * 
 * Limitations 
 * - This interface is not intended to handle when to step (calculate step frequency to rotate at a defined speed).
 *  
 * Changelog
 * |Version|Date|Contributor|Description|
 * |---|---|---|---|
 * |1.0.0|2021/06/03|Victor Salvi|Added the files to be compatible to the Arduino Library Manager (examples, properties file, keywords)|
 * |1.0.0|2021/06/03|Victor Salvi|Source code and usage documentation|
 * |1.0.0|2019/05/08|Victor Salvi|First Implementation|
 *
 */

#ifndef DRV8825_H
#define DRV8825_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define STEPMODE_N 6 //number of items in the e_StepMode

/** 
 * Defines the stepping modes.
 */
enum e_StepMode {
	DRV8825_FULLSTEP = 0, /**< Full step. Means that each driver step will turn the motor by one step. Has the greatest torque and speed but lowest resolution */ 
	DRV8825_HALFSTEP, /**< Half step. Means that two driver steps will turn the motor by one step. Usualy is the most balanced option */ 
	DRV8825_MICROSTEP4, /**< 1/4 step. Means that four driver steps will turn the motor by one step. */ 
	DRV8825_MICROSTEP8, /**< 1/8 step. Means that eight driver steps will turn the motor by one step. */ 
	DRV8825_MICROSTEP16, /**< 1/16 step. Means that sixten driver steps will turn the motor by one step. */ 
	DRV8825_MICROSTEP32 /**< 1/32 step. Means that thirty two driver steps will turn the motor by one step. It is the smoothest and has the highest resolution but has the slowest top speed */ 
};

/** 
 * Defines the convention for the direction of rotation.
 */
enum e_StepDir {
	DRV8825_CCW = 0, /**< Counter clockwise direction */
	DRV8825_CW /**< Clockwise direction */
};

/** 
 * structure that holds the pin mapping and the state variables.
 */
typedef struct {
	uint8_t pin_dir; /**< Arduino pin for the step direction pin. */
	uint8_t pin_step; /**< Arduino pin for the step command pin. */
	uint8_t pin_enable; /**< Arduino pin for the enable pin. */
	uint8_t pin_reset; /**< Arduino pin for the reset pin. */
	uint8_t pin_sleep; /**< Arduino pin for the sleep pin. */
	uint8_t pin_error; /**< Arduino pin for the error state pin. */
	uint8_t pin_mode0; /**< Arduino pin for the stepping mode 0 pin. */
	uint8_t pin_mode1; /**< Arduino pin for the stepping mode 1 pin. */  
	uint8_t pin_mode2; /**< Arduino pin for the stepping mode 2 pin. */
	enum e_StepMode state_mode; /**< Current stepping mode. */
	enum e_StepDir state_dir; /**< Current rotation direction. */
	uint8_t state_enabled; /**< Enable state. */
	uint8_t state_sleeping; /**< Sleep state. */
} t_DRV8825;

/** 
 * Creates a object that represents wiring and state of a stepper driver
 * 
 * All the pins except pin_step are optional. If a optional pin is not connect, set its value to 0. All functionality from unconnected pins will not be usable. The unconnected pins may need to be hardwired to vcc or gnd.
 * If the stepping mode is being set in software, it's recommended to connect the 3 mode pins.
 * The default state is wake and enabled.
 * 
 * @param pin_dir Arduino pin connected to the driver step direction pin. Send 0 if unconnected.
 * @param pin_step Arduino pin connected to the driver step direction pin. This parameter is mandatory.
 * @param pin_enable Arduino pin connected to the driver enable pin. If unconnected, set the value as 0 and connect the driver pin to GND (enabled).
 * @param pin_reset Arduino pin connected to the driver reset pin. If unconnected, set the value as 0 and connect the driver pin to VCC (not reset).
 * @param pin_sleep Arduino pin connected to the driver sleep pin. If unconnected, set the value as 0 and connect the driver pin to VCC (wake).
 * @param pin_error Arduino pin connected to the driver error output pin. Set the value as 0 if unconnected.
 * @param mode Stepping mode. If the stepping mode is not being set by software, set as DRV8825_FULLSTEP.
 * @param pin_mode0 Arduino pin connected to the driver step mode 0 pin. If unconnected, set the value as 0 and check the IC datasheet.
 * @param pin_mode1 Arduino pin connected to the driver step mode 1 pin. If unconnected, set the value as 0 and check the IC datasheet.
 * @param pin_mode2 Arduino pin connected to the driver step mode 2 pin. If unconnected, set the value as 0 and check the IC datasheet.
 * @return A pointer to the structure representing the driver.
 * @since 1.0.0
 */
t_DRV8825 *DRV8825_New (uint8_t pin_dir, uint8_t pin_step, uint8_t pin_enable, uint8_t pin_reset, uint8_t pin_sleep, uint8_t pin_error, enum e_StepMode mode, uint8_t pin_mode0, uint8_t pin_mode1, uint8_t pin_mode2);

/** 
 * Makes the stepper step one time in the given direction.
 * If the driver is sleeping or disabled, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @param dir The step direction.
 * @return 0 if the driver isn't reporting an error or 1 otherwise.
 * @since 1.0.0
 */
uint8_t DRV8825_Step (t_DRV8825 *driver, enum e_StepDir dir);

/** 
 * Sets the stepping mode.
 * It will only set the connected mode pins.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @param mode Stepping mode.
 * @since 1.0.0
 */
void DRV8825_SetMode (t_DRV8825 *driver, enum e_StepMode mode);

/** 
 * Enables the driver.
 * If the pin_enable is unconnected, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @since 1.0.0
 */
void DRV8825_Enable (t_DRV8825 *driver);

/** 
 * Disables the driver. A disabled driver will unpower the motor coils, lowering the power consumption but leaving the motor suceptible to unwanted movement.
 * If the pin_enable is unconnected, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @since 1.0.0
 */
void DRV8825_Disable (t_DRV8825 *driver);

/** 
 * Sets the driver into sleep mode. A sleeping driver will unpower the motor coils, lowering the power consumption but leaving the motor suceptible to unwanted movement. The sleep mode also disable some hardware functions inside the IC consuming lower power than the disabled mode.
 * If the pin_sleep is unconnected, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @since 1.0.0
 */
void DRV8825_Sleep (t_DRV8825 *driver);

/** 
 * Wakes the driver from sleep mode.
 * If the pin_enable is unconnected, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @since 1.0.0
 */
void DRV8825_Wake (t_DRV8825 *driver);

/** 
 * Resets the driver. This will unpower the stepper coils for some moments and reset the position in the step table.
 * If the pin_reset is unconnected, the command will be ignored.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @since 1.0.0
 */
void DRV8825_Reset (t_DRV8825 *driver);

/** 
 * Checks if the driver is reporting an error state. The IC can report an error state if its protection circuits are triggered (Overcurrent Protection, Undervoltage Lockout or Thermal Shutdown)
 * If the pin_error is unconnected, will return no error.
 * 
 * @param driver A pointer to the structure representing the driver.
 * @return 0 if the IC isn't reporting an error or 1 otherwise.
 * @since 1.0.0
 */
uint8_t DRV8825_HasError (t_DRV8825 *driver);

#ifdef __cplusplus
}
#endif

#endif