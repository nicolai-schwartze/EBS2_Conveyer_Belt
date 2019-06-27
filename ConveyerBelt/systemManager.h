
#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include "keyboard.h"
#include <taskLib.h>
#include <stdio.h>
#include <intLib.h>
#include <sysLib.h>
#include <stdLib.h>
#include <time.h>

#include "vxWorks.h"
#include "sockLib.h"
#include "inetLib.h"
#include "stdioLib.h"
#include "strLib.h"
#include "ioLib.h"
#include "fioLib.h"


#include "Data.h"
#include "MotorController.h"
#include "Network.h"
#include "IO_Conveyer.h"
#include "stateTable.h"


extern "C"{
#include "hwFunc.h"
#include "disp.h"
//#include "FHV.h"
#include "hw.h"
#include "ifLib.h"
#include <string.h>
}

struct request {
	int reply;                         /* TRUE = request reply from server */ 
	int msgLen;                        /* length of message text */
	char message[REQUEST_MSG_SIZE];    /* message buffer */ 
};


class SystemManager {
public:
	//SystemManager();
	SystemManager();
	~SystemManager();

	void initTask();
	void initTcpServer();
	void getMassageFromKeyPad();
	void getMessageFromCLIENT();
	void getMessageFromTEL();
	void getMessageFromSERVER();
	void runToCompletion();
private:

};

#endif // SYSTEMMANAGER_H_
