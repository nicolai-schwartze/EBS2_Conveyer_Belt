#ifndef DISPLAY_H_
#define DISPLAY_H_

//#include "systemManager.h"
#include "FHV.h"

class SystemManager;

class Display {
public:
	Display();
	~Display();
	void writeParameter(int row, char value[]);
	
private:
	void initDisplay();
};
#endif // DISPLAY_H_

#ifndef KEYPAD_H_
#define KEYPAD_H_


class KeyPad {
public:
	KeyPad();
	KeyPad(SystemManager *CB);
	~KeyPad();
	SystemManager * myConveyerBelt;
	
	void startKeyboard();
	void setLastCommand(char command);
	char getLastCommand();
private:
	char lastCommand;

};
#endif // KeyPad_H_


void fReadKey (KeyPad* myKeyPad);
