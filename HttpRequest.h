#ifndef HttpReuest_H_H
#define HttpReuest_H_H

#include <string>
using namespace std;

#define UNKNOWN 0
#define GET 1
#define POST 2
#define UPLOAD 10

#ifndef URL_LENGTH
#define URL_LENGTH 1024
#endif

struct HttpRequest
{
//public:
	char type;
	//char requestUrl[URL_LENGTH];
	string requestUrl;
	string boundary;
	unsigned long contenfLength;//上传时用
	string fileName;//上传时用
	bool isStart;
};
#endif