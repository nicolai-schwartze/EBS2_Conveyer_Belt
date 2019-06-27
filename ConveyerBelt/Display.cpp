#include "ConveyerBelt.h"

Display :: Display() {
	initDisplay();
	return;
}

void Display ::initDisplay(){
	void dispHrdInit ();
	int row = 2;
	writeToDisplay(1,30+2, "ConveyerBelt");
	writeToDisplay(1*row+2,5, "maximum Velocity: ");
	writeToDisplay(2*row+2,5, "current mode: ");
	writeToDisplay(2*row+2,25 ,"Stop");
	writeToDisplay(3*row+2,5, "current state: ");
	writeToDisplay(4*row+2,5, "direction: ");
	writeToDisplay(5*row+2,5, "slow time: ");
	writeToDisplay(6*row+2,5, "rise time: ");
	writeToDisplay(7*row+2,5, "total time: ");
	writeToDisplay(8*row+2,5, "fall time: ");
//	writeToDisplay(9*row+2,5, "actual time: ");
}

void Display::writeParameter(int row, char value[]){
	char tempstring[30] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '} ;
	writeToDisplay(2*row+2,25 ,tempstring);
	writeToDisplay(2*row+2,25 ,value);	
}
