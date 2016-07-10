#ifndef HttpResponse_H_H
#define HttpResponse_H_H

#include<winsock2.h>
#include <string>
using namespace std;
#include "time.h"

#include "SocketException.h"
#include "SocketTools.h"

#define CURRENT_VERSION "1.1"

class HttpResponse
{
public:
	HttpResponse();
	HttpResponse(const char* status,const char* version=CURRENT_VERSION);
	~HttpResponse();
	/*
	string httpType;//HTTP1.1
	unsigned short status;//200 OK
	unsigned long age;//秒
	*/
private:
	//通用头域
	string commonHeader;
	//响应头
	string replyHeader;
	//实体头
	string realityHeader;
public:
	//通用头
	void appendCacheControl(const char*str);
	void appendConnectionStatus(const char*str);
	void appendDate();
	void appendPragma(const char*str);
	//响应头
	void appendHttpVersion(const char*str);
	void appendStatus(const char*str);
	void appendLocation(const char*str);
	void appendServerName(const char*str);
	void appendAge(unsigned long age);
	//实体头
	void appendLastModifiedTime(const char*str);
	void appendContentType(const char*str);
	void appendContentLocation(const char*str);
	void appendContentLength(unsigned int length);
	void appendContentRange(const char*str);
	void appendContentEncoding(const char*str);
	//写入头
	void writeHttpHeader(SOCKET& socket);
public:
	//int writeBytes(byte*& buffer,int length);
	int writeBytes(char buffer[],int length,SOCKET& socket);
	int writeInt(int value,SOCKET& socket);
	int writeString(string str,SOCKET& socket);
	int writeUtfString(string str,SOCKET& socket);
	int writeUnicodeString(string str,SOCKET& socket);
};
#endif