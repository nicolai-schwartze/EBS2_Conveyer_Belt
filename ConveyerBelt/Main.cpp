#include "systemManager.h"
#include "stateMachine.h"
extern "C"{
#include "FHV.h"
}
/*
DESCRIPTION
This is a file containing timer and interrupt for use with RTOS
*/

void setLocalIp();

void main (void)
{

	/* Add your code here: create tasks, semaphores, ... */
	// Set tick to 5 ms. This is the time resolution!
	sysClkRateSet(200);
	// Set local IP address according to MAC table
	setLocalIp();
	// Create instance of top class
	initHardware(0);
	
	SystemManager * myConveyerBelt = new SystemManager();

	myConveyerBelt->initTcpServer();
	myConveyerBelt->runToCompletion();

} /* main */



