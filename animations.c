#include "animations.h"

SmallTime myTime;
SmallTimeDigits myTimeDigits;

void aniStandardTime(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	static uint8_t prevmin = 0;

	SmallTimeGet(&myTime);
	if (myTime.minutes != prevmin) {
		prevmin = myTime.minutes;
		SmallTimeGetDigits(&myTime, &myTimeDigits);
		diSetDigits(0, myTimeDigits.hourTenth);
		diSetDigits(2, myTimeDigits.hourDigit);
		diSetDigits(4, myTimeDigits.minTenth);
		diSetDigits(6, myTimeDigits.minDigit);
		chBSemSignal(bSemaStepperGo); //We tell the steeper thread that it can start doing it stuff
	}
}

void aniStandardTimeOff(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	static uint8_t prevmin = 0;
	static uint8_t turns = 1;
	SmallTimeGet(&myTime);
	if (myTime.minutes != prevmin) {
		prevmin = myTime.minutes;
		SmallTimeGetDigits(&myTime, &myTimeDigits);
		diSetDigitsOffset(0, myTimeDigits.hourTenth, -turns);
		diSetDigitsOffset(2, myTimeDigits.hourDigit, turns);
		diSetDigitsOffset(4, myTimeDigits.minTenth, -turns);
		diSetDigitsOffset(6, myTimeDigits.minDigit, turns);
		turns = turns * -1;
		chBSemSignal(bSemaStepperGo); //We tell the steeper thread that it can start doing it stuff
	}
}

#define cputs(msg) chMsgSend(cdtp, (msg_t)msg)

void aniCounter(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	static uint8_t counter = 0;
	//char buff[32];

	diSetDigits(0, counter);
	diSetDigits(2, counter);
	diSetDigits(4, counter);
	diSetDigits(6, counter);
	clARefractorMaxSpeed(200);
	chBSemSignal(bSemaStepperGo);
	counter++;
	if (counter > 9)
		counter = 0;
//	chBSemWait(bSemaStepperDone);
}

void aniRotate(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	clASetAllTargetpostion(256, 1280);

	const int32_t rotationTicks = 4;
	chBSemWait(bSemaStepperDone);
	clASetAllTargetpostion(256 + rotationTicks * ONETURN, 1280 + rotationTicks * ONETURN);
	chBSemWait(bSemaStepperDone);
}

void aniTriangle(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	clASetTargetPosition(0, 0, CL0DEG, CL90DEG);
	clASetTargetPosition(1, 0, CL0DEG, CL180DEG);
	clASetTargetPosition(2, 0, CL180DEG, CL135DEG);
	clASetTargetPosition(0, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1, 1, CL135DEG, CL315DEG);
	clASetTargetPosition(0, 2, CL270DEG, CL315DEG);

	clASetTargetPosition(0 + 4, 0, CL0DEG, CL90DEG);
	clASetTargetPosition(1 + 4, 0, CL0DEG, CL180DEG);
	clASetTargetPosition(2 + 4, 0, CL180DEG, CL135DEG);
	clASetTargetPosition(0 + 4, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1 + 4, 1, CL135DEG, CL315DEG);
	clASetTargetPosition(0 + 4, 2, CL270DEG, CL315DEG);

	clASetTargetPosition(3, 0, CL90DEG, CL135DEG);
	clASetTargetPosition(2, 1, CL135DEG, CL315DEG);
	clASetTargetPosition(3, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1, 2, CL0DEG, CL315DEG);
	clASetTargetPosition(2, 2, CL0DEG, CL180DEG);
	clASetTargetPosition(3, 2, CL180DEG, CL270DEG);

	clASetTargetPosition(3 + 4, 0, CL90DEG, CL135DEG);
	clASetTargetPosition(2 + 4, 1, CL135DEG, CL315DEG);
	clASetTargetPosition(3 + 4, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1 + 4, 2, CL0DEG, CL315DEG);
	clASetTargetPosition(2 + 4, 2, CL0DEG, CL180DEG);
	clASetTargetPosition(3 + 4, 2, CL180DEG, CL270DEG);
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	clASetAllTargetpostion(0, 0);

	clASetTargetPosition(0, 0, CL45DEG, CL90DEG);
	clASetTargetPosition(0, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1, 1, CL45DEG, CL225DEG);
	clASetTargetPosition(0, 2, CL0DEG, CL270DEG);
	clASetTargetPosition(1, 2, CL0DEG, CL180DEG);
	clASetTargetPosition(2, 2, CL180DEG, CL225DEG);

	clASetTargetPosition(0 + 4, 0, CL45DEG, CL90DEG);
	clASetTargetPosition(0 + 4, 1, CL90DEG, CL270DEG);
	clASetTargetPosition(1 + 4, 1, CL45DEG, CL225DEG);
	clASetTargetPosition(0 + 4, 2, CL0DEG, CL270DEG);
	clASetTargetPosition(1 + 4, 2, CL0DEG, CL180DEG);
	clASetTargetPosition(2 + 4, 2, CL180DEG, CL225DEG);

	chBSemSignal(bSemaStepperGo);

}

