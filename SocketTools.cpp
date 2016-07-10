
#include <stdio.h>
#include <winsock2.h>

#include "SocketTools.h"
#include "SocketException.h"

//初始socket环境
int SocketTools::initSocketContext()
{
#ifndef WIN32
	return 1;
#endif
	WSADATA wsad;
	WORD ver;
	//使用2.0的库函数
	ver=MAKEWORD(2,0);		
	int nRetCode=WSAStartup(ver,&wsad);	
	if(nRetCode)
	{
		throw SocketException(::WSAGetLastError());
	}
	return nRetCode;
}
//清除socket环境
int SocketTools::cleanUpSocketContext()
{
#ifndef WIN32
	return 1;
#endif
	return WSACleanup();
}
//字节数组转换为整型
int SocketTools::bytesToInt(char *buf,int sizeOfInt)
{
	int value=0;
	for(int i=0;i<sizeOfInt;i++)
	{
		value+=(buf[i] & 0xff)<<8*i;
	}
	return value;
}
//整型转换为字节数组
void SocketTools::intToBytes(char buf[],int value)
{
	for(int i=0;i<sizeof(value);i++)
	{
		buf[i]=(value>>i*8) & 0xff;
	}
}
//gbk转换unicode
int SocketTools::gbkToUnicode(wstring& strUnicode,char *strGBK)
{
	//MultiByteToWideChar gb2312转换为unicode
	int len;
	wchar_t *pUnicode;	
	if (!strGBK)
	{
		return NULL;
	}
	len =::MultiByteToWideChar(CP_GBK,0,strGBK,-1,NULL,0);
	if (len <1)
	{
		return 0;
	}
	pUnicode = (wchar_t *) malloc(sizeof(wchar_t) * len);
	if (!pUnicode)
	{
		return 0;
	}
	len = ::MultiByteToWideChar(CP_GBK, 0, strGBK, -1, pUnicode, len);
	if (len<1)
	{
		free(pUnicode);
		return 0;
	}
	//*
	char *strUTF8;
	len = ::WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL);
	if (len<1)
	{
		free(pUnicode);
		return 0;
	}
	strUTF8 = (char *) malloc(sizeof(char) * len);
	if (!strUTF8)
	{
		free(pUnicode);
		return 0;
	}
	len = ::WideCharToMultiByte (CP_UTF8, 0, pUnicode, -1, strUTF8, len, NULL,NULL);
	free(pUnicode);
	if (len<1)
	{
		free(strUTF8);
		return 0;
	}//*/
	//strUnicode=pUnicode;
	strUnicode=(wchar_t *)strUTF8;
	free(pUnicode);
	return len;	
}
//gb2312转换unicode --有问题!
int SocketTools::gb2312ToUnicode(wstring& result,char *strGb2312)   
{   
	int n = MultiByteToWideChar( CP_ACP, 0, strGb2312, -1, NULL, 0 );   
	result.resize(n);   
	::MultiByteToWideChar( CP_ACP, 0, strGb2312, -1, (LPWSTR)result.c_str(), result.length());
	return n;
}  
//gbk转换为utf-8
int SocketTools::gbkToUtf8(string& strUtf8,char *strGBK)
{	
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	strUtf8=str;
	delete[]str;
	return len;
}  
//utf8转换为gbk编码
int SocketTools::utf8ToGbk(string& strGbk,char* strUtf8)
{
    int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, wszGBK, len); 
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 
    char *szGBK=new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, szGBK, len, NULL,NULL); 
    strGbk = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
	return len;
}  
//替换字符串
void SocketTools::replaceString(string & strToReplace, const string & strSrc, const string &strDst)
{ 
	string::size_type pos=0; 
	string::size_type srcLen=strSrc.size(); 
	string::size_type dstLen=strDst.size();
	while( (pos=strToReplace.find(strSrc, pos)) != string::npos)
	{  
		strToReplace.replace(pos, srcLen, strDst);  
		pos += dstLen;
	}
} 

