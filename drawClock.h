#ifndef DRAWCLOCK_H
#define DRAWCLOCK_H

#include "clock.h"
#include "accelStepper.h"
#include  <math.h>
#include<GL/glut.h>
#include<GL/gl.h>

#define CLOCKCOLONME 8
#define CLOCKROWS        3
#define CLOCKWIDTH   128  //Size in PIXELS OF CLOCK
#define CLOCKHANDWITH 8

void dcInitDisplay(int argc, char**argv);
void dcDrawClock(clockS *cl,uint32_t x,uint32_t y);
void dcDrawClocks(void);
#endif
