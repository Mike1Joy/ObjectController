// Author/editor/coder: Simo
#pragma once
#include "Str.h"
#include "AbstractThread.h"
#include "ConcurrentLinkedQueue.h"
#include "NetworkMessage.h"
class CNetworkClientConnection : public CAbstractThread
{
public:
	CNetworkClientConnection(CStr * pAddr, CStr * pPort);
	~CNetworkClientConnection(void);

	bool Connect();
	bool Disconnect();
	bool IsConnected();
	bool IsConnectionTried();

	int Run() override; // External function should never call this function directly!

    CNetworkMessage * GetMsg();
	bool SendMsg(CNetworkMessage * pMsg);
private:
	class CSenderHandler : public CAbstractThread
	{
	public:
		CSenderHandler(CNetworkClientConnection * pConnection)
		{
			m_pConnection = pConnection;
		}
		~CSenderHandler(void){}
		int Run() override
		{
			m_pConnection->Send();
            return 0;
		}
	private:
		CNetworkClientConnection * m_pConnection;
	};
	int Send();
	void AddData(char * pBytes, int iLength);
	int GetInt(char * pBytes);
	void IntToData(char * pBytes, int iValue);

	void SendFirstMessage();

	char * m_pLeftOvers;
	int m_iLeftOversLength;
	int m_iLeftOversOffset;

	bool m_bRunning;
	bool m_bConnected;
	bool m_bConnectionTried;
	CStr * m_pAddress;
	CStr * m_pPort;

	void * m_pSocket;

	CConcurrentLinkedQueue<CNetworkMessage> m_sendMessages;
	CConcurrentLinkedQueue<CNetworkMessage> m_receiveMessages;
	CSenderHandler * m_pSender;
};

