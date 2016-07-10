#include "Thread.h"



Thread::Thread(ThreadParameter &threadParameter)
{
	this->m_pThreadParameter=&threadParameter;
	m_hThread=::CreateThread(NULL,0,CallBackOfCreateThread,(LPVOID)this,CREATE_SUSPENDED,NULL);		
}
//开始执行
void Thread::start()
{
	ResumeThread(m_hThread);	
	//关闭句柄
	::CloseHandle(m_hThread);
}
//调用pObject(需要实现Runnable接口)的run函数
void Thread::callBackStartThread()
{
	((Runnable*)m_pThreadParameter->pObject)->run(m_pThreadParameter);
}

//创建线程后回调,表示开始执行新线程了
DWORD WINAPI CallBackOfCreateThread(LPVOID pVoid)
{
	((Thread*)pVoid)->callBackStartThread();
	return true;
}
