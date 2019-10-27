#ifndef DRAWCLOCKCX11_H
#define DRAWCLOCKCX11_H

#include "clock.h"
#include "accelStepper.h"
#include  <math.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CLOCKCOLONME 8
#define CLOCKROWS	 3
#define CLOCKWIDTH   128  //Size in PIXELS OF CLOCK
#define CLOCKHANDWITH 8

cairo_surface_t  *dcInitDisplayX();
void dcDrawClocksX(cairo_t *ctx,cairo_surface_t *sfc);

#endif
