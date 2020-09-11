// Author/editor/coder: Simo
#include "NetworkClientHandler.h"
#include "Endianness.h"

CNetworkClientHandler * CNetworkClientHandler::m_pInstance = nullptr;
CLock CNetworkClientHandler::m_createLock;
CNetworkClientHandler::CNetworkClientHandler(void)
{
	m_pConnection = nullptr;
	m_pDistributer = nullptr;
	CEndianness::Init();
}


CNetworkClientHandler::~CNetworkClientHandler(void)
{
	if(nullptr != m_pDistributer)
	{
		delete m_pDistributer;
	}
	if(nullptr != m_pConnection)
	{
		m_pConnection->Disconnect();
		delete m_pConnection;
	}
}
bool 
CNetworkClientHandler::Connect(CStr * pAddr, CStr * pPort)
{
	CNetworkClientHandler * pHandler = CNetworkClientHandler::GetInstance();
	if(nullptr != pHandler)
	{
		CNetworkClientConnection * pConnection = new CNetworkClientConnection(pAddr, pPort);
		pConnection->Connect();
		pHandler->SetConnection(pConnection);
	}
	return true;
}
void 
CNetworkClientHandler::Disconnect()
{
	if(nullptr != m_pInstance)
	{
		m_pInstance->SetConnection(nullptr);
	}
}
void 
CNetworkClientHandler::CleanUp()
{
	if(nullptr != m_pInstance)
	{
		Disconnect();
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
CNetworkClientHandler *
CNetworkClientHandler::GetInstance()
{
	if(nullptr == m_pInstance)
	{
		m_createLock.Get();
		if(nullptr == m_pInstance) // check it again!
		{
			m_pInstance = new CNetworkClientHandler();
		}
		m_createLock.Release();
	}
	return m_pInstance;
}
void 
CNetworkClientHandler::SetConnection(CNetworkClientConnection * pConnection)
{
	if(nullptr != m_pConnection)
	{
		//delete m_pSimulationHandler;
		//m_pSimulationHandler = nullptr;
		delete m_pDistributer;
		m_pDistributer = nullptr;
		m_pConnection->Disconnect();
		delete m_pConnection;
		m_pConnection = nullptr;
	}
	m_pConnection = pConnection;
	if(nullptr != m_pConnection)
	{
		if(nullptr != m_pDistributer)
		{
			delete m_pDistributer;
		}
		//m_pSimulationHandler = new CNetworkSimulationHandler();
		m_pDistributer = new CNetworkClientMessageDistributer(m_pConnection/*, m_pSimulationHandler*/);
		m_pDistributer->Start();
	}
}
void 
CNetworkClientHandler::Send(CNetworkMessage * pMsg)
{
	CNetworkClientHandler * pHandler = m_pInstance;//GetInstance(); // Lets not initiate it here!
	if(nullptr != pHandler)
	{
		if(nullptr != pHandler->m_pConnection)
		{
			pHandler->m_pConnection->SendMsg(pMsg);
		} else
		{
			delete pMsg;
		}
	} else
	{
		delete pMsg;
	}
}
bool 
CNetworkClientHandler::IsConnected()
{
	if(nullptr != m_pInstance)
	{
		CNetworkClientConnection * pConnection = m_pInstance->m_pConnection;
		if(nullptr != pConnection)
		{
			return pConnection->IsConnected();
		}
	}
	return false;
}