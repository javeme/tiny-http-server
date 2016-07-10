#ifndef HttpParser_H_H
#define HttpParser_H_H

#include "HttpRequest.h"
#include "File.h"
using namespace bluemeiIO;

class HttpParser
{
public:
	HttpParser();
	~HttpParser();
public:	
	void dealWithUpload(string& content,HttpRequest& request);
	void parse(string& src,HttpRequest& request);
private:
	string rootDir; 
};
#endif