void aniSpiral(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	clASetAllTargetpostion(256, 1280);
	chBSemSignal(bSemaStepperGo);
	const int32_t posIncrement = 100;
	const int32_t numTurns = 1;
	chBSemWait(bSemaStepperDone);
	//Set the first clock to new position
	clASetTargetPosition(0, 0, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);
	clASetTargetPosition(0, 1, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);
	clASetTargetPosition(0, 2, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);
	chBSemSignal(bSemaStepperGo);

	for (uint8_t i = 1; i < CLOCKCOL; i++) {
		chThdSleepMilliseconds(300);
		clASetTargetPosition(i, 0, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);
		clASetTargetPosition(i, 1, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);
		clASetTargetPosition(i, 2, 256 + numTurns * ONETURN, 1280 + numTurns * ONETURN);

	}
	chBSemWait(bSemaStepperDone);

	//Set the last clock to new position
	clASetTargetPosition(CLOCKCOL - 1, 0, 256, 1280);
	clASetTargetPosition(CLOCKCOL - 1, 1, 256, 1280);
	clASetTargetPosition(CLOCKCOL - 1, 2, 256, 1280);
	chBSemSignal(bSemaStepperGo);

	for (uint8_t i = 1; i < CLOCKCOL; i++) {
		chThdSleepMilliseconds(300);
		clASetTargetPosition(CLOCKCOL - 1 - i, 0, 256, 1280);
		clASetTargetPosition(CLOCKCOL - 1 - i, 1, 256, 1280);
		clASetTargetPosition(CLOCKCOL - 1 - i, 2, 256, 1280);

	}

}

void aniDiamonds(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	chBSemSignal(bSemaStepperGo);
	const int32_t Turns = 2048 / 2;
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			if (j & 1) {

				if (i & 1) {
					clASetTargetPosition(i, j, CL225DEG, -CL45DEG);
				} else {
					clASetTargetPosition(i, j, CL135DEG, CL45DEG);
				}
			} else {
				if (i & 1) {
					clASetTargetPosition(i, j, CL135DEG, CL45DEG);

				} else {
					clASetTargetPosition(i, j, CL225DEG, -CL45DEG);
				}

			}
		}
	}
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);
	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			if (j & 1) {

				if (i & 1) {
					clASetTargetPosition(i, j, 1280 - Turns, -256 + Turns);
				} else {
					clASetTargetPosition(i, j, 768 + Turns, 256 - Turns);
				}
			} else {
				if (i & 1) {
					clASetTargetPosition(i, j, 768 + Turns, 256 - Turns);

				} else {
					clASetTargetPosition(i, j, 1280 - Turns, -256 + Turns);
				}

			}
		}
	}
	chBSemSignal(bSemaStepperGo);
	aniStandardTime(bSemaStepperDone, bSemaStepperGo);

}

