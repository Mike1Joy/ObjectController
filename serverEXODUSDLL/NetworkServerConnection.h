// Author/editor/coder: Simo
#pragma once
#include "AbstractThread.h"
#include "ConcurrentLinkedQueue.h"
#include "NetworkMessage.h"

// Handles individual client connections!
class CNetworkServerConnection : public CAbstractThread
{
public:
	CNetworkServerConnection(void * pSocket);
	~CNetworkServerConnection(void);
	
	bool Disconnect();
	bool IsConnected();

	int Run();
	CNetworkMessage * GetMsg();
	bool SendMsg(CNetworkMessage * pMsg);
private:
	class CSenderHandler : public CAbstractThread
	{
	public:
		CSenderHandler(CNetworkServerConnection * pConnection)
		{
			m_pConnection = pConnection;
		}
		~CSenderHandler(void){}
		int Run()
		{
			return m_pConnection->Send();
		}
	private:
		CNetworkServerConnection * m_pConnection;
	};
	int Send();
	void Receive();
	void AddData(char * pBytes, int iLength);
	int GetInt(char * pBytes);
	void IntToData(char * pBytes, int iValue);

	char * m_pLeftOvers;
	int m_iLeftOversLength;
	int m_iLeftOversOffset;

	bool m_bConnected;
	bool m_bRunningSend;
	bool m_bRunningReceive;
	bool m_bReceiveFinished;
	void * m_pSocket;

	CConcurrentLinkedQueue<CNetworkMessage> m_sendMessages;
	CConcurrentLinkedQueue<CNetworkMessage> m_receiveMessages;
	CSenderHandler * m_pSender;
};