//显示错误
int SocketTools::showWinsockErrorMsg(int ErrCode,bool bWantReturn)
{
	switch(ErrCode)
	{
		case WSABASEERR: printf("WSABASEERR"); break;
		case WSAEINTR: printf("WSAEINTR"); break;
		case WSAEBADF: printf("WSAEBADF"); break;
		case WSAEACCES: printf("WSAEACCES"); break;
		case WSAEFAULT: printf("WSAEFAULT"); break;
		case WSAEINVAL: printf("WSAEINVAL"); break;
		case WSAEMFILE: printf("WSAEMFILE"); break;
		case WSAEWOULDBLOCK: printf("WSAEWOULDBLOCK"); break;
		case WSAEINPROGRESS: printf("WSAEINPROGRESS"); break;
		case WSAEALREADY: printf("WSAEALREADY"); break;
		case WSAENOTSOCK: printf("WSAENOTSOCK"); break;
		case WSAEDESTADDRREQ: printf("WSAEDESTADDRREQ"); break;
		case WSAEMSGSIZE: printf("WSAEMSGSIZE"); break;
		case WSAEPROTOTYPE: printf("WSAEPROTOTYPE"); break;
		case WSAENOPROTOOPT: printf("WSAENOPROTOOPT"); break;
		case WSAEPROTONOSUPPORT: printf("WSAEPROTONOSUPPORT"); break;
		case WSAESOCKTNOSUPPORT: printf("WSAESOCKTNOSUPPORT"); break;
		case WSAEOPNOTSUPP: printf("WSAEOPNOTSUPP"); break;
		case WSAEPFNOSUPPORT: printf("WSAEPFNOSUPPORT"); break;
		case WSAEAFNOSUPPORT: printf("WSAEAFNOSUPPORT"); break;
		case WSAEADDRINUSE: printf("WSAEADDRINUSE"); break;
		case WSAEADDRNOTAVAIL: printf("WSAEADDRNOTAVAIL"); break;
		case WSAENETDOWN: printf("WSAENETDOWN"); break;
		case WSAENETUNREACH: printf("WSAENETUNREACH"); break;
		case WSAENETRESET: printf("WSAENETRESET"); break;
		case WSAECONNABORTED: printf("WSAECONNABORTED"); break;
		case WSAECONNRESET: printf("WSAECONNRESET"); break;
		case WSAENOBUFS: printf("WSAENOBUFS"); break;
		case WSAEISCONN: printf("WSAEISCONN"); break;
		case WSAENOTCONN: printf("WSAENOTCONN"); break;
		case WSAESHUTDOWN: printf("WSAESHUTDOWN"); break;
		case WSAETOOMANYREFS: printf("WSAETOOMANYREFS"); break;
		case WSAETIMEDOUT: printf("WSAETIMEDOUT"); break;
		case WSAECONNREFUSED: printf("WSAECONNREFUSED"); break;
		case WSAELOOP: printf("WSAELOOP"); break;
		case WSAENAMETOOLONG: printf("WSAENAMETOOLONG"); break;
		case WSAEHOSTDOWN: printf("WSAEHOSTDOWN"); break;
		case WSAEHOSTUNREACH: printf("WSAEHOSTUNREACH"); break;
		case WSAENOTEMPTY: printf("WSAENOTEMPTY"); break;
		case WSAEPROCLIM: printf("WSAEPROCLIM"); break;
		case WSAEUSERS: printf("WSAEUSERS"); break;
		case WSAEDQUOT: printf("WSAEDQUOT"); break;
		case WSAESTALE: printf("WSAESTALE"); break;
		case WSAEREMOTE: printf("WSAEREMOTE"); break;
		case WSASYSNOTREADY: printf("WSASYSNOTREADY"); break;
		case WSAVERNOTSUPPORTED: printf("WSAVERNOTSUPPORTED"); break;
		case WSANOTINITIALISED: printf("WSANOTINITIALISED"); break;
		case WSAEDISCON: printf("WSAEDISCON"); break;
		case WSAENOMORE: printf("WSAENOMORE"); break;
		case WSAECANCELLED: printf("WSAECANCELLED"); break;
		case WSAEINVALIDPROCTABLE: printf("WSAEINVALIDPROCTABLE"); break;
		case WSAEINVALIDPROVIDER: printf("WSAEINVALIDPROVIDER"); break;
		case WSAEPROVIDERFAILEDINIT: printf("WSAEPROVIDERFAILEDINIT"); break;
		case WSASYSCALLFAILURE: printf("WSASYSCALLFAILURE"); break;
		case WSASERVICE_NOT_FOUND: printf("WSASERVICE_NOT_FOUND"); break;
		case WSATYPE_NOT_FOUND: printf("WSATYPE_NOT_FOUND"); break;
		case WSA_E_NO_MORE: printf("WSA_E_NO_MORE"); break;
		case WSA_E_CANCELLED: printf("WSA_E_CANCELLED"); break;
		case WSAEREFUSED: printf("WSAEREFUSED"); break;
		case WSAHOST_NOT_FOUND: printf("WSAHOST_NOT_FOUND"); break;
		case WSATRY_AGAIN: printf("WSATRY_AGAIN"); break;
		case WSANO_RECOVERY: printf("WSANO_RECOVERY"); break;
		case WSANO_DATA: printf("WSANO_DATA"); break;
		//case WSANO_ADDRESS: printf("WSANO_ADDRESS"); break;
		case WSA_QOS_RECEIVERS: printf("WSA_QOS_RECEIVERS"); break;
		case WSA_QOS_SENDERS: printf("WSA_QOS_SENDERS"); break;
		case WSA_QOS_NO_SENDERS: printf("WSA_QOS_NO_SENDERS"); break;
		case WSA_QOS_NO_RECEIVERS: printf("WSA_QOS_NO_RECEIVERS"); break;
		case WSA_QOS_REQUEST_CONFIRMED: printf("WSA_QOS_REQUEST_CONFIRMED"); break;
		case WSA_QOS_ADMISSION_FAILURE: printf("WSA_QOS_ADMISSION_FAILURE"); break;
		case WSA_QOS_POLICY_FAILURE: printf("WSA_QOS_POLICY_FAILURE"); break;
		case WSA_QOS_BAD_STYLE: printf("WSA_QOS_BAD_STYLE"); break;
		case WSA_QOS_BAD_OBJECT: printf("WSA_QOS_BAD_OBJECT"); break;
		case WSA_QOS_TRAFFIC_CTRL_ERROR: printf("WSA_QOS_TRAFFIC_CTRL_ERROR"); break;
		case WSA_QOS_GENERIC_ERROR: printf("WSA_QOS_GENERIC_ERROR"); break;
	}
	if(bWantReturn)
		printf("\n");
	return 0;
}
