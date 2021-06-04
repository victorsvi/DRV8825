# DRV8825

DRV8825 Stepper Driver interface for Arduino.

## Data Types

### `t_DRV8825`

Structure that holds the pin mapping and the state variables.

#### Fields

* **`uint8_t pin_dir`** Arduino pin for the step direction pin.
* **`uint8_t pin_step`** Arduino pin for the step command pin.
* **`uint8_t pin_enable`** Arduino pin for the enable pin.
* **`uint8_t pin_reset`** Arduino pin for the reset pin.
* **`uint8_t pin_sleep`** Arduino pin for the sleep pin.
* **`uint8_t pin_error`** Arduino pin for the error state pin.
* **`uint8_t pin_mode0`** Arduino pin for the stepping mode 0 pin.
* **`uint8_t pin_mode1`** Arduino pin for the stepping mode 1 pin.
* **`uint8_t pin_mode2`** Arduino pin for the stepping mode 2 pin.
* **`enum e_StepMode state_mode`** Current stepping mode.
* **`enum e_StepDir state_dir`** Current rotation direction.
* **`uint8_t state_enabled`** Enable state.
* **`uint8_t state_sleeping`** Sleep state.

### `e_StepMode`

Enumaration that defines the stepping modes.

#### Values

* **`DRV8825_FULLSTEP`** Full step. Means that each driver step will turn the motor by one step. Has the greatest torque and speed but lowest resolution.
* **`DRV8825_HALFSTEP`** Half step. Means that two driver steps will turn the motor by one step. Usualy is the most balanced option.
* **`DRV8825_MICROSTEP4`** 1/4 step. Means that four driver steps will turn the motor by one step.
* **`DRV8825_MICROSTEP8`** 1/8 step. Means that eight driver steps will turn the motor by one step.
* **`DRV8825_MICROSTEP16`** 1/16 step. Means that sixten driver steps will turn the motor by one step.
* **`DRV8825_MICROSTEP32`** 1/32 step. Means that thirty two driver steps will turn the motor by one step. It is the smoothest and has the highest resolution but has the slowest top speed.

### `e_StepDir`

Enumaration that defines the convention for the direction of rotation.

#### Values

* **`DRV8825_CCW`** Counter clockwise direction.
* **`DRV8825_CW`** Clockwise direction.

## Methods

### `DRV8825_New`

Creates a object that represents wiring and state of a stepper driver
All the pins except pin_step are optional. If a optional pin is not connect, set its value to 0. All functionality from unconnected pins will not be usable. The unconnected pins may need to be hardwired to vcc or gnd.
If the stepping mode is being set in software, it's recommended to connect the 3 mode pins.
The default state is wake and enabled.

#### Definition

```
t_DRV8825 *DRV8825_New (uint8_t pin_dir, uint8_t pin_step, uint8_t pin_enable, uint8_t pin_reset, uint8_t pin_sleep, uint8_t pin_error, enum e_StepMode mode, uint8_t pin_mode0, uint8_t pin_mode1, uint8_t pin_mode2);
```

#### Parameters

* **`pin_dir`** Arduino pin connected to the driver step direction pin. Send 0 if unconnected.
* **`pin_step`** Arduino pin connected to the driver step direction pin. This parameter is mandatory.
* **`pin_enable`** Arduino pin connected to the driver enable pin. If unconnected, set the value as 0 and connect the driver pin to GND (enabled).
* **`pin_reset`** Arduino pin connected to the driver reset pin. If unconnected, set the value as 0 and connect the driver pin to VCC (not reset).
* **`pin_sleep`** Arduino pin connected to the driver sleep pin. If unconnected, set the value as 0 and connect the driver pin to VCC (wake).
* **`pin_error`** Arduino pin connected to the driver error output pin. Set the value as 0 if unconnected.
* **`mode`**  Stepping mode. If the stepping mode is not being set by software, set as _DRV8825_FULLSTEP_.
* **`pin_mode0`** Arduino pin connected to the driver step mode 0 pin. If unconnected, set the value as 0 and check the IC datasheet.
* **`pin_mode1`** Arduino pin connected to the driver step mode 1 pin. If unconnected, set the value as 0 and check the IC datasheet.
* **`pin_mode2`** Arduino pin connected to the driver step mode 2 pin. If unconnected, set the value as 0 and check the IC datasheet.

#### Returns

A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_Step`

Makes the stepper step one time in the given direction.
If the driver is sleeping or disabled, the command will be ignored.

#### Definition

```
uint8_t DRV8825_Step (t_DRV8825 *stepper, enum e_StepDir dir);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.
* **`dir`** The step direction.

#### Returns

0 if the driver isn't reporting an error or 1 otherwise.

#### Since

1.0.0

### `DRV8825_SetMode`

Sets the stepping mode.
It will only set the connected mode pins.

#### Definition

```
void DRV8825_SetMode (t_DRV8825 *stepper, enum e_StepMode mode);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.
* **`mode`** Stepping mode.

#### Since

1.0.0

### `DRV8825_Enable`

Enables the driver.
If the pin_enable is unconnected, the command will be ignored.

#### Definition

```
void DRV8825_Enable (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_Disable`

Disables the driver. A disabled driver will unpower the motor coils, lowering the power consumption but leaving the motor suceptible to unwanted movement.
If the pin_enable is unconnected, the command will be ignored.

#### Definition

```
void DRV8825_Disable (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_Sleep`

Sets the driver into sleep mode. A sleeping driver will unpower the motor coils, lowering the power consumption but leaving the motor suceptible to unwanted movement. The sleep mode also disable some hardware functions inside the IC consuming lower power than the disabled mode.
If the pin_sleep is unconnected, the command will be ignored.

#### Definition

```
void DRV8825_Sleep (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_Wake`

Wakes the driver from sleep mode.
If the pin_enable is unconnected, the command will be ignored.

#### Definition

```
void DRV8825_Wake (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_Reset`

Resets the driver. This will unpower the stepper coils for some moments and reset the position in the step table.
If the pin_reset is unconnected, the command will be ignored.

#### Definition

```
void DRV8825_Reset (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Since

1.0.0

### `DRV8825_HasError`

Checks if the driver is reporting an error state. The IC can report an error state if its protection circuits are triggered (Overcurrent Protection, Undervoltage Lockout or Thermal Shutdown)
If the pin_error is unconnected, will return no error.

#### Definition

```
void DRV8825_HasError (t_DRV8825 *stepper);
```

#### Parameters

* **`stepper`** A pointer to the structure representing the driver.

#### Returns

0 if the IC isn't reporting an error or 1 otherwise.

#### Since

1.0.0

## Source Code Version

1.0.0
