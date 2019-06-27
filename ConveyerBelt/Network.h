#include <string>

extern "C"{
#include "FHV.h"
}

//TCP and TELNET Constants
#define TCPSERVER_PORT_NUM      5555   /* TCP server's port number for bind() */
#define TCPCLIENT_PORT_NUM      5555   /* TCP client's port number for bind() */
#define TELSERVER_PORT_NUM		4444   /* TELNET server's port number for bind() */
#define SERVER_WORK_PRIORITY    100    /* priority of server's work task */
#define SERVER_STACK_SIZE       10000  /* stack size of server's work task */
#define SERVER_MAX_CONNECTIONS  4      /* max clients connected at a time */
#define REQUEST_MSG_SIZE        1024   /* max size of request message */
#define REPLY_MSG_SIZE          500    /* max size of reply message */


class StateMachine;
class SystemManager;

#ifndef TCPCLIENT_H_
#define TCPCLIENT_H_

class TcpClient {
public:
	TcpClient();
	TcpClient(SystemManager * CB);
	~TcpClient();
	SystemManager * myConveyerBelt;
	
	void initTcpClient(char ipAdressRight[]);
	void sendRequest();
	void setLastMessage(char message[]);
	void setSocket(int socket);
	char* getLastMessage();
	char* getMyIpAdress();
	
	
private:
	char lastMessage[20];
	int tcpSocket;
	char myIpAdress[20];
};
#endif // TCPCLIENT_H_


#ifndef TCPSERVER_H_
#define TCPSERVER_H_

class TcpServer {
public:
	TcpServer();
	TcpServer(SystemManager * CB);
	~TcpServer();
	SystemManager * myConveyerBelt;

	void initTcpServer();
	void sendRelease();	
	void sendWait();
	void sendReady();
	void setLastMessage(char message[]);
	char* getLastMessage();
	void setSocket(int socket);

private:
	char lastMessage[20];
	int tcpSocket;

};
#endif // TCPCLIENT_H_


#ifndef TELNETSERVER_H_
#define TELNETSERVER_H_
class TelnetServer {
public:
	TelnetServer();
	TelnetServer(SystemManager *CB);
	~TelnetServer();
	SystemManager * myConveyerBelt;
	void setLastMessage(char message);
	char getLastMessage();
	void initTelnetServer();
private:
	char lastMessage;
};
#endif // TELNETSERVER_H_


void fTelnetServer(TelnetServer * myTelnetServer);
void telnetServerWorkTask(int sFd, char * address, u_short port, TelnetServer *myTelnetServer);

void fTcpServer(TcpServer * myTcpServer);
void tcpServerWorkTask(int sFd, char * address, u_short port ,TcpServer *myTcpServer);

void fTcpClient(TcpClient * myTcpClient);
void tcpClientWorkTask(int sFd, char * address, u_short port ,TcpClient *myTcpClient);
