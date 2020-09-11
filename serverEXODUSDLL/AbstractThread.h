// Author/editor/coder: Simo
#pragma once
#ifndef ABSTRACTTHREAD_H
#define ABSTRACTTHREAD_H
#include <windows.h>

class CAbstractThread
{
public:
	CAbstractThread(void);
	~CAbstractThread(void);

	HANDLE GetThreadHandle();
	HANDLE Start();
	virtual int Run();
private:
	static int StaticRun(void * pThis);
	HANDLE m_ThisThreadHandle;
};

#endif