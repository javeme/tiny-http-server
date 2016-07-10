#include "SocketException.h"
#include "SocketTools.h"

SocketException::SocketException(int nError)
{
	this->SocketException::SocketException(nError,"");
}
SocketException::SocketException(string strError)
{
	this->SocketException::SocketException(0,strError);
}
SocketException::SocketException(int nError,string strError)
{
	this->m_nError=nError;
	this->m_strLastError=strError;
}
SocketException::~SocketException()
{
	;
}
//获取错误id
int SocketException::getError()
{
	return this->m_nError;
}
/*
//获取错误信息
string& SocketException::getErrorString()//指针还是对象引用好?
{
	//string *pStr=new string(m_strError);
	//return *pStr;
	return m_strError;
}*/
//打印错误休息
void SocketException::printErrorMsg()
{
	SocketTools::showWinsockErrorMsg(m_nError);
}