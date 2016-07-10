#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
	appendDate();
}
HttpResponse::HttpResponse(const char* status,const char* version)
{
	appendHttpVersion(version);
	appendStatus(status);
	appendDate();
}
HttpResponse::~HttpResponse()
{
	;
}
//ͨ��ͷ
void HttpResponse::appendCacheControl(const char*str)
{
	this->commonHeader.append("Cache-Control:");
	this->commonHeader.append(str);
	this->commonHeader.append("\n");
}
void HttpResponse::appendConnectionStatus(const char*str)
{
	this->commonHeader.append("Connection:");
	this->commonHeader.append(str);
	this->commonHeader.append("\n");
}
void HttpResponse::appendDate()
{
	time_t rawtime; 
	time(&rawtime); 
	struct tm *timeInfo; 
	timeInfo = localtime(&rawtime); 
	char *str=asctime(timeInfo);
printf ( "\007The current time is: %s", str); //��ת��Ϊ��׼��ʽ
	this->commonHeader.append("Date:");
	this->commonHeader.append(str);
//this->commonHeader.append("\n");
}
void HttpResponse::appendPragma(const char*str)
{
	this->commonHeader.append("Pragma:");
	this->commonHeader.append(str);
	this->commonHeader.append("\n");
}
//��Ӧͷ
void HttpResponse::appendHttpVersion(const char*str)
{
	this->replyHeader="HTTP/";
	this->replyHeader.append(str);
}
void HttpResponse::appendStatus(const char*str)
{
	if(replyHeader.length()==0)
		this->appendHttpVersion(CURRENT_VERSION);
	this->replyHeader.append(" ");
	this->replyHeader.append(str);
	this->replyHeader.append("\n");
}
void HttpResponse::appendLocation(const char*str)
{
	;
}
void HttpResponse::appendServerName(const char*str)
{
	this->replyHeader.append(" Server:");
	this->replyHeader.append(str);
	this->replyHeader.append("\n");
}
void HttpResponse::appendAge(unsigned long age)
{
	this->realityHeader.append(" Age:");
	char buf[12];
	sprintf(buf,"%L",age);
	this->realityHeader.append(buf);
	this->realityHeader.append("\n");
}
//ʵ��ͷ
void HttpResponse::appendLastModifiedTime(const char*str)
{
	;
}
void HttpResponse::appendContentType(const char*str)
{
	this->realityHeader.append(" Content-Type:");
	this->realityHeader.append(str);
	this->realityHeader.append("\n");
}
void HttpResponse::appendContentLocation(const char*str)
{
	;
}
void HttpResponse::appendContentLength(unsigned int length)
{
	this->realityHeader.append(" Content-Length:");
	char buf[10];
	sprintf(buf,"%d",length);
	this->realityHeader.append(buf);
	this->realityHeader.append("\n");
}
void HttpResponse::appendContentRange(const char*str)
{
	;
}
void HttpResponse::appendContentEncoding(const char*str)
{
	;
}
//дhttp��Ӧͷ
void HttpResponse::writeHttpHeader(SOCKET& socket)
{
	this->writeString(replyHeader,socket);
	this->writeString(commonHeader,socket);
	this->writeString(realityHeader+"\n",socket); printf("%s%s%s",replyHeader.c_str(),commonHeader.c_str(),realityHeader.c_str());
}
//д�ֽ�,����ȡ���ֽ���
int HttpResponse::writeBytes(char buffer[],int length,SOCKET& socket)
{
	int size=0;
	size=::send(socket,buffer,length,0);
	if(size==SOCKET_ERROR)
	{
		throw SocketException(::WSAGetLastError());
	}
	return size;
}
//д����
int HttpResponse::writeInt(int value,SOCKET& socket)
{
	int sizeOfInt=sizeof(value);
	char *buf=new char[sizeOfInt];
	value=::htonl(value);
	SocketTools::intToBytes(buf,value);
	int size=writeBytes(buf,sizeOfInt,socket);
	delete[]buf;
	return size;
}
//д�ַ���,Դ����Ϊgbk����,����������utf-8������ʽ����
int HttpResponse::writeUtfString(string str,SOCKET& socket)
{	
	char*buf=(char*)str.c_str();//new char[length];
	int length=SocketTools::gbkToUtf8(str,buf);	
	int size=this->writeBytes((char*)str.c_str(),length,socket);	
	return size;
}
//д�ַ���,Դ����Ϊgbk����,����������unicode������ʽ����
int HttpResponse::writeUnicodeString(string str,SOCKET& socket)
{	
	wstring wstr;
	char*buf=(char*)str.c_str();//new char[length];
	int length=SocketTools::gbkToUnicode(wstr,buf);	//gb2312ToUnicode
	int size=this->writeBytes((char*)wstr.c_str(),length,socket);			
	return size;
}
////д�ַ���,����������Ĭ�ϱ��봫��
int HttpResponse::writeString(string str,SOCKET& socket)
{
	int length=str.length();
	int size=this->writeBytes((char*)str.c_str(),length,socket);
	return size;
}