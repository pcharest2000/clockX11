#include "drawClock.h"

const GLint rectS[2] = { 0, -CLOCKHANDWITH / 2 };
const GLint rectE[2] = { CLOCKWIDTH / 2 - 6, CLOCKHANDWITH / 2 };
GLUquadricObj *diskClockRing;

void dcSetPerspective(void) {
	glViewport(0, 0, CLOCKCOLONME * CLOCKWIDTH, CLOCKROWS * CLOCKWIDTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, CLOCKCOLONME * CLOCKWIDTH, 0, CLOCKROWS * CLOCKWIDTH);
	glMatrixMode(GL_MODELVIEW);
}

void dcInitDisplay(int argc, char**argv) {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(CLOCKCOLONME * CLOCKWIDTH, CLOCKROWS * CLOCKWIDTH);
	glutCreateWindow("Hello World");
	dcSetPerspective();
	diskClockRing = gluNewQuadric();
	//glClearColor( 	1,1,1,0);
}

void dcDrawClocks(void) {
	float angleMin;
	float angleHour;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	for (uint8_t i = 0; i < CLOCKCOL; i++) {
		for (uint8_t j = 0; j < CLOCKROW; j++) {
			//	angleMin = clockA[i][j].minStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
			//	angleHour = clockA[i][j].hourStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
			angleMin = (float)clAGetStepperPositionMin(i, j) / (STEPSPERREVOLUTION * GEARRATIO) * 360;
			angleHour = (float)clAGetStepperPositionHour(i, j) / (STEPSPERREVOLUTION * GEARRATIO) * 360;

			glLoadIdentity();
			// rotation about Z axis
		//	glColor3f(1, 1, 1);
			glTranslatef(CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * j, 0);
			gluDisk(diskClockRing, CLOCKWIDTH / 2 - 1, CLOCKWIDTH / 2, 64, 64);
		//	glColor3f(0, 0, 1);
			glRotatef(angleMin, 0.0, 0.0, 1.0);
			glRectiv(rectS, rectE);
			glLoadIdentity();
			glTranslatef(CLOCKWIDTH / 2 + CLOCKWIDTH * i, CLOCKWIDTH / 2 + CLOCKWIDTH * j, 0);
		//	glColor3f(0, 1, 0);
			glRotatef(angleHour, 0.0, 0.0, 1.0);
			glRectiv(rectS, rectE);

		}
	}
	glFinish();
	glutSwapBuffers();
}

void dcDrawClock(clockS *cl, uint32_t x, uint32_t y) {

	float angleMin = cl->minStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
	float angleHour = cl->hourStepper._currentPos / (STEPSPERREVOLUTION * GEARRATIO) * 360;
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(3);
	glColor3f(1, 1, 1);
	glLoadIdentity();
	// rotation about Z axis
	glTranslatef(CLOCKWIDTH / 2, CLOCKWIDTH / 2, 0);
	gluDisk(diskClockRing, CLOCKWIDTH / 2 - 2, CLOCKWIDTH / 2, 64, 64);
	glRotatef(angleMin, 0.0, 0.0, 1.0);
	glRectiv(rectS, rectE);
	glLoadIdentity();
	glTranslatef(CLOCKWIDTH / 2, CLOCKWIDTH / 2, 0);
	glRotatef(angleHour, 0.0, 0.0, 1.0);
	glRectiv(rectS, rectE);
//	glBegin(GL_POINTS);
//	glColor3f(1, 0, 0);
//	glVertex2f(0, 0);
//	glVertex2f(CLOCKWIDTH/2, 0); // set vertex color to red
//	glEnd();
	glFinish();
	glutSwapBuffers();
}