void aniDiamonds2(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	chBSemSignal(bSemaStepperGo);
	const int32_t Turns = 2048 / 2;
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			if (j & 1) {

				if (i & 1) {
					clASetTargetPosition(i, j, CL225DEG, -CL45DEG);
				} else {
					clASetTargetPosition(i, j, CL135DEG, CL45DEG);
				}
			} else {
				if (i & 1) {
					clASetTargetPosition(i, j, CL135DEG, CL45DEG);

				} else {
					clASetTargetPosition(i, j, CL225DEG, -CL45DEG);
				}

			}
		}
	}
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);
	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			if (j & 1) {

				if (i & 1) {
					clASetTargetPosition(i, j, CL225DEG + ONETURN, -CL45DEG + ONETURN);
					clASetMaxSpeed(i, j, 150, 150);
				} else {
					clASetTargetPosition(i, j, CL135DEG - ONETURN, CL45DEG - ONETURN);
					clASetMaxSpeed(i, j, 200, 200);
				}
			} else {

				if (i & 1) {
					clASetTargetPosition(i, j, CL135DEG + ONETURN, CL45DEG + ONETURN);
					clASetMaxSpeed(i, j, 150, 150);

				} else {
					clASetTargetPosition(i, j, CL225DEG - ONETURN, -CL45DEG - ONETURN);
					clASetMaxSpeed(i, j, 200, 200);
				}

			}
		}
	}
	chBSemSignal(bSemaStepperGo);
	aniStandardTime(bSemaStepperDone, bSemaStepperGo);

}

void aniSquares(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	chBSemSignal(bSemaStepperGo);
	const int32_t Turns = ONETURN / 2;
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	uint8_t steps = 0;

	while (steps < 4) {

		for (uint8_t j = 0; j < CLOCKROW; j++) {
			for (uint8_t i = 0; i < CLOCKCOL; i++) {
				if (j & 1) {

					if (i & 1) {
						clASetTargetPosition(i, j, CL180DEG + CL90DEG * steps, -CL90DEG + CL90DEG * steps);
					} else {
						clASetTargetPosition(i, j, CL0DEG - CL90DEG * steps, -CL90DEG - CL90DEG * steps);
					}
				} else {
					if (i & 1) {
						clASetTargetPosition(i, j, CL180DEG - CL90DEG * steps, CL90DEG - CL90DEG * steps);

					} else {
						clASetTargetPosition(i, j, CL0DEG + CL90DEG * steps, CL90DEG + CL90DEG * steps);
					}

				}
			}
		}
		chBSemSignal(bSemaStepperGo);
		chBSemWait(bSemaStepperDone);
		steps++;

		chThdSleepMilliseconds(2000);

	}

	while (steps != 0) {

		for (uint8_t j = 0; j < CLOCKROW; j++) {
			for (uint8_t i = 0; i < CLOCKCOL; i++) {
				if (j & 1) {

					if (i & 1) {
						clASetTargetPosition(i, j, CL180DEG + CL90DEG * steps, -CL90DEG + CL90DEG * steps);
					} else {
						clASetTargetPosition(i, j, CL0DEG - CL90DEG * steps, -CL90DEG - CL90DEG * steps);
					}
				} else {
					if (i & 1) {
						clASetTargetPosition(i, j, CL180DEG - CL90DEG * steps, CL90DEG - CL90DEG * steps);

					} else {
						clASetTargetPosition(i, j, CL0DEG + CL90DEG * steps, CL90DEG + CL90DEG * steps);
					}

				}
			}
		}
		chBSemSignal(bSemaStepperGo);
		steps--;
		chBSemWait(bSemaStepperDone);
		chThdSleepMilliseconds(1000);
	}

}

