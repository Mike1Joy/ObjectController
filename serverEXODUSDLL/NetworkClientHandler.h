// Author/editor/coder: Simo
#pragma once
//#include "WinSockWrapper.h"

// Initialization by http://msdn.microsoft.com/en-us/library/windows/desktop/ms737629(v=vs.85).aspx

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
#include "NetworkClientConnection.h"
#include "Str.h"
#include "Lock.h"
#include "NetworkClientMessageDistributer.h"

class CNetworkClientHandler
{
public:
	static bool Connect(CStr * pAddr, CStr * pPort);
	static void Disconnect();
	static void CleanUp();
	static void Send(CNetworkMessage * pMsg);
	static bool IsConnected();
private:
	static CNetworkClientHandler * GetInstance();

	CNetworkClientHandler(void);
	~CNetworkClientHandler(void);

	static CNetworkClientHandler * m_pInstance;
	static CLock m_createLock;

	void SetConnection(CNetworkClientConnection * pConnection);
	CNetworkClientConnection * m_pConnection;
	CNetworkClientMessageDistributer * m_pDistributer;
};

