#include "accelStepper.h"

void asSetMaxSpeed(accelStepper *stepper, float speed) {
	if (speed < 0.0)
		speed = -speed;
	if (stepper->_maxSpeed != speed) {
		stepper->_maxSpeed = speed;
		stepper->_cmin = OS_TICK_FREQUENCY / speed;
		// Recompute _n from current speed and adjust speed if accelerating or cruising
		if (stepper->_n > 0) {
			stepper->_n = (int32_t) ((stepper->_speed * stepper->_speed) / (2.0 * stepper->_acceleration)); // Equation 16
			asComputeNewSpeed(stepper);
		}
	}
}

void asSetAcceleration(accelStepper *stepper, float acceleration) {

	if (acceleration == 0.0)
		return;
	if (acceleration < 0.0)
		acceleration = -acceleration;
	if (stepper->_acceleration != acceleration) {
		// Recompute _n per Equation 17
		stepper->_n = stepper->_n * (stepper->_acceleration / acceleration);
		// New c0 per Equation 7, with correction per Equation 15
		stepper->_c0 = 0.676 * sqrtf(2.0 / acceleration) * 1000.0; // Equation 15
		stepper->_acceleration = acceleration;
		asComputeNewSpeed(stepper);
	}

}

void asSetSpeed(accelStepper *stepper, float speed) {

	if (speed == stepper->_speed)
		return;

	if (speed == 0.0)
		stepper->_stepInterval = 0;
	else {
		stepper->_stepInterval = ((OS_TICK_FREQUENCY / (fabsf(stepper->_speed))));
		stepper->_direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
	}

	stepper->_speed = speed;

}
float asGetSpeed(accelStepper *stepper) {
	return stepper->_speed;
}


float max(float one, float two) {
	if (two >= one) {
		return two;
	} else
		return one;
}

void asMoveTo(accelStepper *stepper, int32_t absolute) {

	if (stepper->_targetPos != absolute) {
		stepper->_targetPos = absolute;
		asComputeNewSpeed(stepper);
		// compute new n?
	}
}

void asComputeNewSpeed(accelStepper *stepper) {

	int32_t distanceTo = asDistanceToGo(stepper); // +ve is clockwise from curent location

	int32_t stepsToStop = (int32_t) ((stepper->_speed * stepper->_speed) / (2.0 * stepper->_acceleration)); // Equation 16

	if (distanceTo == 0 && stepsToStop <= 1) {
		// We are at the target and its time to stop
		stepper->_stepInterval = 0;
		stepper->_speed = 0.0;
		stepper->_n = 0;
		return;
	}

	if (distanceTo > 0) {
		// We are anticlockwise from the target
		// Need to go clockwise from here, maybe decelerate now
		if (stepper->_n > 0) {
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if ((stepsToStop >= distanceTo) || stepper->_direction == DIRECTION_CCW)
				stepper->_n = -stepsToStop; // Start deceleration
		} else if (stepper->_n < 0) {
			// Currently decelerating, need to accel again?
			if ((stepsToStop < distanceTo) && stepper->_direction == DIRECTION_CW)
				stepper->_n = -stepper->_n; // Start accceleration
		}
	} else if (distanceTo < 0) {
		// We are clockwise from the target
		// Need to go anticlockwise from here, maybe decelerate
		if (stepper->_n > 0) {
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if ((stepsToStop >= -distanceTo) || stepper->_direction == DIRECTION_CW)
				stepper->_n = -stepsToStop; // Start deceleration
		} else if (stepper->_n < 0) {
			// Currently decelerating, need to accel again?
			if ((stepsToStop < -distanceTo) && stepper->_direction == DIRECTION_CCW)
				stepper->_n = -stepper->_n; // Start accceleration
		}
	}

	// Need to accelerate or decelerate
	if (stepper->_n == 0) {
		// First step from stopped
		stepper->_cn = stepper->_c0;
		stepper->_direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
	} else {
		// Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
		stepper->_cn = (stepper->_cn - ((2.0 * stepper->_cn) / ((4.0 * stepper->_n) + 1))); // Equation 13
		stepper->_cn = max(stepper->_cn, stepper->_cmin);
	}
	stepper->_n++;
	stepper->_stepInterval = stepper->_cn;
	stepper->_speed = (OS_TICK_FREQUENCY / stepper->_cn);
	if (stepper->_direction == DIRECTION_CCW)
		stepper->_speed = -stepper->_speed;

}

void asMove(accelStepper *stepper, int32_t relative) {
	asMoveTo(stepper, stepper->_currentPos + relative);
}

int32_t asDistanceToGo(accelStepper *stepper) {
	return stepper->_targetPos - stepper->_currentPos;
}

int32_t asTargetPosition(accelStepper *stepper) {
	return stepper->_targetPos;
}

int32_t asCurrentPosition(accelStepper *stepper) {
	return stepper->_currentPos;
}

