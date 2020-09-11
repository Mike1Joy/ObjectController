// Author/editor/coder: Simo
#pragma once
#include "AbstractThread.h"

// Listens for new connections!
class CNetworkServerListener : public CAbstractThread
{
public:
	CNetworkServerListener(char * pszPort);
	~CNetworkServerListener(void);

	void StartListening();
	void StopListening();

	int Run();

private:
	void Listen();

	bool m_bListening;
	bool m_bWantMoreConnections;

	void * m_pListenSocket;
    char * m_pszPort;
};

