// Author/editor/coder: Simo
#include "NetworkServerHandler.h"
#include "NetworkServerInterface.h"

CNetworkServerInterface::CNetworkServerInterface(void){}
CNetworkServerInterface::~CNetworkServerInterface(void){}


bool 
CNetworkServerInterface::AcceptConnections()
{
	char* c;
	c[0] = '9';
	c[1] = '0';
	c[2] = '0';
	c[3] = '0';
	c[4] = '\0';
	return CNetworkServerHandler::AcceptConnections(c);
}
bool 
CNetworkServerInterface::StopAccepting()
{
	return CNetworkServerHandler::StopAccepting();
}
bool 
CNetworkServerInterface::IsAccepting()
{
	return CNetworkServerHandler::IsAccepting();
}
void 
CNetworkServerInterface::CleanUp()
{
	CNetworkServerHandler::CleanUp();
}
CNetworkServerSenderInterface *
CNetworkServerInterface::GetSenderInterface()
{
	return CNetworkServerHandler::GetSenderInterface();
}

void 
CNetworkServerInterface::BroadcastMessage(CNetworkMessage * pMsg)
{
	CNetworkServerHandler::BroadcastMessage(pMsg);
}
bool 
CNetworkServerInterface::SendMessage(int iClient, CNetworkMessage * pMsg)
{
	return CNetworkServerHandler::SendMessage(iClient, pMsg);
}
CClientMessageContainer * 
CNetworkServerInterface::GetNewMessage()
{
	return CNetworkServerHandler::GetNewMessage();
}
char * 
CNetworkServerInterface::GetHelloWorld()
{
	char * pMsg = new char[20];
	const char * pConstMsg = "Hello World!\0";
	int iLength = strlen(pConstMsg) + 1;
	int i = 0;
	for(; i < iLength; i++)
	{
		pMsg[i] = pConstMsg[i];
	}
	return pMsg;
}
void 
CNetworkServerInterface::DeleteObj(void * pObj)
{
	if(NULL == pObj) return;
	delete pObj;
}
CNetworkMessage *
CNetworkServerInterface::GetMessageFromContainer(CClientMessageContainer * pMsg)
{
	// commented out below and now works - may break something else in future (12/02/2020)
	//if(NULL != pMsg) return pMsg->GetMessageA();
	return NULL;
}