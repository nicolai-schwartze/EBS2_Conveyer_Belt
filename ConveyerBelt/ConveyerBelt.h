#ifndef CONVEYERBELT_H_
#define CONVEYERBELT_H_

#include "keyboard.h"
#include <taskLib.h>
#include <stdio.h>
#include <intLib.h>
#include <sysLib.h>
#include <stdLib.h>
#include <time.h>
#include <string.h>
#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "stdioLib.h"
#include "strLib.h"
#include "ioLib.h"
#include "fioLib.h"
#include "systemManager.h"
#include "stateMachine.h"


#include "Data.h"
#include "MotorController.h"
#include "Network.h"
#include "IO_Conveyer.h"

extern "C"{
#include "hwFunc.h"
#include "disp.h"
//#include "FHV.h"
#include "hw.h"
#include "ifLib.h"
}

class ConveyerBelt{
	public:	ConveyerBelt();
	~ConveyerBelt();
	
	MotorController * myMotorController;
	Data * myUserData;
	TelnetServer * myTelnetServer;
	TcpServer * myTcpServer;
	TcpClient * myTcpClient;
	Display * myDisplay;
	KeyPad * myKeyPad;
	SystemManager *mySystemManager;
	StateMachine  *myStateMachine;
	
	void keyboardChangeModeService();
	void keyboardChangeModeChain();
	void keyboardChangeModeStop();
	void keyboardStartProfil();
	void keyboardEndProfil();
	void keyboardModifyParameter();
	void keyboardChangeNextParameter();
	void keyboardChangeIncreaseParameter();
	void keyboardChangeDecreaseParameter();
	void keyboardChangeLeaveModification();
	
	void telnetChangeModeService();
	void telnetChangeModeChain();
	void telnetChangeModeStop();
	void telnetStartProfil();
	void telnetEndProfil();
	void telnetModifyParameter();
	void telnetChangeNextParameter();
	void telnetChangeIncreaseParameter();
	void telnetChangeDecreaseParameter();
	void telnetChangeLeaveModification();
		
	void tcpServerReceivedReady();
	void tcpClientReceivedWait();
	void tcpClientReceivedReady();
	void tcpClientReceivedRelease();
	
};

#endif // CONVEYERBELT_H_
