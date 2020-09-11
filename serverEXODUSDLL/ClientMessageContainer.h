// Author/editor/coder: Simo
#pragma once
#include "NetworkMessage.h"
class CClientMessageContainer
{
public:
	CClientMessageContainer(int iClient, CNetworkMessage * pMsg);
	~CClientMessageContainer(void);

	int GetClient();
    CNetworkMessage * GetMessage();

	// For DLL:
	static int GetMessageType(CClientMessageContainer * pMsg);

private:
	int m_iClient;
    CNetworkMessage * m_pMsg;
};

