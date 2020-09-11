// Author/editor/coder: Simo
#include "AbstractThread.h"

struct workAround
{
	CAbstractThread * this_thread;
};

CAbstractThread::CAbstractThread(void)
{
	m_ThisThreadHandle = NULL;
}

CAbstractThread::~CAbstractThread(void)
{
	CloseHandle(m_ThisThreadHandle);
}
HANDLE
CAbstractThread::Start()
{
	workAround * wA = new workAround;
	wA->this_thread = this;
	// It is easiest to let the thread object keep its own handle as we got pointer to the thread object
	// so we can request the thread handle at any time we want.
	return m_ThisThreadHandle = ::CreateThread(NULL, 0, (unsigned long (_stdcall *)(void *))this->StaticRun,
		(void *)wA, 0, NULL);
}

int
CAbstractThread::StaticRun(void * pThis)
{
 	workAround * wA = (workAround *)pThis;

	CAbstractThread * thread = wA->this_thread;
	wA->this_thread = nullptr;
	//delete wA;
	return thread->Run();
}
int
CAbstractThread::Run()
{
	return 0; // Virtual
}
HANDLE 
CAbstractThread::GetThreadHandle()
{
	return m_ThisThreadHandle;
}