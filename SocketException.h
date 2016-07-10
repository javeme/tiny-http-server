
#ifndef _SocketException_h_
#define _SocketException_h_

#include <string>  
using namespace std;

#include "Exception.h"
using namespace bluemeiException;

class SocketException:public Exception
{
public:
	SocketException(int nError);
	SocketException(string strError);
	SocketException(int nError,string strError);
	~SocketException();
public:
	int getError();
	void printErrorMsg();
protected:
	int m_nError;
};
#endif