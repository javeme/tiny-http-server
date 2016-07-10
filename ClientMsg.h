#ifndef ClientMsg_H_H
#define ClientMsg_H_H

#include <winsock2.h>
#include "HttpRequest.h"

struct ClientMsg
{
	sockaddr_in clientAddr;
	HttpRequest request;
	unsigned long receivedLength;
};
#endif