// Author/editor/coder: Simo
#include "Lock.h"


CLock::CLock(void)
{
	InitializeCriticalSection(&m_CriticalSection);
}


CLock::~CLock(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}

void
CLock::Get()
{
	EnterCriticalSection(&m_CriticalSection);
}

void
CLock::Release()
{
	LeaveCriticalSection(&m_CriticalSection);
}