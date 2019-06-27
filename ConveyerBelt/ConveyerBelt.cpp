#include "ConveyerBelt.h"

ConveyerBelt :: ConveyerBelt() {
	


	
	

	
	
}


void ConveyerBelt :: keyboardChangeModeChain(){	
	printf("Sende Event from Keyboard TriggToChainIdle\n");
	myStateMachine->sendEvent("TriggChainIdle");	
} 
void ConveyerBelt :: keyboardChangeModeService(){
	printf("Sende Event from Keyboard  TriggToServiceIdle\n");
	myStateMachine->sendEvent("TriggServiceIdle");			
}
void ConveyerBelt :: keyboardChangeModeStop(){
	printf("Sende Event from Keyboard  TriggToStop\n");
	myStateMachine->sendEvent("TriggToStop");			
}
void ConveyerBelt :: keyboardChangeNextParameter(){
	printf("Sende Event from Keyboard  TriggModifyNext\n");
	myStateMachine->sendEvent("TriggModifyNext");	
}




void ConveyerBelt :: telnetChangeModeService(){
	printf("Sende Event from Telnet TriggToServiceIdle\n");
	myStateMachine->sendEvent("TriggServiceIdle");				
}
void ConveyerBelt :: telnetChangeModeChain(){
	printf("Sende Event from Telnet  TriggToChainIdle\n");
	myStateMachine->sendEvent("TriggChainIdle");		
}
void ConveyerBelt :: telnetChangeModeStop(){
	printf("Sende Event from Telnet TriggToStop\n");
	myStateMachine->sendEvent("TriggToStop");			
}
void ConveyerBelt :: telnetModifyParameter(){
	printf("Sende Event from Telnet  TriggModifyTR\n");
	myStateMachine->sendEvent("TriggModifyTR");			
}

void ConveyerBelt :: tcpServerReceivedReady(){
	printf("Sende Event from Tcp  TriggModifyTR\n");
	myStateMachine->sendEvent("TriggModifyTR");		
}
