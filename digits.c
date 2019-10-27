#include "digits.h"

digitS digitAZero[2][3] = { //
		{ //
				{ 0, 512 }, //
						{ 1536, 512 }, //
						{ 1536, 0 } //
				}, //
				{ { 1024, 512 }, //
						{ 1536, 512 }, //
						{ 1536, 1024 } //
				}
		//
		};

digitS digitAOne[2][3] = { //
		{ //
				{ 1280, 1280 }, //
						{ 1280, 1280 }, //
						{ 1280, 1280 } //
				}, //
				{ { 512, 512 }, //
						{ 1536, 512 }, //
						{ 1536, 1536 } }
		//
		};
digitS digitATwo[2][3] = { //
		{ //
				{ 0, 512 }, //
						{ 1536, 0 }, //
						{ 0, 0 } //
				}, //
				{ { 1024, 1024 }, //
						{ 1024, 512 }, //
						{ 1024, 1536 } }
		//
		};

digitS digitAThree[2][3] = { //
		{ //
				{ 0, 0 }, //
						{ 0, 0 }, //
						{ 0, 0 } //
				}, //
				{ { 1024, 512 }, //
						{ 1024, 512 }, //
						{ 1024, 1536 } }
//
		};
digitS digitAFour[2][3] = { //
		{ //
				{ 1280, 1280 }, //
						{ 0, 512 }, //
						{ 1536, 1536 } //
				}, //
				{ { 512, 512 }, //
						{ 1536, 512 }, //
						{ 1536, 1536 } }
//
		};
digitS digitAFive[2][3] = { //
		{ //
				{ 0, 0 }, //
						{ 512, 0 }, //
						{ 1536, 0 } //
				}, //
				{ { 1024, 512 }, //
						{ 1536, 1024 }, //
						{ 1024, 1024 } }
		//
		};
digitS digitASix[2][3] = { //
		{ //

				{ 512, 0 }, //
						{ 1536, 512 }, //
						{ 1536, 0 } //
				}, { { 1024, 512 }, //
						{ 1536, 1024 }, //
						{ 1024, 1024 } } //
				//
		};
digitS digitASeven[2][3] = { //
		{ //
				{ 1280, 1280 }, //
						{ 1280, 1280 }, //
						{ 0, 0 } //
				}, //
				{ { 512, 512 }, //
						{ 1536, 512 }, //
						{ 1024, 1536 } }
		//
		};

digitS digitAEight[2][3] = { //
		{ //
				{ 0, 512 }, //
						{ 0, 512 }, //
						{ 1536, 0 } //
				}, //
				{ { 512, 1024 }, //
						{ 512, 1024 }, //
						{ 1536, 1024 } }
		//
		};
digitS digitANine[2][3] = { //
		{ //
				{ 0, 0 }, //
						{ 512, 0 }, //
						{ 1536, 0 } //
				}, //
				{ { 512, 1024 }, //
						{ 1536, 512 }, //
						{ 1024, 1536 } }
		//
		};


void diSetDigits(uint8_t position, uint8_t value) {
	if (position > CLOCKCOL-2)
		return;

	digitS (*digitArray)[2][3];
	digitArray = &digitAZero;

	switch (value) {
	case 0:
		digitArray = &digitAZero;
		break;
	case 1:
		digitArray = &digitAOne;
		break;
	case 2:
		digitArray = &digitATwo;
		break;
	case 3:
		digitArray = &digitAThree;
		break;
	case 4:
		digitArray = &digitAFour;
		break;
	case 5:
		digitArray = &digitAFive;
		break;
	case 6:
		digitArray = &digitASix;
		break;
	case 7:
		digitArray = &digitASeven;
		break;
	case 8:
		digitArray = &digitAEight;
		break;
	case 9:
		digitArray = &digitANine;
		break;
	default:
		break;
	}

	for (uint8_t j = 0; j < 3; j++) {
		clASetTargetPosition(0+position, j, (*digitArray)[0][j].minPosition, (*digitArray)[0][j].hourPosition);
		clASetTargetPosition(1+position, j, (*digitArray)[1][j].minPosition, (*digitArray)[1][j].hourPosition);
	}
}


void diSetDigitsOffset(uint8_t position, uint8_t value, int8_t turnOffset) {
	if (position > CLOCKCOL-2)
		return;

	digitS (*digitArray)[2][3];
	digitArray = &digitAZero;

	switch (value) {
	case 0:
		digitArray = &digitAZero;
		break;
	case 1:
		digitArray = &digitAOne;
		break;
	case 2:
		digitArray = &digitATwo;
		break;
	case 3:
		digitArray = &digitAThree;
		break;
	case 4:
		digitArray = &digitAFour;
		break;
	case 5:
		digitArray = &digitAFive;
		break;
	case 6:
		digitArray = &digitASix;
		break;
	case 7:
		digitArray = &digitASeven;
		break;
	case 8:
		digitArray = &digitAEight;
		break;
	case 9:
		digitArray = &digitANine;
		break;
	default:
		break;
	}

	for (uint8_t j = 0; j < 3; j++) {
		clASetTargetPosition(0+position, j, (*digitArray)[0][j].minPosition+turnOffset*CLOCKRESOLUTION, (*digitArray)[0][j].hourPosition+turnOffset*CLOCKRESOLUTION);
		clASetTargetPosition(1+position, j, (*digitArray)[1][j].minPosition+turnOffset*CLOCKRESOLUTION, (*digitArray)[1][j].hourPosition+turnOffset*CLOCKRESOLUTION);
	}
}
