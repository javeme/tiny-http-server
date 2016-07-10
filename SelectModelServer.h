
#ifndef SelectModelServer_H
#define SelectModelServer_H

#define FD_SETSIZE 100

#include <stdio.h>
#include <winsock2.h>

#include "SocketTools.h"
#include "SocketException.h"

class ReceiveAble;

class SelectModelServer
{
public:
	SelectModelServer(ReceiveAble& receiver);
	~SelectModelServer();
private:
	int m_nCurrentConCount;
	SOCKET m_serverSocket;  
	SOCKET m_socketArray[FD_SETSIZE];
	//bool m_bUseableArray[FD_SETSIZE];
	//sockaddr_in peerAddrs[MAXIMUM_WAIT_OBJECTS];

	ReceiveAble* m_pReceiver;
private:
	void onAccept();
	void onReceive(SOCKET&  clientSocket);
	void onClose(SOCKET&  clientSocket);
public:
	void init(unsigned short port);
	void startServer();
	void closeServer();
	void close(SOCKET&  socket);
}; 
#endif
