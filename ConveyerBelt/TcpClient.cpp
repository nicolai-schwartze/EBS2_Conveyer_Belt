#include "systemManager.h" 
#include "stateMachine.h"
TcpClient :: TcpClient(SystemManager * CB){ 
	myConveyerBelt = CB;
	return;
}	

void TcpClient::initTcpClient(char ipAdressRight[]){
	printf("Create Task TcpClient\n");
	//static char ipAdress[] = "91.0.0.105";
	strncpy(myIpAdress,ipAdressRight,12);
	int tcpClientID = taskSpawn("tTcpClient", 118, 0, 0x1000, (FUNCPTR) fTcpClient, (int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);	
}


void fTcpClient(TcpClient *myTcpClient){
		
	struct sockaddr_in clientAddr; 
    int sock = 0;
    struct sockaddr_in serverAddr;    
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return; 
    } 
    
    memset(&serverAddr, '0', sizeof(serverAddr)); 
   
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(TCPCLIENT_PORT_NUM); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
 
    if(inet_pton(AF_INET, myTcpClient->getMyIpAdress(), &serverAddr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) 
    { 
        printf("\nConnection Failed \n"); 
        return; 
    } 
    myTcpClient->setSocket(sock);

	/* accept new connect requests and spawn tasks to process them */
	while(1) {
		char actualMessage[100];
        char messageFromServer[10] = {0};  
    	int valread = read( sock , messageFromServer, 10);
	    
    	sprintf(actualMessage, "Got Message form TCP Server  %s\n", messageFromServer); 
    	printf(actualMessage);
		if( strncmp(messageFromServer,myTcpClient->getLastMessage(), 7) != 0 && strncmp(messageFromServer,"",1) != 0){
			myTcpClient->setLastMessage(messageFromServer);
			//strncpy(myTcpClient->lastMessage,messageFromServer,7);
			myTcpClient->myConveyerBelt->getMessageFromSERVER();
		}
		beLazy(100);
	}
}

void TcpClient :: sendRequest(){
	printf("TCP Client sends Request\n");
	static char readyMsg[] = "Request\n";
	write(tcpSocket ,readyMsg , strlen(readyMsg)); 
} 

void TcpClient :: setLastMessage(char message[]){
	strncpy(lastMessage,message,7);
	return;
}

void TcpClient :: setSocket(int socket){
	tcpSocket = socket;
	return;
}
char* TcpClient :: getLastMessage(){
	return lastMessage;
}
char* TcpClient :: getMyIpAdress(){
	return myIpAdress;
}
