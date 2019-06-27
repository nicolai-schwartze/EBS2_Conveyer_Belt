#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

     
/* ANSI C headers */
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* VxWorks headers */
#include <vxWorks.h>
#include <taskLib.h>
#include <sysLib.h>
#include <semLib.h>
#include <rebootLib.h>
#include <logLib.h>

extern "C" {
#include "Subsystem.h"  
#include "rtwtypes.h"  
}

/* this sets the standard stack size for spawned tasks used by the model.
 * this can be changed by compiling with '-DSTACK_SIZE=nnn' where nnn is
 * the stack size desired.
 */
#ifndef STACK_SIZE
#define STACK_SIZE                     16384
#endif



class SystemManager;
class StateMachine;
class Motor;

class MotorController {
public:
	/* Default constructor */
	MotorController(SystemManager *CB, StateMachine * SM);
	/* Default destructor */
	~MotorController();
	SystemManager * myConveyerBelt;
	StateMachine * myStateMachine;
	Motor * myMotor;
	void init();
	void controllerStart();
	void outputController();
	
	void setRPM(float rpm,bool direction);
	float getRPM();
	
private:
	//float targetRPM;
};
#endif // MOTORCONTROLLER_H_

#ifndef MOTOR_H_
#define MOTOR_H_

class Motor{
public:
	Motor();	
	void startMotor();
	void offMotor();
	void drive();
	void setZero();
	void setSpeed(int v);
	int getSpeed();
		
private:	
	int speed;	
};
#endif // MOTOR_H_

void tController(MotorController *myMotorController);
static int_T tBaseRate(SEM_ID sem, SEM_ID startStopSem,MotorController *myMotorController);
