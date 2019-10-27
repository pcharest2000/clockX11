#include "clock.h"

clockS clockA[CLOCKCOL][CLOCKROW];

//This is a function pointer to the running mode

void (*clRunningModeFunction_ptr)(void)=&clARunToTargetAcceleration;

void clSwitchRunningMode(StepperRunMode_t mode) {
	switch (mode) {
	case ACCELERATION:
		clRunningModeFunction_ptr = &clARunToTargetAcceleration;
		break;
	case CONSTANTSPEED:
		clRunningModeFunction_ptr = &clARunToTargetSpeed;
		break;
	default:
		break;
	}
}

void clRunSteppers(void) {

	(*clRunningModeFunction_ptr)();
}

//This function calculates and sets the max speed of clock arrays so they all arrive at there destination at the same time
void clARefractorMaxSpeed(float maxspeed) {
	uint8_t ilongest;  //Longest distance to get to target
	uint8_t jlongest;	//Longest distance to get to target
	clNeedle whatNeedle;
	int32_t minDistance;
	int32_t hourDistance;
	int32_t longestDistance = 0;	//The fastest speed in all the array we normalize to that speed
	float maxtime;
	float calspeed;

	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			minDistance = abs(clockA[i][j].minStepper._targetPos - clockA[i][j].minStepper._currentPos);
			hourDistance = abs(clockA[i][j].hourStepper._targetPos - clockA[i][j].hourStepper._currentPos);
			if (longestDistance < minDistance) {
				ilongest = i;
				jlongest = j;
				whatNeedle = MINNEEDLE;
				longestDistance = minDistance;
			}
			if (longestDistance < hourDistance) {
				ilongest = i;
				jlongest = j;
				longestDistance = hourDistance;
				whatNeedle = HOURNEEDLE;
			}
		}
	}

	maxtime = longestDistance / maxspeed;
	///We set the longuest clock and needle to the max speed
//	if (whatNeedle == MINNEEDLE)
//		maxtime = abs(clockA[ilongest][jlongest].minStepper._targetPos - clockA[ilongest][jlongest].minStepper._currentPos) / maxspeed;
//	else
//		maxtime = abs(clockA[ilongest][jlongest].hourStepper._targetPos - clockA[ilongest][jlongest].hourStepper._currentPos) / maxspeed;

	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			minDistance = abs(clockA[i][j].minStepper._targetPos - clockA[i][j].minStepper._currentPos);
			hourDistance = abs(clockA[i][j].hourStepper._targetPos - clockA[i][j].hourStepper._currentPos);
			calspeed = (float) minDistance/maxtime;
			asSetMaxSpeed(&clockA[i][j].minStepper, calspeed);
			calspeed = (float) hourDistance/maxtime;
			asSetMaxSpeed(&clockA[i][j].hourStepper, calspeed);


		}
	}

	//We now calculate the new speed base on the maxtime and set it

}

void clASetTargetPositions(uint8_t i, uint8_t j, int32_t minPosition, int32_t hourPosition) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	clSetTargetPositions(&clockA[i][j], minPosition, hourPosition);
}

void clASetAllTargetpostion(int32_t minPosition, int32_t hourPosition) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			clSetTargetPositions(&clockA[i][j], minPosition, hourPosition);
		}
	}
}

int32_t clAGetStepperPositionHour(uint8_t i, uint8_t j) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return 0;
	}
	return clockA[i][j].hourStepper._currentPos;

}

int32_t clAGetStepperPositionMin(uint8_t i, uint8_t j) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return 0;
	}
	return clockA[i][j].minStepper._currentPos;
}

void clASetMaxSpeed(uint8_t i, uint8_t j, float minSpeed, float hourSpeed) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	clSetMaxSpeed(&clockA[i][j], minSpeed, hourSpeed);
}

void clASetSpeed(uint8_t i, uint8_t j, float minSpeed, float hourSpeed) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	clSetSpeed(&clockA[i][j], minSpeed, hourSpeed);
}

