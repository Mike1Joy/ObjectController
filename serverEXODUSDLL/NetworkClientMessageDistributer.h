// Author/editor/coder: Simo
#pragma once
#include "AbstractThread.h"
#include "NetworkClientConnection.h"

class CNetworkClientMessageDistributer : public CAbstractThread
{
public:
	CNetworkClientMessageDistributer(CNetworkClientConnection * pConnection);
	~CNetworkClientMessageDistributer(void);

	int Run() override;

private:
	void ProcessMsg(CNetworkMessage * pMsg);

	CNetworkClientConnection * m_pConnection;

	void SetLevel(CNetworkMessage * pMsg);
	void SetWall(CNetworkMessage * pMsg);
	void SetPerson(CNetworkMessage * pMsg);
    void SetPersonAuggmed(CNetworkMessage * pMsg);
    void SetTimeAuggmed(CNetworkMessage * pMsg);
    void SetPersonMoveAuggmed(CNetworkMessage * pMsg);
	//void SetPersonAction(CNetworkMessage * pMsg);
	void SetFrame(CNetworkMessage * pMsg);
    void SetDeleteAuggmed(CNetworkMessage * pMsg);
    void SetPersonAddedAuggmed(CNetworkMessage * pMsg);

};

