#ifndef _ReceiveAble_h_ 
#define _ReceiveAble_h_ 

#include "SelectModelServer.h"

class ReceiveAble
{
public:
	virtual void onReceive(SOCKET& socket,SelectModelServer& server)=0;
public:
	virtual void addPeerAddr(const SOCKET key,sockaddr_in addr)=0;
	virtual void removePeer(const SOCKET key)=0;
	virtual void getPeerAddr(const SOCKET key,sockaddr_in& addr)=0;
	virtual string getPeerHost(const SOCKET key)=0;
	virtual int getPeerPort(const SOCKET key)=0;
};

#endif