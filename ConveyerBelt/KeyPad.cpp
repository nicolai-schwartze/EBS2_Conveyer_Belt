#include "SystemManager.h"
#include "stateMachine.h"


char globalKey = 0;

KeyPad::KeyPad(SystemManager *CB){ 
	myConveyerBelt = CB;
	lastCommand = 0;
	return;
}	

void fReadKey (KeyPad* myKeyPad) {
	char tempKey = 0;
	while (1) {
		tempKey = getKey();
		if (tempKey != myKeyPad->getLastCommand()&& tempKey != 0x0) { 
			myKeyPad->setLastCommand(tempKey);
			myKeyPad->myConveyerBelt->getMassageFromKeyPad();
		}
		beLazy(100);
	}
}

void KeyPad::startKeyboard(){
	printf("Create Task KeyPad\n");
	int tReadKey = taskSpawn("tReadKeyTask", 120, 0, 0x1000, (FUNCPTR) fReadKey, (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
}

void KeyPad::setLastCommand(char command){
	lastCommand = command;
	return;
	
}
char KeyPad::getLastCommand(){
	return lastCommand;
}

