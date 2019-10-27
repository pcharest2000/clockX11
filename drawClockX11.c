#include "drawClockX11.h"

cairo_surface_t *dcInitDisplayX(void) {

	int x = CLOCKWIDTH * CLOCKCOL;
	int y = CLOCKWIDTH * CLOCKROW;
	Display *dsp;
	Drawable da;
	Screen *scr;
	int screen;
	cairo_surface_t *sfc;

	if ((dsp = XOpenDisplay(NULL)) == NULL)
		exit(1);
	screen = DefaultScreen(dsp);
	scr = DefaultScreenOfDisplay(dsp);
	if (!x || !y) {
		x = WidthOfScreen(scr), y = HeightOfScreen(scr);

		da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, x, y, 0, 0, 0);

		fullscreen(dsp, da);
	} else
		da = XCreateSimpleWindow(dsp, DefaultRootWindow(dsp), 0, 0, x, y, 0, 0, 0);

	XSelectInput(dsp, da, ButtonPressMask | KeyPressMask);
	XMapWindow(dsp, da);
	sfc = cairo_xlib_surface_create(dsp, da, DefaultVisual(dsp, screen), x, y);
	cairo_xlib_surface_set_size(sfc, x, y);

	return sfc;
}

void dcDrawClocksX(cairo_t *ctx, cairo_surface_t *sfc) {
	float angleMin;
	float angleHour;
	cairo_push_group(ctx);
	cairo_set_source_rgb(ctx, 0, 0, 0);
	cairo_paint(ctx);
	cairo_set_source_rgb(ctx, 1, 1, 1);
	cairo_set_line_width(ctx, 1.0);
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			//	angleMin = clockA[i][j].minStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
			//	angleHour = clockA[i][j].hourStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
			angleMin = -(float) clAGetStepperPositionMin(i, j) / (STEPSPERREVOLUTION * GEARRATIO) * 2*M_PI;
			angleHour = -(float) clAGetStepperPositionHour(i, j) / (STEPSPERREVOLUTION * GEARRATIO) * 2*M_PI;
			cairo_identity_matrix(ctx);
			cairo_translate(ctx, CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * (2-j));
			cairo_arc(ctx, 0, 0, CLOCKWIDTH / 2, 0, 2 * M_PI);
			cairo_stroke(ctx);

			cairo_rotate(ctx, angleMin);
			cairo_rectangle(ctx, 0,  -CLOCKHANDWITH / 2, CLOCKWIDTH / 2, CLOCKHANDWITH);
			cairo_identity_matrix(ctx);
			cairo_translate(ctx, CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * (2-j));
			cairo_rotate(ctx, angleHour);
			cairo_rectangle(ctx, 0,  -CLOCKHANDWITH / 2, CLOCKWIDTH / 2, CLOCKHANDWITH);

			cairo_fill(ctx);

//
//			glLoadIdentity();
//			// rotation about Z axis
//		//	glColor3f(1, 1, 1);
//			glTranslatef(CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * j, 0);
//			gluDisk(diskClockRing, CLOCKWIDTH / 2 - 1, CLOCKWIDTH / 2, 64, 64);
//		//	glColor3f(0, 0, 1);
//			glRotatef(angleMin, 0.0, 0.0, 1.0);
//			glRectiv(rectS, rectE);
//			glLoadIdentity();
//			glTranslatef(CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * j, 0);
//		//	glColor3f(0, 1, 0);
//			glRotatef(angleHour, 0.0, 0.0, 1.0);
//			glRectiv(rectS, rectE);

		}
	}
	cairo_pop_group_to_source(ctx);
	cairo_paint(ctx);
	cairo_surface_flush(sfc);
}
