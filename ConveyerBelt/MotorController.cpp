#include "systemManager.h"


/* this sets the standard stack size for spawned tasks used by the model.
 * this can be changed by compiling with '-DSTACK_SIZE=nnn' where nnn is
 * the stack size desired.
 */
#ifndef STACK_SIZE
#define STACK_SIZE                     16384
#endif

	float targetRPM;
	float actualRPM;
	float newRPM;
MotorController::MotorController(SystemManager * CB, StateMachine * SM){ 
	myConveyerBelt = CB;
	myMotor = new Motor();
return;
}	

void MotorController::init(){
	printf("Create Task MotorController\n");
	int motorControllerID = taskSpawn("tMotorController", 105, 0, 0x1000, (FUNCPTR) tController, (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
	return;
}

static int_T tBaseRate(SEM_ID sem, SEM_ID startStopSem, MotorController *myMotorController)
{
  int_T i;
  char buffer[10];
  while (1) {
    if (rtmGetErrorStatus(Subsystem_M) != (NULL)) {
      fprintf(stderr,"Error status: %s \n", rtmGetErrorStatus(Subsystem_M));
      semGive(startStopSem);
      return(1);
    }

    if (semTake(sem,NO_WAIT) != ERROR) {
      logMsg("Rate for SingleRate task too fast.\n",0,0,0,0,0,0);
    } else {
      semTake(sem, WAIT_FOREVER);
    }
    /* Set model inputs here */

    actualRPM = (getEncoderPulsesZeroCorrected())*60*getRotationDirection(0); 
    float diffRPM = (-1)*(targetRPM - actualRPM); 
    
    Subsystem_U.In2 = diffRPM;
    
    sprintf(buffer,"%.0f\n",actualRPM);
    printf(buffer);
    
    
    
    /* Step the model */
    Subsystem_step();

    /* Get model outputs here */
    
    
    //float output = 4095.0 / (2.0 * 12.0) *Subsystem_Y.u_ref;
    float output = Subsystem_Y.u_ref *220 +1600;
    
    //Test
    
    //sprintf(buffer,"%.0f\n",output);
    //printf(buffer);
      
    if(output > 4095){
    	output = 4095;    	
    } else if(output < 0){
    	output =0;  	
    }
        
    myMotorController->myMotor->setSpeed(output);
    myMotorController->myMotor->drive();
  }
  return(1);
}

void tController(MotorController *myMotorController) {
	  const char *status;
	  int_T priority = 99;
	  real_T requestedSR, actualSR;
	  int_T VxWorksTIDs[1];
	  SEM_ID rtTaskSemaphoreList[1];
	  SEM_ID rtClockSem, startStopSem;
	  printf("Warning: The simulation will run forever. "
	         "To change this behavior select the 'MAT-file logging' option.\n");

	  if (priority <= 0 || priority > 255-(1)+1) {
	    priority = 30;
	  }

	  rtClockSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	  startStopSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);

	  /* Initialize model */
	  Subsystem_initialize();
	  requestedSR = 1.0 / 0.015625;
	  sysAuxClkDisable();
	  sysAuxClkRateSet((int_T)(requestedSR + 0.5));
	  actualSR = (real_T) sysAuxClkRateGet();
	  printf("Actual sample rate in Hertz: %f\n",actualSR);
	  VxWorksTIDs[0] = taskSpawn("tBaseRate",priority, VX_FP_TASK, STACK_SIZE, (FUNCPTR)tBaseRate, (int_T) rtClockSem,(int_T) startStopSem, (int_T) rtTaskSemaphoreList, (int)myMotorController, 0, 0, 0, 0, 0, 0);
	  if (sysAuxClkConnect((FUNCPTR) semGive, (int_T) rtClockSem) == OK) {
	    rebootHookAdd((FUNCPTR) sysAuxClkDisable);
	    printf("\nSimulation starting\n");
	    sysAuxClkEnable();
	  }

	  semTake(startStopSem, WAIT_FOREVER);
	  sysAuxClkDisable();
	  taskDelete(VxWorksTIDs[0]);
	  semDelete(rtClockSem);
	  semDelete(startStopSem);

	  /* Disable rt_OneStep() here */

	  /* Terminate model */
	  Subsystem_terminate();
	  return;
}

void MotorController::setRPM(float rpm, bool direction){
	if(direction){
		targetRPM = (-1)*rpm;}
	else {
		targetRPM = rpm;}
			
	//char buffer[6];
	//sprintf(buffer,"%.0f\n",targetRPM);
	//printf(buffer);
	return;
}
float MotorController::getRPM(){
	return targetRPM;
}




