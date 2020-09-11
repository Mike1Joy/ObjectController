// Author/editor/coder: Simo
#include "ClientMessageContainer.h"

CClientMessageContainer::CClientMessageContainer(int iClient, CNetworkMessage * pMsg)
{
	m_iClient = iClient;
	m_pMsg = pMsg;
}

CClientMessageContainer::~CClientMessageContainer(void)
{
	if(NULL != m_pMsg) delete m_pMsg;
}

int 
CClientMessageContainer::GetClient()
{
	return m_iClient;
}
CNetworkMessage *
CClientMessageContainer::GetMessage()
{
	return m_pMsg;
}

int 
CClientMessageContainer::GetMessageType(CClientMessageContainer * pMsg)
{
	if(NULL == pMsg || NULL == pMsg->m_pMsg) return 0;
	return pMsg->m_pMsg->GetType();
}