#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "Smalltime.h"
#include "clock.h"
#include "digits.h"
#include "ch.h"
void aniStandardTime(binary_semaphore_t *bSemaStepperDone,binary_semaphore_t *bSemaWait);
void aniStandardTimeOff(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniRotate(binary_semaphore_t *bSemaStepperDone,binary_semaphore_t *bSemaWait);
void animationCounter(binary_semaphore_t *bSemaStepperDone,thread_t *cdtp);
void aniCounter(binary_semaphore_t *bSemaStepperDone,binary_semaphore_t *bSemaWait);
void aniSpiral(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniDiamonds(binary_semaphore_t *bSemaStepperDone,binary_semaphore_t *bSemaWait);
void aniDiamonds2(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniTriangle(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniSquares(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniStairs(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
void aniFlower(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo);
#endif