void asSetCurrentPosition(accelStepper *stepper, int32_t position) {
	stepper->_targetPos=stepper->_currentPos = position;
	stepper->_n = 0;
	stepper-> _stepInterval = 0;
	stepper-> _speed = 0.0;
	}



// Work out and return a new speed.
// Subclasses can override if they want
// Implement acceleration, deceleration and max speed
// Negative speed is anticlockwise
// This is called:
//  after each step
//  after user changes:
//   maxSpeed
//   acceleration
//   target position (relative or absolute)
//static float asDesiredSpeed2(accelStepper *stepper) {
//	int32_t distanceTo = asDistanceToGo(stepper);
//
//	// Max possible speed that can still decelerate in the available distance
//	float requiredSpeed;
//	if (distanceTo == 0)
//		return 0.0; // Were there
//	else if (distanceTo > 0) // Clockwise
//		requiredSpeed = sqrtf(2.0 * distanceTo * stepper->_acceleration);
//	else
//		// Anticlockwise
//		requiredSpeed = -sqrtf(2.0 * -distanceTo * stepper->_acceleration);
//
//	if (requiredSpeed > stepper->_speed) {
//		// Need to accelerate in clockwise direction
//		if (stepper->_speed == 0)
//			requiredSpeed = sqrtf(2.0 * stepper->_acceleration);
//		else
//			requiredSpeed = stepper->_speed + fabsf(stepper->_acceleration / stepper->_speed);
//		if (requiredSpeed > stepper->_maxSpeed)
//			requiredSpeed = stepper->_maxSpeed;
//	} else if (requiredSpeed < stepper->_speed) {
//		// Need to accelerate in anticlockwise direction
//		if (stepper->_speed == 0)
//			requiredSpeed = -sqrtf(2.0 * stepper->_acceleration);
//		else
//			requiredSpeed = stepper->_speed - fabsf(stepper->_acceleration / stepper->_speed);
//		if (requiredSpeed < -stepper->_maxSpeed)
//			requiredSpeed = -stepper->_maxSpeed;
//	}
////  Serial.println(requiredSpeed);
//	return requiredSpeed;
//}

static void asStep(accelStepper *stepper, uint8_t step) {

	switch (step) {
	case 0:    // 1010
		stepper->_stepSequence = 0b1010;
		break;

	case 1:    // 0110
		stepper->_stepSequence = 0b0110;
		break;

	case 2:    //0101
		stepper->_stepSequence = 0b0101;
		break;

	case 3:    //1001
		stepper->_stepSequence = 0b1001;
		break;
	}

}
////Run with acceleration
bool asRun(accelStepper *stepper) {
//	if (stepper->_targetPos == stepper->_currentPos)
//		return false;

	if (asRunSpeed(stepper))
		asComputeNewSpeed(stepper);
	return stepper->_speed != 0.0 || asDistanceToGo(stepper) != 0;
}

//Run with acceleration
//bool asRun(accelStepper *stepper) {
//	if (stepper->_targetPos == stepper->_currentPos)
//			return false;
//	if (asRunSpeed(stepper))
//		asComputeNewSpeed(stepper);
//	return (bool) asDistanceToGo(stepper)!=0;
//}
bool asRunSpeedToPosition(accelStepper *stepper) {
	if (stepper->_targetPos == stepper->_currentPos) {
		return false;
	}
    if (stepper->_targetPos >stepper->_currentPos)
    	stepper->_direction = DIRECTION_CW;
    else
    	stepper->_direction = DIRECTION_CCW;

    asRunSpeed(stepper);
    return  stepper->_speed != 0.0 || asDistanceToGo(stepper) != 0;

}


// Blocks until the target position is reached
void asRunToPosition(accelStepper *stepper) {
	while (asRun(stepper))
		;
}

// Implements steps according to the current speed
// You must call this at least once per step
// returns true if a step occurred
bool asRunSpeed(accelStepper *stepper) {
	//Do nothing unless we have a step interval
	if(stepper->_stepInterval==0)
		return false;
	uint16_t time = chVTGetSystemTime();
	if ((uint16_t) (time - stepper->_lastStepTime) >= stepper->_stepInterval)  //We need to use 16 bit math for overflow to work
			{

		if (stepper->_direction == DIRECTION_CW) {
			stepper->_currentPos += 1;
		} else {
			stepper->_currentPos -= 1;
		}

//		if (stepper->_speed > 0) {
//			// Clockwise
//			stepper->_currentPos += 1;
//		} else if (stepper->_speed < 0) {
//			// Anticlockwise
//			stepper->_currentPos -= 1;
//		}
		asStep(stepper, stepper->_currentPos & 0x3); // Bottom 2 bits (same as mod 4, but works with + and - numbers)
		stepper->_lastStepTime = time;
		return true;
	} else
		return false;
}

