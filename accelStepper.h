#ifndef ACELSTEPPER_H
#define ACELSTEPPER_H

#include "hal.h"
#include  <math.h>

#define  OS_TICK_FREQUENCY  2000.0/2
/// \brief Direction indicator
/// Symbolic names for the direction the motor is turning
typedef enum {
	DIRECTION_CCW = 0,  ///< Counter-Clockwise
	DIRECTION_CW = 1   ///< Clockwise
} Direction;

typedef struct {
	/// The current absolution position in steps.
	int32_t _currentPos;    // Steps
	/// The target position in steps. The AccelStepper library will move the
	/// motor from teh _currentPos to the _targetPos, taking into account the
	/// max speed, acceleration and deceleration
	int32_t _targetPos;     // Steps
	/// The current motos speed in steps per second
	/// Positive is clockwise
	float _speed;         // Steps per second
	/// The maximum permitted speed in steps per second. Must be > 0.
	float _maxSpeed;
	/// The acceleration to use to accelerate or decelerate the motor in steps
	/// per second per second. Must be > 0
	float _acceleration;
	float _sqrt_twoa; // Precomputed sqrt(2*_acceleration)

	/// The current interval between steps in milliseconds.
	uint16_t _stepInterval;
	/// The last step time in milliseconds
	uint16_t _lastStepTime;
	uint8_t _stepSequence;  //The index of the step function
	int8_t _direction; //This indicates the direction of the stepper

	 /// The step counter for speed calculations
	int32_t _n;
	/// Initial step size in milliseconds
	float _c0;

	/// Last step size in milliseconds
	float _cn;

	/// Min step size in microseconds based on maxSpeed
	float _cmin; // at max speed

} accelStepper;

/// Set the target position. The run() function will try to move the motor
/// from the current position to the target position set by the most
/// recent call to this function.
/// \param[in] absolute The desired absolute position. Negative is
/// anticlockwise from the 0 position.
void asMoveTo(accelStepper *stepper, int32_t absolute);

/// Set the target position relative to the current position
/// \param[in] relative The desired position relative to the current position. Negative is
/// anticlockwise from the current position.
void asMove(accelStepper *stepper, int32_t relative);

/// Poll the motor and step it if a step is due, implementing
/// accelerations and decelerations to achive the ratget position. You must call this as
/// fequently as possible, but at least once per minimum step interval,
/// preferably in your main loop.
/// \return true if the motor is at the target position.
bool asRun(accelStepper *stepper);

/// Poll the motor and step it if a step is due, implmenting a constant
/// speed as set by the most recent call to setSpeed().
/// \return true if the motor was stepped.
bool asRunSpeed(accelStepper *stepper);

/// Sets the maximum permitted speed. the run() function will accelerate
/// up to the speed set by this function.
/// \param[in] speed The desired maximum speed in steps per second. Must
/// be > 0. Speeds of more than 1000 steps per second are unreliable.
void asSetMaxSpeed(accelStepper *stepper, float speed);

/// Sets the acceleration and deceleration parameter.
/// \param[in] acceleration The desired acceleration in steps per second
/// per second. Must be > 0.
void asSetAcceleration(accelStepper *stepper, float acceleration);

/// Sets the desired constant speed for use with runSpeed().
/// \param[in] speed The desired constant speed in steps per
/// second. Positive is clockwise. Speeds of more than 1000 steps per
/// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
/// once per hour, approximately. Speed accuracy depends on the Arduino
/// crystal. Jitter depends on how frequently you call the runSpeed() function.
void asSetSpeed(accelStepper *stepper, float speed);

/// The most recently set speed
/// \return the most recent speed in steps per second
float asGetSpeed(accelStepper *stepper);

/// The distance from the current position to the target position.
/// \return the distance from the current position to the target position
/// in steps. Positive is clockwise from the current position.
int32_t asDistanceToGo(accelStepper *stepper);

/// The most recently set target position.
/// \return the target position
/// in steps. Positive is clockwise from the 0 position.
int32_t asTargetPosition(accelStepper *stepper);

/// The currently motor position.
/// \return the current motor position
/// in steps. Positive is clockwise from the 0 position.
int32_t asCurrentPosition(accelStepper *stepper);

/// Resets the current position of the motor, so that wherever the mottor
/// happens to be right now is considered to be the new position. Useful
/// for setting a zero position on a stepper after an initial hardware
/// positioning move.
/// \param[in] position The position in steps of wherever the motor
/// happens to be right now.
void asSetCurrentPosition(accelStepper *stepper, int32_t position);

/// Moves the motor to the target position and blocks until it is at
/// position. Dont use this in event loops, since it blocks.
void asRunToPosition(accelStepper *stepper);

/// Runs at the currently selected speed until the target position is reached
/// Does not implement accelerations.
bool asRunSpeedToPosition(accelStepper *stepper);

/// Moves the motor to the new target position and blocks until it is at
/// position. Dont use this in event loops, since it blocks.
/// \param[in] position The new target position.
void asRunToNewPosition(int32_t position);

/// Disable motor pin outputs by setting them all LOW
/// Depending on the design of your electronics this may turn off
/// the power to the motor coils, saving power.
/// This is useful to support Arduino low power modes: disable the outputs
/// during sleep and then reenable with enableOutputs() before stepping
/// again.
void asDisableOutputs(accelStepper *stepper);

/// Enable motor pin outputs by setting the motor pins to OUTPUT
/// mode. Called automatically by the constructor.
void enableOutputs(accelStepper *stepper);

//Private stuff
static void asComputeNewSpeed(accelStepper *stepper);
static void asStep(accelStepper *stepper, uint8_t step);







#endif
