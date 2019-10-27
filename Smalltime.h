#ifndef SMALLTIME_H
#define SMALLTIME_H

#include "hal.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

typedef struct {
	uint32_t hours;
	uint32_t minutes;
	uint32_t seconds;
} SmallTime;

typedef struct {
	uint8_t hourTenth;
	uint8_t hourDigit;
	uint8_t minTenth;
	uint8_t minDigit;
} SmallTimeDigits;



void SmallTimeGet(SmallTime *t);
void SmallTimeGetDigits(SmallTime *t,SmallTimeDigits *d);
void SmallTimeSet(uint32_t hours, uint32_t minutes, uint32_t seconds);
uint32_t SmallTimeGetMicroSec(void);

#endif
