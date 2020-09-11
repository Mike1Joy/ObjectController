// Author/editor/coder: Simo
#pragma once
#include <Windows.h>
class CLock
{
public:
	CLock(void);
	~CLock(void);
	void Get();
	void Release();
private:
	CRITICAL_SECTION m_CriticalSection;
};