void aniStairs(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	int32_t minangle = CL0DEG;
	int32_t hourangle = CL0DEG;

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			if (j & 1) {
				if (i & 1) {
					clASetTargetPosition(i, j, CL90DEG, CL180DEG);
				} else {
					clASetTargetPosition(i, j, CL0DEG, CL270DEG);
				}
			} else {
				if (i & 1) {
					clASetTargetPosition(i, j, CL0DEG, CL270DEG);

				} else {
					clASetTargetPosition(i, j, CL90DEG, CL180DEG);
				}

			}
		}
	}
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
		}
	}
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
		}
	}
	chBSemSignal(bSemaStepperGo);
	chBSemSignal(bSemaStepperGo);
	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	for (uint8_t j = 0; j < CLOCKROW; j++) {
		for (uint8_t i = 0; i < CLOCKCOL; i++) {
			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
		}
	}
	chBSemSignal(bSemaStepperGo);

}
void aniFlower(binary_semaphore_t *bSemaStepperDone, binary_semaphore_t *bSemaStepperGo) {
	int32_t minangle = CL0DEG;
	int32_t hourangle = CL0DEG;

	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done

	clASetAllTargetpostion(CL90DEG, -CL90DEG);
	chBSemSignal(bSemaStepperGo);

	chBSemWait(bSemaStepperDone);
	float speed = 120;
	for (uint8_t i = 0; i < CLOCKCOL / 2; i++) {
		clASetMaxSpeed(3 - i, 1, speed, speed);
		clASetMaxSpeed(4 + i, 1, speed, speed);
		clASetTargetPosition(3 - i, 1, CL90DEG + 2 * ONETURN , -CL90DEG - 2 * ONETURN);
		clASetTargetPosition(4 + i, 1, CL90DEG - 2 * ONETURN , -CL90DEG + 2 * ONETURN);
		clASetMaxSpeed(3 - i, 0, speed - 10, speed - 10);
		clASetMaxSpeed(4 + i, 0, speed - 10, speed - 10);
		clASetTargetPosition(3 - i, 0, CL90DEG + 2 * ONETURN , -CL90DEG - 2 * ONETURN);
		clASetTargetPosition(4 + i, 0, CL90DEG - 2 * ONETURN , -CL90DEG + 2 * ONETURN);
		clASetMaxSpeed(3 - i, 2, speed - 10, speed - 10);
		clASetMaxSpeed(4 + i, 2, speed - 10, speed - 10);
		clASetTargetPosition(3 - i, 2, CL90DEG + 2 * ONETURN , -CL90DEG - 2 * ONETURN);
		clASetTargetPosition(4 + i, 2, CL90DEG - 2 * ONETURN , -CL90DEG + 2 * ONETURN);
		speed -= 10;
	}
	chBSemSignal(bSemaStepperGo);


//	for (uint8_t j = 0; j < CLOCKROW; j++) {
//		for (uint8_t i = 0; i < CLOCKCOL; i++) {
//			if (j & 1) {
//				if (i & 1) {
//					clASetTargetPosition(i, j, CL90DEG, CL180DEG);
//				} else {
//					clASetTargetPosition(i, j, CL0DEG, CL270DEG);
//				}
//			} else {
//				if (i & 1) {
//					clASetTargetPosition(i, j, CL0DEG, CL270DEG);
//
//				} else {
//					clASetTargetPosition(i, j, CL90DEG, CL180DEG);
//				}
//
//			}
//		}
//	}
//	chBSemSignal(bSemaStepperGo);
//	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
//
//	for (uint8_t j = 0; j < CLOCKROW; j++) {
//		for (uint8_t i = 0; i < CLOCKCOL; i++) {
//			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
//		}
//	}
//	chBSemSignal(bSemaStepperGo);
//	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
//
//	for (uint8_t j = 0; j < CLOCKROW; j++) {
//		for (uint8_t i = 0; i < CLOCKCOL; i++) {
//			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
//		}
//	}
//	chBSemSignal(bSemaStepperGo);
//	chBSemSignal(bSemaStepperGo);
//	chBSemWait(bSemaStepperDone);  //Wait for the stepper loop to be done
//
//	for (uint8_t j = 0; j < CLOCKROW; j++) {
//		for (uint8_t i = 0; i < CLOCKCOL; i++) {
//			clASetTargetRelative(i, j, CL90DEG, CL90DEG);
//		}
//	}
//	chBSemSignal(bSemaStepperGo);

}

