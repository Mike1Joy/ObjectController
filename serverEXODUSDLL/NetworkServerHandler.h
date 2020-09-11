// Author/editor/coder: Simo
#pragma once
//#include "..\Scene\Geometry.h"
#include "Lock.h"
#include "ArrayList.h"
#include "NetworkServerListener.h"
#include "NetworkServerClient.h"
#include "NetworkServerSenderInterface.h"
#include "ClientMessageContainer.h"

class CNetworkServerHandler
{
	friend class CNetworkServerSenderInterface;
	friend class CNetworkServerClient;
	friend class CNetworkServerListener;
public:
	static bool AcceptConnections(char * pszPort);
	static bool StopAccepting();
	static bool IsAccepting();
	static void CleanUp();
	static CNetworkServerSenderInterface * GetSenderInterface();


	// For dummy-EXODUS version:
	//static void SetGeometry(CGeometry * pGeometry);

	static void BroadcastMessage(CNetworkMessage * pMsg);
	static bool SendMessage(int iClient, CNetworkMessage * pMsg);
	static CClientMessageContainer * GetNewMessage();

private:
	static int GetNextClientId();
	static void AddNewMessage(int iClient, CNetworkMessage * pMsg);
	static CNetworkServerHandler * GetInstance();
	static void AddClient(CNetworkServerClient * pClient);

	CNetworkServerHandler(void);
	~CNetworkServerHandler(void);

	static CNetworkServerHandler * m_pInstance;
	static CLock m_createLock;
	
	bool m_bAccepting;
	CNetworkServerListener * m_pListener;

	// For dummy-EXODUS version:
	//CGeometry * m_pGeometry;

	CArrayList<CNetworkServerClient> m_Clients;

	CNetworkServerSenderInterface * m_pSenderInterface;
	CConcurrentLinkedQueue<CClientMessageContainer> m_newMessages;
};

