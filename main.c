/*
  ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#include "hal.h"
#include "ch.h"

#define cputs(msg) chMsgSend(cdtp, (msg_t)msg)

thread_t *cdtp;  //The console thread pointer

#include "shell.h"
//#include "chprintf.h"
#include  "animations.h"
#include "Smalltime.h"
#include "accelStepper.h"
#include "clock.h"
#include "drawClock.h"
#include "drawClockX11.h"
#include "digits.h"

#include<GL/glut.h>
#include<GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static THD_WORKING_AREA(waThreadStepper, 4096);
static THD_WORKING_AREA(waThreadAnimation, 4096);
static THD_WORKING_AREA(waThreadPrint, 4096);

#define SHELL_WA_SIZE       THD_WORKING_AREA_SIZE(4096)
#define CONSOLE_WA_SIZE     THD_WORKING_AREA_SIZE(4096)
#define TEST_WA_SIZE        THD_WORKING_AREA_SIZE(4096)

binary_semaphore_t bSemaStepperDone;
binary_semaphore_t bSemaStepperGo;

/*
 * Stepper Threads
 */

static __attribute__((noreturn)) THD_FUNCTION(Stepperthread, arg) {
    (void) arg;
    chRegSetThreadName("stepper");
    //Initialize all steppers
    clAInit(200, 0, 0);

    //chBSemSignal(&bSemaStepperDone);
    while (true) {
        clARunToTargetAcceleration();
        if (clATargetsReached() == true) {
            //Turn off all motors
            //Signal the animation thread we have reached destination
            chBSemSignal(&bSemaStepperDone);
            chBSemWait(&bSemaStepperGo);
            //cputs("Out");

        }
        chThdSleep(1);

    }                       //
}

/*
 * Print Threads
 */

static __attribute__((noreturn)) THD_FUNCTION(Printthread, arg) {
    (void) arg;
    chRegSetThreadName("print");
    char buff[32];
    while (true) {

        sprintf(buff, "Sm: %d", clockA[0][0].minStepper._currentPos);
        cputs(buff);
        sprintf(buff, "Sh: %d", clockA[0][0].hourStepper._currentPos);
        cputs(buff);
        sprintf(buff, "diff: %d", clockA[0][0].hourStepper._currentPos - clockA[0][0].minStepper._currentPos);
        cputs(buff);

        chThdSleepMilliseconds(1000);
    }
}
/*
 * Drawing thread.
 */

static THD_FUNCTION(drawingThread, arg) {
    (void) arg;

    cairo_surface_t *sfc;
    cairo_t *ctx;
    sfc = dcInitDisplayX();
    ctx = cairo_create(sfc);

    while (true) {
        //dcDrawClocks();
        dcDrawClocksX(ctx,sfc);
        chThdSleepMilliseconds(50);

    }
}

static THD_FUNCTION(animationThread, arg) {
    (void) arg;

    while (true) {

      //aniCounter(&bSemaStepperDone, &bSemaStepperGo);
        //cputs("Anim");

        //aniSquares(&bSemaStepperDone, &bSemaStepperGo);
        //      aniStandardTime(&bSemaStepperDone,&bSemaStepperGo);
        
        //aniStairs(&bSemaStepperDone,&bSemaStepperGo);
        //aniFlower(&bSemaStepperDone, &bSemaStepperGo);
        aniSpiral(&bSemaStepperDone,&bSemaStepperGo);
        //aniStandardTimeOff(&bSemaStepperDone,&bSemaStepperGo);
        //aniTriangle(&bSemaStepperDone,&bSemaStepperGo);
        //      aniTriangle
        //      aniCounter(&bSemaStepperDone, &bSemaStepperGo);
      // aniRotate(&bSemaStepperDone);
        //chThdSleepMilliseconds(2000);

    }
}

/*
 * Console print server done using synchronous messages. This makes the access
 * to the C printf() thread safe and the print operation atomic among threads.
 * In this example the message is the zero terminated string itself.
 */

static THD_FUNCTION(console_thread, arg) {

    (void) arg;
    while (!chThdShouldTerminateX()) {
        thread_t *tp = chMsgWait();
        puts((char *) chMsgGet(tp));
        fflush(stdout);
        chMsgRelease(tp, MSG_OK);
    }
}

static evhandler_t fhandlers[] = { };

/*------------------------------------------------------------------------*
 * Simulator main.                                                        *
 *------------------------------------------------------------------------*/

int main(int argc, char**argv) {

    //dcInitDisplay(argc, argv);

    halInit();
    chSysInit();
    chBSemObjectInit(&bSemaStepperDone, true);
    chBSemObjectInit(&bSemaStepperGo, true);
    /*
     * Console thread started.
     */
    cdtp = chThdCreateFromHeap(NULL, CONSOLE_WA_SIZE, "console", NORMALPRIO - 1, console_thread, NULL);
    chThdCreateStatic(waThreadAnimation, sizeof(waThreadAnimation), NORMALPRIO, animationThread, NULL);
    /***Stepper thread*/
    chThdCreateStatic(waThreadStepper, sizeof(waThreadStepper), NORMALPRIO + 2, Stepperthread, NULL);

    /***Drawing Thread*/
    chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(4096), "draw", NORMALPRIO + 1, drawingThread, NULL);
    /**print stuff thread
     *
     */
    //chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(4096), "print", NORMALPRIO + 1, Printthread, NULL);
    /*
     * Events servicing loop.
     */
    while (!chThdShouldTerminateX())
        chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
    return 0;
}
