/*
*edit by bluemei
*write for java-users
*/

#ifndef _Thread_h_
#define _Thread_h_

#include <windows.h>
#include <winbase.h>

#include "ThreadParameter.h"
#include "Runnable.h"

class Thread
{
public:
	Thread(ThreadParameter &threadParameter);
protected:
	ThreadParameter *m_pThreadParameter;
	HANDLE m_hThread;
public:
	void start();
	void callBackStartThread();
};
DWORD WINAPI CallBackOfCreateThread(LPVOID pVoid);

#endif