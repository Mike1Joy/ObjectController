// Author/editor/coder: Simo
#include "MessageServerComposer.h"

//#include "..\Playback\TimeKeeper.h"
#include "NetworkServerHandler.h"
#include "NetworkServerClient.h"


CNetworkServerClient::CNetworkServerClient(int iClient, CNetworkServerConnection * pConnection)
	: CAbstractThread()
{
	m_iClient = iClient;
	m_pConnection = pConnection;
	//m_pGeometry = NULL;
	m_bRunning = false;
	Start();
}


CNetworkServerClient::~CNetworkServerClient(void)
{
	m_bRunning = false;
}

/*
void 
CNetworkClient::SetGeometry(CGeometry * pGeometry)
{
	m_pGeometry = pGeometry;
}*/
int 
CNetworkServerClient::Run()
{
	if(m_bRunning)
	{
		return 0;
	}
	m_bRunning = true;
	//bool bGeometrySent = false;
    CNetworkMessage * pMsg = NULL;
	while(m_bRunning)
	{
		/*
		if(NULL != m_pGeometry && !bGeometrySent)
		{
			// Send geometry
			for(int iLevel = 0; iLevel < m_pGeometry->GetTotalLevels(); iLevel++)
			{
				CLevel * pLevel = m_pGeometry->GetLevel(iLevel);
				pMsg = CMessageComposer::SetLevelHeight(1, iLevel + 1, pLevel->GetRegion(0)->GetLevelHeight());
				if(NULL != pMsg)
				{
					if(!m_pConnection->SendMsg(pMsg))
					{
						delete pMsg; // We need a better way for error recovery!
					}
				}
				for(int iWall = 0; iWall < pLevel->GetTotalWalls(); iWall++)
				{
					CWall * pWall = pLevel->GetWall(iWall);
					pMsg = CMessageComposer::SetWall(1, iLevel + 1, iWall + 1, pWall->GetX(), pWall->GetZ(), pWall->GetLength(), pWall->GetRotation(), pWall->GetHeight());
					if(NULL != pMsg)
					{
						if(!m_pConnection->SendMsg(pMsg))
						{
							delete pMsg; // We need a better way for error recovery!
						}
					}
				}
			}
			bGeometrySent = true;
		}
		//else if(bGeometrySent)
		//{
		//	Sleep(1000);
		//}
		if(NULL != m_pPeople)
		{

		}*/
        CNetworkMessage * pMsg = NULL;
		while(NULL != (pMsg = m_pConnection->GetMsg()))
		{
			switch(pMsg->GetType()) // Now the time control messages go to NetworkHandler.
			{/*
			case CNetworkMessage::NWMSG_SET_RESET:
				{
					CTimeKeeper * pKeeper = CTimeKeeper::GetInstance();
					pKeeper->Reset();
				} break;
			case CNetworkMessage::NWMSG_SET_PLAY:
				{
					CTimeKeeper * pKeeper = CTimeKeeper::GetInstance();
					pKeeper->Play();
				} break;
			case CNetworkMessage::NWMSG_SET_STOP:
				{
					CTimeKeeper * pKeeper = CTimeKeeper::GetInstance();
					pKeeper->Stop();
				} break;
			case CNetworkMessage::NWMSG_SET_REWIND:
				{
					CTimeKeeper * pKeeper = CTimeKeeper::GetInstance();
					pKeeper->Rewind();
				} break;*/
				// Later on we have internal messages, such as ping and version checker that will not be passed on further.
			default:
				{
					CNetworkServerHandler::AddNewMessage(m_iClient, pMsg);
				} break;
			}
		}
		Sleep(10);
	}
	return 0;
}
void 
CNetworkServerClient::Send(CNetworkMessage * pMsg)
{
	if(NULL == pMsg) return;
	if(!m_pConnection->SendMsg(pMsg))
	{
		delete pMsg;
	}
}