#ifndef CLOCK_H
#define CLOCK_H

#include "clock.h"
#include "accelStepper.h"
#include  <math.h>

//True 28BYJ-48 gear ratio  63.68395:1

#define GEARRATIO 63.68395f
#define GEARMUL	0.015702544f
#define STEPSPERREVOLUTION  32.0f
#define CLOCKRESOLUTION  2048  //Numbers of ticks per clock revolution per 360 deg rotation
#define ONETURN 2048
#define CLOCKCOL 8
#define CLOCKROW 3

#define CL0DEG 0
#define CL45DEG 256
#define CL90DEG 512
#define CL135DEG 768
#define CL180DEG 1024
#define CL225DEG 1280
#define CL270DEG 1536
#define CL315DEG 1792
#define CL360DEG 2048




typedef struct {
	accelStepper hourStepper;
	accelStepper minStepper;
	int32_t minPosition;		//Positions are mapped from 0-360 deg from 0-2048
	int32_t hourPosition;
	int32_t minTarget;
	int32_t hourTarget;
	bool targetReached;

} clockS;

extern clockS clockA[CLOCKCOL][CLOCKROW];

typedef enum StepperRunningMode {
	ACCELERATION, CONSTANTSPEED
} StepperRunMode_t;
typedef enum StepperNeedle {
	MINNEEDLE, HOURNEEDLE
} clNeedle;
void clSwitchRunningMode(StepperRunMode_t mode);

void clInit(clockS *clocks, float maxSpeed, int32_t minPosition, int32_t hourPosition);
void clSetTargetPositions(clockS *clocks, int32_t minPosition, int32_t hourPosition);
void clASetAllTargetpostion(int32_t minPosition, int32_t hourPosition);
void clSetTargetHourPosition(clockS *clocks, int32_t hourPosition);
void clSetTargetMinPosition(clockS *clocks, int32_t minPosition);
void clRunToTargetAcceleration(clockS *clocks);
void clSetMaxSpeed(clockS *clocks, float minSpeed, float hourSpeed);
void clSetSpeed(clockS *clocks, float minSpeed, float hourSpeed);
void clRunToTargetSpeed(clockS *clocks);
int32_t clGetTargetMinPosition(clockS *clock);
int32_t clGetTargetHourPosition(clockS *clock);
bool clReachedTarget(clockS *clocks);
int32_t clGetNumberofTurn(int32_t postion);
//Returns  the postion of the clock as a functions of steps
int32_t clPosToSteps(int32_t pos);
int32_t clStepstoClockPos(int32_t stepperPosition);
void clPrepareStepperArray(clockS *clock, uint8_t array[]);
void sendstuf(void);

//These functions have an effect on the global array
void clASetTargetPositions(uint8_t i, uint8_t j, int32_t minPosition, int32_t hourPosition);
void clASetSpeed(uint8_t i, uint8_t j, float minSpeed, float hourSpeed);
void clASetSpeedAll(float minSpeed, float hourSpeed);
void clASetAcceleration(uint8_t i, uint8_t j, float minAcc, float hourAcc);
void clASetTargetPosition(uint8_t i, uint8_t j, int32_t minPosition, int32_t hourPosition);
void clASetTargetRelative(uint8_t i, uint8_t j, int32_t minRelative, int32_t hourRelative);
void clAInit(float maxSpeed, int32_t minPosition, int32_t hourPosition);
void clARunToTargetAcceleration(void);
void clARunToTargetSpeed(void);
int32_t clAGetStepperPositionHour(uint8_t i, uint8_t j);
int32_t clAGetStepperPositionMin(uint8_t i, uint8_t j);
bool clATargetsReached(void);
bool clATargetReached(uint8_t i, uint8_t j);
void clRunSteppers(void);
void clARefractorMaxSpeed(float maxspeed);
int32_t clAGetNumberofTurnHour(uint8_t i, uint8_t j);
int32_t clAGetNumberofTurnMin(uint8_t i, uint8_t j);
void clASetMaxSpeed(uint8_t i, uint8_t j, float minSpeed, float hourSpeed);
#endif
