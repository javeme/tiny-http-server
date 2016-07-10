#include "MyReceiver.h"
#include "stdio.h"

MyReceiver::MyReceiver()
{
	;
}
MyReceiver::~MyReceiver()
{
	;
}

void MyReceiver::onReceive(SOCKET& socket,SelectModelServer& server)
{	
	char buf[1024];
	int length=recv(socket,buf,sizeof(buf),0);
	if(length<0 ||  strcmp(buf,"exit")==0)
	{
		printf("error\n");
		server.close(socket);
		return;
	}
	buf[length]=0;
	printf("收到信息:%s\n",buf);
}