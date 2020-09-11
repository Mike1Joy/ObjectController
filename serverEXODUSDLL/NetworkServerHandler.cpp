// Author/editor/coder: Simo
#include "Endianness.h"
#include "NetworkServerHandler.h"
extern bool StaticAddconnection(int anID);

CNetworkServerHandler * CNetworkServerHandler::m_pInstance = NULL;
CLock CNetworkServerHandler::m_createLock;
CNetworkServerHandler::CNetworkServerHandler(void)
{
	//m_pGeometry = NULL;
	m_bAccepting = false;
	m_pListener = NULL;
	m_pSenderInterface = new CNetworkServerSenderInterface(); // This is a lightweight class to initiate!
	CEndianness::Init();
}


CNetworkServerHandler::~CNetworkServerHandler(void)
{
	if(NULL != m_pListener)
	{
		delete m_pListener;
	}
	if(NULL != m_pSenderInterface)
	{
		delete m_pSenderInterface;
	}
	m_Clients.DeleteAll(); // We should probably clean them up!
	m_newMessages.DeleteAllData();
}

bool 
CNetworkServerHandler::AcceptConnections(char * pszPort) // Added port argument quickly (Simo 06/02/2018)
{
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		if(NULL == pInstance->m_pListener)
		{
            CStr * pCopy = new CStr(pszPort); // Dubious quick addition (Simo 06/02/2018)
            pszPort = (char *)pCopy->GetArray();
			pInstance->m_pListener = new CNetworkServerListener(pszPort);
		}
		pInstance->m_pListener->StartListening();
		pInstance->m_bAccepting = true;
		return true;
	}
	return false;
}

bool 
CNetworkServerHandler::StopAccepting()
{
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		if(NULL == pInstance->m_pListener)
		{
			pInstance->m_bAccepting = false;
			return true;
		}
		pInstance->m_pListener->StopListening();
		pInstance->m_bAccepting = false;
		return true;
	}
	return false;
}

bool 
CNetworkServerHandler::IsAccepting()
{
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		return pInstance->m_bAccepting;
	}
	return false;
}
/*
void 
CNetworkHandler::SetGeometry(CGeometry * pGeometry)
{
	CNetworkHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		if(NULL != pInstance->m_pGeometry)
		{
			delete pInstance->m_pGeometry;
		}
		pInstance->m_pGeometry = pGeometry;
	}
}
*/
CNetworkServerHandler *
CNetworkServerHandler::GetInstance()
{
	if(NULL == m_pInstance)
	{
		m_createLock.Get();
		if(NULL == m_pInstance) // check it again!
		{
			m_pInstance = new CNetworkServerHandler();
		}
		m_createLock.Release();
	}
	return m_pInstance;
}
void 
CNetworkServerHandler::CleanUp()
{
	if(NULL != m_pInstance)
	{
		m_createLock.Get();
		StopAccepting();
		delete m_pInstance;
		m_pInstance = NULL;
		m_createLock.Release();
	}
}
CNetworkServerSenderInterface *
CNetworkServerHandler::GetSenderInterface()
{
	if(NULL != m_pInstance)
	{
		return m_pInstance->m_pSenderInterface;
	}
	return NULL;
}
void 
CNetworkServerHandler::AddClient(CNetworkServerClient * pClient)
{
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		if(NULL != pClient)
		{
			pInstance->m_Clients.Add(pClient);
            StaticAddconnection(pInstance->m_Clients.Size() - 1);
			//pClient->SetGeometry(pInstance->m_pGeometry); // For dummy version of EXODUS
			//CGeometryHandler * pGeometry = GetGeometry();
			//pGeometry->AddFreshClient(pClient);
		}
	}
}
void 
CNetworkServerHandler::BroadcastMessage(CNetworkMessage * pMsg)
{
	if(NULL == pMsg) return;
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		for(int iClient = 0; iClient < pInstance->m_Clients.Size(); iClient++)
		{
			CNetworkServerClient * pClient = pInstance->m_Clients.Get(iClient);
			pClient->Send(pMsg->GetCopy());
		}
	}
	delete pMsg;
}
bool 
CNetworkServerHandler::SendMessage(int iClient, CNetworkMessage * pMsg)
{
	if(NULL == pMsg || iClient < 0) return false;
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		if(iClient >= pInstance->m_Clients.Size()) return false;
		CNetworkServerClient * pClient = pInstance->m_Clients.Get(iClient);
		pClient->Send(pMsg);
		return true;
	}
	return false;
}
CClientMessageContainer * 
CNetworkServerHandler::GetNewMessage()
{
	CNetworkServerHandler * pInstance = GetInstance();
	if(NULL != pInstance)
	{
		return pInstance->m_newMessages.Take();
	}
	return NULL;
}
int 
CNetworkServerHandler::GetNextClientId()
{
	if(NULL != m_pInstance)
	{
		return m_pInstance->m_Clients.Size();
	}
	return NULL;
}
void 
CNetworkServerHandler::AddNewMessage(int iClient, CNetworkMessage * pMsg)
{
	if(NULL != m_pInstance)
	{
		m_pInstance->m_newMessages.Add(new CClientMessageContainer(iClient, pMsg));
	}
}