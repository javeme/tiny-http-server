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
//��ȡ����id
int SocketException::getError()
{
	return this->m_nError;
}
/*
//��ȡ������Ϣ
string& SocketException::getErrorString()//ָ�뻹�Ƕ������ú�?
{
	//string *pStr=new string(m_strError);
	//return *pStr;
	return m_strError;
}*/
//��ӡ������Ϣ
void SocketException::printErrorMsg()
{
	SocketTools::showWinsockErrorMsg(m_nError);
}