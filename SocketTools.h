#ifndef _SocketTools_h_ 
#define _SocketTools_h_ 

#include <string>  
using namespace std;

#define CP_GBK 950

#ifdef WIN32
# define ioctl ioctlsocket
#endif

class SocketTools
{
public:
	static int showWinsockErrorMsg(int ErrCode,bool bWantReturn=true);
	static int initSocketContext();
	static int cleanUpSocketContext();

	static int bytesToInt(char *buf,int sizeOfInt);
	static void intToBytes(char buf[],int value);
	//static char* SocketTools::gbkToUtf8(char *strGBK);
	static int gbkToUtf8(string& strUtf8,char *strGBK);
	static int utf8ToGbk(string& strGbk,char* strUtf8);
	static int gbkToUnicode(wstring& strUnicode,char *strGBK);
	static int gb2312ToUnicode(wstring& result,char *strGb2312);

	static void replaceString(string & strToReplace, const string & strSrc, const string &strDst);
};
#endif