void clASetSpeedAll(float minSpeed, float hourSpeed) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			clSetSpeed(&clockA[i][j], minSpeed, hourSpeed);
		}
	}
}

void clASetAcceleration(uint8_t i, uint8_t j, float minAcc, float hourAcc) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	asSetAcceleration(&clockA[i][j].hourStepper, hourAcc);
	asSetAcceleration(&clockA[i][j].minStepper, minAcc);
}

void clASetTargetPosition(uint8_t i, uint8_t j, int32_t minPosition, int32_t hourPosition) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	clockA[i][j].targetReached = false;
	clockA[i][j].minTarget = minPosition;
	clockA[i][j].hourTarget = hourPosition;
	asMoveTo(&clockA[i][j].hourStepper, clPosToSteps(hourPosition));
	asMoveTo(&clockA[i][j].minStepper, clPosToSteps(minPosition));
}

void clASetTargetRelative(uint8_t i, uint8_t j, int32_t minRelative, int32_t hourRelative) {
	if (i >= CLOCKCOL || j >= CLOCKROW) {
		return;
	}
	clockA[i][j].targetReached = false;
	clockA[i][j].minTarget += minRelative;
	clockA[i][j].hourTarget += hourRelative;
	asMove(&clockA[i][j].hourStepper, clPosToSteps(hourRelative));
	asMove(&clockA[i][j].minStepper, clPosToSteps(minRelative));
}


void clInit(clockS * clocks, float maxSpeed, int32_t minPosition, int32_t hourPosition) {

	asSetMaxSpeed(&clocks->hourStepper, maxSpeed);
	asSetMaxSpeed(&clocks->minStepper, maxSpeed);
	asSetAcceleration(&clocks->hourStepper, 30);
	asSetAcceleration(&clocks->minStepper, 30);
	clocks->minPosition = minPosition;
	clocks->hourPosition = hourPosition;
	asSetCurrentPosition(&clocks->hourStepper, clPosToSteps(hourPosition));
	asSetCurrentPosition(&clocks->minStepper, clPosToSteps(minPosition));
}

void clAInit(float maxSpeed, int32_t minPosition, int32_t hourPosition) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			clASetAcceleration(i, j, 100, 100);
			clASetMaxSpeed(i, j, maxSpeed, maxSpeed);
			//clASetTargetPosition(i, j, (rand() % 6000) - 3000, (rand() % 6000) - 3000);
			clASetTargetPosition(i, j, minPosition, hourPosition);

		}
	}

}

void clARunToTargetAcceleration(void) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			clRunToTargetAcceleration(&clockA[i][j]);
		}
	}
}

void clARunToTargetSpeed(void) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			clRunToTargetSpeed(&clockA[i][j]);
		}
	}
}

bool clATargetReached(uint8_t i, uint8_t j) {

	return clockA[i][j].targetReached;

}

bool clATargetsReached(void) {
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			if (clockA[i][j].targetReached == false) {
				return false;
			}
		}
	}
	return true;
}

void clSetTargetPositions(clockS *clocks, int32_t minPosition, int32_t hourPosition) {
	clocks->targetReached = false;
	clocks->minTarget = minPosition;
	clocks->hourTarget = hourPosition;
	asMoveTo(&clocks->hourStepper, clPosToSteps(hourPosition));
	asMoveTo(&clocks->minStepper, clPosToSteps(minPosition));
}
void clSetTargetHourPosition(clockS *clocks, int32_t hourPosition) {
	clocks->targetReached = false;
	clocks->hourTarget = hourPosition;
	asMoveTo(&clocks->hourStepper, clPosToSteps(hourPosition));
}
void clSetTargetMinPosition(clockS *clocks, int32_t minPosition) {
	clocks->targetReached = false;
	clocks->minTarget = minPosition;
	asMoveTo(&clocks->hourStepper, clPosToSteps(minPosition));
}

void clSetSpeed(clockS *clocks, float minSpeed, float hourSpeed) {

	asSetSpeed(&clocks->minStepper, minSpeed);
	asSetSpeed(&clocks->hourStepper, hourSpeed);
}

