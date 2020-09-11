// Author/editor/coder: Simo
#pragma once
#include "NetworkServerConnection.h"
//#include "..\Scene\Geometry.h"
//#include "..\Scene\People.h"
#include "AbstractThread.h"
class CNetworkServerClient : public CAbstractThread
{
public:
	CNetworkServerClient(int iClient, CNetworkServerConnection * pConnection);
	~CNetworkServerClient(void);

	int Run();

	// For dummy version of EXODUS
	//void SetGeometry(CGeometry * pGeometry);

	void Send(CNetworkMessage * pMsg);
private:
	CNetworkServerConnection * m_pConnection;

	// For dummy version of EXODUS
	//CGeometry * m_pGeometry;
	//CPeople * m_pPeople;

	bool m_bRunning;
	int m_iClient;
};

