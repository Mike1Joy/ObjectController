// Author/editor/coder: Simo
#pragma once
#include "Defines.h"
#include "NetworkMessage.h"
#include "NetworkServerSenderInterface.h"
#include "ClientMessageContainer.h"

// This class works as the exported interface of NetworkHandler, all these functions are directly called from NetworkHandler
class EXODUS_SERVER_DLL_API CNetworkServerInterface
{
public:
	static bool AcceptConnections();
	static bool StopAccepting();
	static bool IsAccepting();
	static void CleanUp();
	static CNetworkServerSenderInterface * GetSenderInterface();

	static void BroadcastMessage(CNetworkMessage * pMsg);
	static bool SendMessage(int iClient, CNetworkMessage * pMsg);
	static CClientMessageContainer * GetNewMessage();

	static CNetworkMessage * GetMessageFromContainer(CClientMessageContainer * pMsg);

	static char * GetHelloWorld();

	static void DeleteObj(void * pObj);
private:
	CNetworkServerInterface(void);
	~CNetworkServerInterface(void);
};

