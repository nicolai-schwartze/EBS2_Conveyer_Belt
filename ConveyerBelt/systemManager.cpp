
#include "systemManager.h"
#include "stateMachine.h"
#include "keyboard.h"
#include "myFunctions.h"

int rising, falling;
StateMachine * myStateMachine;
Display * myDisplay;
KeyPad * myKeyPad;
TelnetServer * myTelnetServer;
TcpServer * myTcpServer;
TcpClient * myTcpClient;
MotorController * myMotorController;

bool requestReady;
bool inIdle;
bool serviceIdle;
int maxV;
int timePP;
int timeTR;
int timeTT;
int timeTF;
float step ;
float targetValue;
char rightIpAdress[10];
bool direction;

SystemManager :: SystemManager() {
	// Initialize table for all diagrams, event time in ms (POSIX)
	// The maximum size of the table is defined in stateTable.h:
	// MAXDIA = 9, MAXLINES = 66
	// Should these be exceeded, please correct!

	//State Mode Stop 
	tab[0][0] = new TableEntry ("Stop","ChainIdle","TriggChainIdle",0,StopToChainIdle,myConditionTrue);
	tab[0][1] = new TableEntry ("Stop","ServiceIdle","TriggServiceIdle",0,StopToServiceIdle,myConditionTrue);
	
	//Service Mode
	tab[0][2] = new TableEntry ("ServiceIdle","Stop","TriggToStop",0,ServiceToStop,myConditionTrue);
	tab[0][3] = new TableEntry ("ServiceIdle","ServiceRampRising","ServiceStart",0,ServiceIdleToRampRising,myConditionTrue);
		
	tab[0][4] = new TableEntry ("ServiceRampRising","Stop","TriggToStop",0,ServiceToStop,myConditionTrue);
	tab[0][5] = new TableEntry ("ServiceRampRising","ServiceRampRising","Timer0",20,ServiceRampRising,myConditionTrue);
	tab[0][6] = new TableEntry ("ServiceRampRising","ServiceRampConst","ServiceToConst",0,ServiceRampRisingToConst,myConditionTrue);
	
	tab[0][7] = new TableEntry ("ServiceRampConst","Stop","TriggToStop",0,ServiceToStop,myConditionTrue);
	tab[0][8] = new TableEntry ("ServiceRampConst","ServiceRampFall","Timer0",6000,ServiceRampConstToRampFall,myConditionTrue);
	
	tab[0][9] = new TableEntry ("ServiceRampFall","Stop","TriggToStop",0,ServiceToStop,myConditionTrue);
	tab[0][10] = new TableEntry ("ServiceRampFall","ServiceRampFall","Timer0",20,ServiceRampFall,myConditionTrue);
	tab[0][11] = new TableEntry ("ServiceRampFall","ServiceIdle","ServiceFallToIdle",0,ServiceRampFallingToIdle,myConditionTrue);
	
	//Chain Mode
	tab[0][12] = new TableEntry ("ChainIdle","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][13] = new TableEntry ("ChainIdle","ChainSlowRecive","TriggSlowRecive",0,ChainIdleToChainSlowRecive,myConditionTrue);
	
	tab[0][14] = new TableEntry ("ChainSlowRecive","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][15] = new TableEntry ("ChainSlowRecive","ChainRampRising","Timer0",1000,ChainSlowReciveToChainRampRising,myConditionTrue);
	
	tab[0][16] = new TableEntry ("ChainRampRising","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][17] = new TableEntry ("ChainRampRising","ChainRampRising","Timer0",20,ChainRampRising,myConditionTrue);
	tab[0][18] = new TableEntry ("ChainRampRising","ChainRampConst","TriggToConst",0,ChainRampRisingToChainRampConst,myConditionTrue);
	
	tab[0][19] = new TableEntry ("ChainRampConst","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][20] = new TableEntry ("ChainRampConst","ChainRampFall","Timer0",6000,ChainRampConstToChainRampFall,myConditionTrue);
	
	tab[0][21] = new TableEntry ("ChainRampFall","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][22] = new TableEntry ("ChainRampFall","ChainRampFall","Timer0",20,ChainRampFall,myConditionTrue);
	tab[0][23] = new TableEntry ("ChainRampFall","Waiting","TriggToWait",0,ChainRampFallToWaiting,myConditionTrue);
	
	tab[0][24] = new TableEntry ("Waiting","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][25] = new TableEntry ("Waiting","Waiting","TriggWaiting",0,Waiting,myConditionTrue);
	tab[0][26] = new TableEntry ("Waiting","ChainSlowSend","TriggSlowSend",0,WaitingToChainSlowSend,myConditionTrue);
	
	tab[0][27] = new TableEntry ("ChainSlowSend","Stop","TriggToStop",0,ChainToStop,myConditionTrue);
	tab[0][28] = new TableEntry ("ChainSlowSend","ChainIdle","TriggChainIdle",0,ChainSlowSendToChainIdle,myConditionTrue); 
	

	// Initialize timer names for all diagrams
	// Timer names are always Timer followed by the diagram number
	timerNames[0] = "Timer0";
	//timerNames[1] = "TimerTR";
	//timerNames[2] = "TimerTT";
	//timerNames[2] = "TimerTF";
	

	// Initialize line numbers for all diagrams
	lines[0] = 29;
	//lines[1] = 4;
	//lines[2] = 1;

	// Initialize first state for all diagrams
	actualState[0] = "Stop";
	//actualState[1] = "StateC";
	//actualState[2] = "StateK";
	
	// Set the actual number of diagrams
	diagrams = 1;
	
	// Create instance of state machine
	myStateMachine = new StateMachine;

	// Create instance of my Keypad
	myKeyPad = new KeyPad(this);
	
	// Create instance of my Display
	myDisplay = new Display();
	
	// Create instance of my TelnetServer
	myTelnetServer = new TelnetServer(this);
	
	// Create instance of my TcpServer
	myTcpServer = new TcpServer(this);
	
	// Create instance of my TcpServer
	myTcpClient = new TcpClient(this);
	
	// Create instance of my MotorController	
	myMotorController = new MotorController(this, myStateMachine );
	
	
	// Start timer for each diagram which needs one in the first state!
	// In my case these are diagram 0 and 2
	//myStateMachine->diaTimerTable[0]->startTimer(tab[0][0]->eventTime);
	//myStateMachine->diaTimerTable[2]->startTimer(tab[2][0]->eventTime);

	// Initial actions can be done here, if needed!
	direction = false;
	rising = 0;
	falling = 0;
	maxV = 1800;
	return;
}

SystemManager :: ~SystemManager() {
	return;
}
void SystemManager :: initTcpServer(){
	myTcpServer->initTcpServer();
	myKeyPad->startKeyboard();
	myMotorController->init();
}
void SystemManager :: initTask(){
	myTelnetServer->initTelnetServer();
	myTcpClient->initTcpClient(rightIpAdress);
}

void SystemManager::runToCompletion(){
	myStateMachine->runToCompletion();
}

void SystemManager:: getMassageFromKeyPad(){
	printf("Got Message form KeyPad\n");
	char message = myKeyPad->getLastCommand();
	
	switch (message){			
	case 'A':		
		printf("Key A was pressed\n\r");
		myStateMachine->sendEvent("TriggServiceIdle");
		break;
	case 'B':		
		printf("Key B was pressed\n\r");
		myStateMachine->sendEvent("TriggChainIdle");
		break;	
	case 'F':		
		printf("Key F was pressed\n\r");
		myStateMachine->sendEvent("TriggToStop");
		break;
	case '4':	
		if(serviceIdle){
		char buffer[6];
		printf("Key 4 was pressed\n\r");
		printf("Rising Vmax\n\r");
		if(maxV < 2200){
		maxV= maxV + 100;
		
		sprintf(buffer,"%d",maxV);
		myDisplay->writeParameter(1,buffer);
		}
		myKeyPad->setLastCommand(0);
		}
		break;	
	case '5':	
		if(serviceIdle){
		printf("Key 5 was pressed\n\r");
		printf("Change direction\n\r");
		if(direction) {
			direction = false; 
			myDisplay->writeParameter(4,"Right");}
		else{ 
			direction = true;
			myDisplay->writeParameter(4,"Left");}
		}
		myKeyPad->setLastCommand(0);
		break;
	case '7':	
		if(serviceIdle){
		char buffer[6];
		printf("Key 7 was pressed\n\r");
		printf("Decrease Vmax\n\r");
		if(maxV > 100){
		maxV= maxV - 100;
		
		sprintf(buffer,"%d",maxV);
		myDisplay->writeParameter(1,buffer);
		}
		myKeyPad->setLastCommand(0);
		}
		break;		
	case 'C':		
		printf("Key C was pressed\n\r");
		myStateMachine->sendEvent("ServiceStart");
		break;	
	default : break;
	}
}
	

void SystemManager :: getMessageFromCLIENT(){
	printf("Compare Message from TCP Client\n");	
	char *message = myTcpServer->getLastMessage();
	
	if(strncmp(message,"REQUEST",7) == 0){
		
		printf("Got Request from TCP Client\n");
		requestReady = true;
		
		if(inIdle){
			myTcpServer->sendReady();
			requestReady = false;
			myStateMachine->sendEvent("TriggSlowRecive");
		} 
		else{
			myTcpServer->sendWait();
		}
		return;
	}	
	else if(strncmp(message,"Right",5) == 0){
		printf("Got IpAdress from TCP Server\n\r");
		char delimiter[] = {' ', '\n'};
		char *ptr; 
		ptr= strtok(message,delimiter);
		ptr= strtok(NULL,delimiter);
		printf("IP Adresse vom rechten Nachbar %s\n", ptr);
		strncpy(rightIpAdress,ptr,12);
		initTask();
		myTcpServer->setLastMessage("");
	}	
}

void SystemManager :: getMessageFromTEL(){
	
	printf("Got Message form Telnet\n");
	char message = myTelnetServer->getLastMessage();
	
	switch (message){			
	case 'A':		
		printf("Key A was pressed\n\r");
		myStateMachine->sendEvent("TriggServiceIdle");
		break;
	case 'B':		
		printf("Key B was pressed\n\r");
		myStateMachine->sendEvent("TriggChainIdle");
		break;	
	case 'F':		
		printf("Key F was pressed\n\r");
		myStateMachine->sendEvent("TriggToStop");
		break;
	case '4':	
		if(serviceIdle){
		char buffer[6];
		printf("Key 4 was pressed\n\r");
		printf("Rising Vmax\n\r");
		if(maxV < 2200){
		maxV= maxV + 100;
		
		sprintf(buffer,"%d",maxV);
		myDisplay->writeParameter(1,buffer);
		}
		myTelnetServer->setLastMessage(0);
		}
		break;	
	case '5':	
		if(serviceIdle){
		printf("Key 5 was pressed\n\r");
		printf("Change direction\n\r");
		if(direction) {
			direction = false; 
			myDisplay->writeParameter(4,"Right");}
		else{ 
			direction = true;
			myDisplay->writeParameter(4,"Left");}
		}
		myTelnetServer->setLastMessage(0);
		break;
	case '7':	
		if(serviceIdle){
		char buffer[6];
		printf("Key 7 was pressed\n\r");
		printf("Decrease Vmax\n\r");
		if(maxV > 100){
		maxV= maxV - 100;
		
		sprintf(buffer,"%d",maxV);
		myDisplay->writeParameter(1,buffer);
		}
		myKeyPad->setLastCommand(0);
		}
		break;		
	case 'C':		
		printf("Key C was pressed\n\r");
		myStateMachine->sendEvent("ServiceStart");
		break;	
	default : break;
	}
}

void SystemManager::getMessageFromSERVER(){
	printf("Compare Message from TCP Server \n");
	char *message = myTcpClient->getLastMessage();
	
	if(strncmp(message,"READY",5) == 0){
		printf("Got Ready from TCP Server\n\r");
		myStateMachine->sendEvent("TriggSlowSend");
	} 
	else if(strncmp(message,"WAIT",4) == 0){
		printf("Got Wait from TCP Server\n\r");
		myStateMachine->sendEvent("TriggWaiting");		
	}
	else if(strncmp(message,"RELEASE",7) == 0){
		printf("Got Release from TCP Server\n\r");
		myTcpServer->setLastMessage("");
		myStateMachine->sendEvent("TriggChainIdle");	
	}
};
//Mode
void StopToChainIdle(){	
	//printf("Stop -> ChainIdle\n\r");
	inIdle = true;
	direction = false;
	maxV = 1800;
	timePP = 1000;
	timeTR = 1000;
	timeTT = 8000;
	timeTF = 1000;
	
	myDisplay->writeParameter(2,"Chain");
	myDisplay->writeParameter(3,"Idle");
	myDisplay->writeParameter(4,"Right");
	
	char buffer[6];
	sprintf(buffer,"%.0f",maxV);
	myDisplay->writeParameter(1,buffer);
	
	sprintf(buffer,"%d",timePP);
	myDisplay->writeParameter(5,buffer);
	
	sprintf(buffer,"%d",timeTR);
	myDisplay->writeParameter(6,buffer);
	
	sprintf(buffer,"%d",timeTT);
	myDisplay->writeParameter(7,buffer);
	
	sprintf(buffer,"%d",timeTF);
	myDisplay->writeParameter(8,buffer);
	
	return;
}
void StopToServiceIdle(){
	printf("Stop -> ServiceIdle\n\r");
	serviceIdle = true;
	timePP = 1000;
	timeTR = 1000;
	timeTT = 8000;
	timeTF = 1000;
	
	myDisplay->writeParameter(2,"Service");
	myDisplay->writeParameter(3,"Idle");
	myDisplay->writeParameter(4,"Right");
	
	char buffer[6];
	sprintf(buffer,"%.0f",maxV);
	myDisplay->writeParameter(1,buffer);
	
	sprintf(buffer,"%d",timePP);
	myDisplay->writeParameter(5,buffer);
		
	sprintf(buffer,"%d",timeTR);
	myDisplay->writeParameter(6,buffer);
	
	sprintf(buffer,"%d",timeTT);
	myDisplay->writeParameter(7,buffer);
	
	sprintf(buffer,"%d",timeTF);
	myDisplay->writeParameter(8,buffer);
	return;
}


//Service

void ServiceToStop(){
	printf(" ServiceIdle -> Stop\n\r"); 
	serviceIdle = false;
	myDisplay->writeParameter(2,"Stop");
	myDisplay->writeParameter(3,"");
	myMotorController->setRPM(0,direction);
	myMotorController->myMotor->offMotor();
	return;
}
void ServiceIdleToRampRising(){
	printf("ServiceIdle -> ServiceRampRising\n\r");
	serviceIdle = false;
	myDisplay->writeParameter(3,"Rising");
	targetValue = 0;
	rising = 0;
	myMotorController->myMotor->startMotor();
	myMotorController->setRPM(targetValue,direction);
	return;	
}
void ServiceRampRising(){
	//sprintf("ServiceRising\n\r");
	step = (maxV-200)/timeTR;
	rising = rising+20;
	targetValue = targetValue + 20*step;
	myMotorController->setRPM(targetValue,direction);
	if(rising >= timeTR){
		myStateMachine->sendEvent("ServiceToConst");		
	}	
	/*
	char buffer[6];
	sprintf(buffer,"%.0f",targetValue);
	myDisplay->writeParameter(9,buffer);*/
	return;
}

void ServiceRampRisingToConst(){
	printf("ServiceRampRising -> ServiceRampConst\n\r"); 
	myDisplay->writeParameter(3,"Constant");
	targetValue = maxV;
	myMotorController->setRPM(targetValue,direction);
	
	return;
}
void ServiceRampConstToRampFall(){
	printf("ChainRampConst -> ChainRampFall\n\r");
	myDisplay->writeParameter(3,"Falling");
	falling = 0;
	return;
}

void ServiceRampFall(){	
	step = maxV/timeTF;
	falling = falling+20;
	targetValue = targetValue - 20*step;
	myMotorController->setRPM(targetValue,direction);
	if(falling >= timeTR){
		myStateMachine->sendEvent("ServiceFallToIdle");
		myMotorController->setRPM(0,direction);
		myMotorController->myMotor->offMotor();
	}	
	/*
	char buffer[6];
	sprintf(buffer,"%.0f",targetValue);
	myDisplay->writeParameter(9,buffer);*/	
	return;	
}

void ServiceRampFallingToIdle(){
	printf(" RampFalling -> ServiceIdle\n\r");
	myDisplay->writeParameter(3,"Idle");
	myKeyPad->setLastCommand(0);
	myTelnetServer->setLastMessage(0);
	serviceIdle = true;
	return;
}



//Chain
void ChainToStop(){
	printf("ChainIdle -> Stop\n\r");
	inIdle = false;
	myDisplay->writeParameter(2,"Stop");
	myDisplay->writeParameter(3,"");
	targetValue = 0;
	myMotorController->setRPM(targetValue,direction);
	return;
}

void ChainIdleToChainSlowRecive(){
	printf("ChainIdle -> ChainSlowRecive\n\r"); 
	myDisplay->writeParameter(3,"Slow");
	inIdle = false;	
	myMotorController->myMotor->startMotor();
	targetValue = 200;
	myMotorController->setRPM(targetValue,direction);
	return;
}
void ChainSlowReciveToChainRampRising(){
	printf("ChainSlowRecive -> ChainRampRising\n\r");
	myDisplay->writeParameter(3,"Rising");
	myTcpServer->sendRelease();
	rising = 0;
	return;
}
void ChainRampRising(){
	step = (maxV-200)/timeTR;
	rising = rising+20;
	targetValue = targetValue + 20*step;
	myMotorController->setRPM(targetValue,direction);
	if(rising >= timeTR){
		myStateMachine->sendEvent("TriggToConst");		
	}	
	return;
}

void ChainRampRisingToChainRampConst(){
	printf("ChainRampRising -> ChainRampConst\n\r"); 
	myDisplay->writeParameter(3,"Constant");
	targetValue = maxV;
	myMotorController->setRPM(targetValue,direction);
	return;
}
void ChainRampConstToChainRampFall(){
	printf("ChainRampConst -> ChainRampFall\n\r");
	myDisplay->writeParameter(3,"Falling");
	falling = 0;
	return;
}
void ChainRampFall(){	
	step = maxV/timeTF;
	falling = falling+20;
	targetValue = targetValue - 20*step;
	myMotorController->setRPM(targetValue,direction);
	if(falling >= timeTR){
		myStateMachine->sendEvent("TriggToWait");
		myMotorController->setRPM(0,direction);
	}	
	return;	
}

void ChainRampFallToWaiting(){
	printf("ChainRampFall -> Waiting\n\r");
	myDisplay->writeParameter(3,"Waiting");
	myTcpClient->sendRequest();
	return;
}
void WaitingToChainSlowSend(){
	printf("Waiting -> ChainSlowSend\n\r");
	myDisplay->writeParameter(3,"Slow");
	targetValue = 200;
	myMotorController->setRPM(targetValue,direction);
	return;
}
void ChainSlowSendToChainIdle(){
	printf("ChainSlowSend -> ChainIdle\n\r");
	inIdle = true;
	myDisplay->writeParameter(3,"Idle");
	
	if(requestReady){
		myTcpServer->sendReady();
		requestReady = false;
		myStateMachine->sendEvent("TriggSlowRecive");
	} else{
		targetValue = 0;
		myMotorController->setRPM(targetValue,direction);	
		myMotorController->myMotor->offMotor();
	}
	return;
}
void Waiting(){
	printf("Waiting for Ready\n\r");
	return;
}


bool myConditionTrue(){
	return TRUE;
}
