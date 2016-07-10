#include "Thread.h"



Thread::Thread(ThreadParameter &threadParameter)
{
	this->m_pThreadParameter=&threadParameter;
	m_hThread=::CreateThread(NULL,0,CallBackOfCreateThread,(LPVOID)this,CREATE_SUSPENDED,NULL);		
}
//��ʼִ��
void Thread::start()
{
	ResumeThread(m_hThread);	
	//�رվ��
	::CloseHandle(m_hThread);
}
//����pObject(��Ҫʵ��Runnable�ӿ�)��run����
void Thread::callBackStartThread()
{
	((Runnable*)m_pThreadParameter->pObject)->run(m_pThreadParameter);
}

//�����̺߳�ص�,��ʾ��ʼִ�����߳���
DWORD WINAPI CallBackOfCreateThread(LPVOID pVoid)
{
	((Thread*)pVoid)->callBackStartThread();
	return true;
}