void clSetMaxSpeed(clockS *clocks, float minSpeed, float hourSpeed) {

	asSetMaxSpeed(&clocks->minStepper, minSpeed);
	asSetMaxSpeed(&clocks->hourStepper, hourSpeed);
}

void clRunToTargetAcceleration(clockS *clocks) {
	bool asMovedHour = asRun(&clocks->hourStepper);
	bool asMovedMin = asRun(&clocks->minStepper);
	if (asMovedMin == true || asMovedHour == true)
		clocks->targetReached = false;
	else {
		clocks->minPosition = clocks->minTarget;
		clocks->hourPosition = clocks->hourTarget;
		clocks->targetReached = true;
	}
}

void clRunToTargetSpeed(clockS *clocks) {
	bool asMovedHour = asRunSpeedToPosition(&clocks->hourStepper);
	bool asMovedMin = asRunSpeedToPosition(&clocks->minStepper);
	if (asMovedMin == true || asMovedHour == true)
		clocks->targetReached = false;
	else {
		clocks->minPosition = clocks->minTarget;
		clocks->hourPosition = clocks->hourTarget;
		clocks->targetReached = true;
	}
}

bool clReachedTarget(clockS *clocks) {

	return clocks->targetReached;
}

int32_t clGetTargetMinPosition(clockS *clock) {
	return clock->minTarget;
}

int32_t clGetTargetHourPosition(clockS *clock) {
	return clock->hourTarget;
}

int32_t clPosToSteps(int32_t pos) {
//float result = round( (float)pos*(STEPSPERREVOLUTION*GEARRATIO/2048.0));
	return (int32_t)((float) pos * (STEPSPERREVOLUTION * GEARRATIO / CLOCKRESOLUTION));
}

int32_t clStepstoClockPos(int32_t stepperPosition) {
//float result = round( (float)pos*(STEPSPERREVOLUTION*GEARRATIO/2048.0));
	return (int32_t) stepperPosition / (STEPSPERREVOLUTION * GEARRATIO) * CLOCKRESOLUTION;
}

int32_t clAGetNumberofTurnHour(uint8_t i, uint8_t j){
	if (i >= CLOCKCOL || j >= CLOCKROW) {
			return NULL;
		}

	return   clGetNumberofTurn(clockA[i][j].hourPosition);
}

int32_t clAGetNumberofTurnMin(uint8_t i, uint8_t j){
	if (i >= CLOCKCOL || j >= CLOCKROW) {
			return NULL;
		}

	return   clGetNumberofTurn(clockA[i][j].minPosition);
}

int32_t clGetNumberofTurn(int32_t postion){

	return postion/CLOCKRESOLUTION;
}


struct SEND_DATA_STRUCTURE {
//put your variable definitions here for the data you want to send
//THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
	int32_t minStepper;
	int32_t hourStepper;
};

//CComm stuuf

void clPrepareStepperArray(clockS *clock, uint8_t array[]) {
	int32_t inData[2];
	inData[0] = clock->minStepper._currentPos;
	inData[1] = clock->hourStepper._currentPos;
	uint8_t *ptr;
	ptr = (uint8_t*) &inData[0];

	array[0] = 0x06; //Magic bytes
	array[1] = 0x85;
	for (uint32_t i = 0; i < sizeof(inData); i++) {
		array[i + 2] = *ptr;
		ptr++;
	}

}

void sendstuf(void) {
	uint8_t array[10];
	static int32_t inData[2];
	inData[0] += 10;
	inData[1] = 0; //-299433971

	if (inData[0] > 2038) {
		inData[0] = 0;
	}
	uint8_t *ptr;
	ptr = (uint8_t*) inData;

	array[0] = 0x06; //Magic bytes
	array[1] = 0x85;
	for (uint32_t i = 0; i < 4 * 2; i++) {
		array[i + 2] = *ptr;
		ptr++;
	}
	sdWrite(&SD1, array, 10);
}

