#ifndef _ClientSocket_h_ 
#define _ClientSocket_h_ 

#include <map> 
using namespace std;

#include "SocketException.h"
#include "SocketTools.h"
#include "ReceiveAble.h"
#include "ClientMsg.h"

#include "HttpResponse.h"
#include "HttpParser.h"
#include "File.h"
using namespace bluemeiIO;

#define BUFFER_SIZE 1024

class SocketChanel:public ReceiveAble
{
public:
	SocketChanel();
	//SocketChanel(SOCKET hSocket);
	~SocketChanel();
private:
	bool m_bClose;
	HttpParser m_httpParser;
	map<SOCKET,ClientMsg> m_clientsMap;
public:
	void addPeerAddr(const SOCKET key,sockaddr_in addr);
	void removePeer(const SOCKET key);
	void getPeerAddr(const SOCKET key,sockaddr_in& addr);
	string getPeerHost(const SOCKET key);
	int getPeerPort(const SOCKET key);

	void removeClient(const SOCKET key);
	//void getClientMsg(const SOCKET key,ClientMsg& value);
	ClientMsg* getClientMsg(const SOCKET key);

	int readBytes(char buffer[],int maxLength,SOCKET& socket);
	int readInt(SOCKET& socket);
	//string& readLineByUtf(SOCKET& socket);
	string readLineByGbk(SOCKET& socket);
	//string readUtfString(int wantToReadSizeSOCKET& socket);
	string readUtfString(SOCKET& socket);

	//void close();
public:
	void onReceive(SOCKET& socket,SelectModelServer& server);
};

#endif