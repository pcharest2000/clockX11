#include "Smalltime.h"

void SmallTimeGet(SmallTime *t) {

	time_t t2 = time(NULL);
	struct tm tm = *localtime(&t2);

	t->seconds = (uint32_t) tm.tm_sec;

	t->minutes = (uint32_t) tm.tm_min;

	t->hours = (uint32_t) tm.tm_hour;
}

uint32_t SmallTimeGetMicroSec(void) {

	struct timeval tv;
	//struct timezone tz;
	struct tm *tm;
	gettimeofday(&tv, NULL);
	//tm=localtime(&tv.tv_usec);
	return tv.tv_usec;

}



void SmallTimeSet(uint32_t hours, uint32_t minutes, uint32_t seconds) {

	if (hours > 23 || minutes > 59 || seconds > 59) {
		return;
	}
	uint32_t outputSeconds = hours * 3600 + minutes * 60 + seconds;
	#ifndef SIMULATOR
	rtcSTM32SetSec(&RTCD1, outputSeconds);
	#endif
}

void SmallTimeGetDigits(SmallTime *t,SmallTimeDigits *d){

	d->hourDigit=(t->hours%10);
	d->hourTenth=(t->hours/10);

	d->minDigit=(t->minutes%10);
	d->minTenth=(t->minutes/10);


}
