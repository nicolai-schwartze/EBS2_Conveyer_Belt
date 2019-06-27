#include "systemManager.h"
#include "stateMachine.h"

TelnetServer::TelnetServer(SystemManager *CB){ 
	myConveyerBelt = CB;
	return;
}	

void TelnetServer::initTelnetServer(){
	printf("Create Task TelNetServer\n");
	int telnetServerID = taskSpawn("tTelnetServer", 119, 0, 0x1000, (FUNCPTR) fTelnetServer, (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
}

void fTelnetServer(TelnetServer *myTelnetServer){
	//TCP/IP Lab
	struct sockaddr_in  serverAddr;    /* server's socket address */
	struct sockaddr_in  clientAddr;    /* client's socket address */
	int                 sockAddrSize;  /* size of socket address structure */
	int                 sFd;           /* socket file descriptor */
	int                 newFd;         /* socket descriptor from accept */
	//int                 ix = 0;        /* counter for work task names */
	char                workName[16];  /* name of work task */
	
	/* set up the local address */
	sockAddrSize = sizeof (struct sockaddr_in);
	bzero ((char *) &serverAddr, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_len = (u_char) sockAddrSize;
	serverAddr.sin_port = htons (TELSERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
		
	/* create a TCP-based socket */
	if ((sFd = socket (AF_INET, SOCK_STREAM, 0)) == ERROR) {
		perror ("socket");
		printf("Fehler TCP Server");	
	}
	
	/* bind socket to local address */
	if (bind (sFd, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR) {
		perror ("bind");
		printf("Fehler TCP Server");
		close (sFd);
	}
	
	/* create queue for client connection requests */
	if (listen (sFd, SERVER_MAX_CONNECTIONS) == ERROR) {
		perror ("listen");
		printf("Fehler TCP Server");
		close (sFd);
	}
			
	/* accept new connect requests and spawn tasks to process them */
	while(1) {
		socklen_t len = sizeof(clientAddr);
		if ((newFd = accept (sFd, (struct sockaddr *) &clientAddr, &len)) == ERROR) {
			printf("In if");
			perror ("accept");
			printf("Fehler TCP Server");	
			close (sFd);
		}

		if (taskSpawn("tReadTelnetTask", SERVER_WORK_PRIORITY, 0, SERVER_STACK_SIZE, (FUNCPTR) telnetServerWorkTask, newFd, (int) inet_ntoa (clientAddr.sin_addr), ntohs (clientAddr.sin_port),(int) myTelnetServer, 0, 0, 0, 0, 0, 0) == ERROR) {
			/* if taskSpawn fails, close fd and return to top of loop */
			printf("Fehler create telnettask");
			perror ("taskSpawn");
			close (newFd);
		}
	}
}
void telnetServerWorkTask(int sFd, /* server's socket fd */ char * address, /* client's socket address */ u_short port /* client's socket port */,TelnetServer *myTelnetServer){
	
struct request clientRequest;  /* request/message from client */
int nRead;          /* number of bytes read */
static char replyMsg[] = "Server received your message\n\r";
/* read client request, display message */

char clientAnfrage[3];
//write(sFd, "Hello Client\n", sizeof ("Hello Client\n"));
	while ((nRead = fioRdString (sFd, (char *) &clientAnfrage, sizeof (clientAnfrage))) > 0) {
			//printf ("MESSAGE FROM CLIENT (Internet Address %s, port %d):\n%s\n\r", address, port, clientRequest.message);
			//free (address); -> funktioniert nicht                 /* free malloc from inet_ntoa() */
			if (clientRequest.reply) {
				if (write (sFd, replyMsg, strlen(replyMsg)) == ERROR) {
					perror ("write");
				}
			}
			printf("Got Message form 'Telnet\n");
			
			if( clientAnfrage[0] != myTelnetServer->getLastMessage() &&  strncmp(clientAnfrage,"",1) != 0){
			myTelnetServer->setLastMessage(clientAnfrage[0]);		
			myTelnetServer->myConveyerBelt->getMessageFromTEL();
			}
			beLazy(100);
	}		
		if (nRead == ERROR){					/* error from read() */
			perror ("read");
		}	
close (sFd);                        /* close server socket connection */	
}

void TelnetServer::setLastMessage(char message){
	lastMessage = message;
	return;
	
}
char TelnetServer::getLastMessage(){
	return lastMessage;
}
