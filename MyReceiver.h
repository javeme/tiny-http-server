#ifndef _MyReceiver_h_ 
#define _MyReceiver_h_ 

#include "ReceiveAble.h"

class MyReceiver:public ReceiveAble
{
public:
	MyReceiver();
	~MyReceiver();
public:
	void onReceive(SOCKET& socket,SelectModelServer& server);
};
#endif