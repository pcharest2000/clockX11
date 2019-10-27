#ifndef DIGITS_H
#define DIGITS_H

#include "clock.h"
#include  <math.h>

 typedef struct {
	int32_t minPosition;		//Positions are mapped from 0-360 deg from 0-2048
	int32_t hourPosition;

} digitS;

void diSetDigits(uint8_t position, uint8_t value);
void diSetDigitsOffset(uint8_t position, uint8_t value, int8_t turnOffset);
#endif
