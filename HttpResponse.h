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
	unsigned long age;//��
	*/
private:
	//ͨ��ͷ��
	string commonHeader;
	//��Ӧͷ
	string replyHeader;
	//ʵ��ͷ
	string realityHeader;
public:
	//ͨ��ͷ
	void appendCacheControl(const char*str);
	void appendConnectionStatus(const char*str);
	void appendDate();
	void appendPragma(const char*str);
	//��Ӧͷ
	void appendHttpVersion(const char*str);
	void appendStatus(const char*str);
	void appendLocation(const char*str);
	void appendServerName(const char*str);
	void appendAge(unsigned long age);
	//ʵ��ͷ
	void appendLastModifiedTime(const char*str);
	void appendContentType(const char*str);
	void appendContentLocation(const char*str);
	void appendContentLength(unsigned int length);
	void appendContentRange(const char*str);
	void appendContentEncoding(const char*str);
	//д��ͷ
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