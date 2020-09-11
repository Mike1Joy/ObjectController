// Author/editor/coder: Simo

#include "NetworkClientMessageDistributer.h"
#include "MessageClientParser.h"
#include <Windows.h>

CNetworkClientMessageDistributer::CNetworkClientMessageDistributer(CNetworkClientConnection * pConnection)
{
	m_pConnection = pConnection;
}


CNetworkClientMessageDistributer::~CNetworkClientMessageDistributer(void)
{
}

int 
CNetworkClientMessageDistributer::Run()
{
	long long llTime = timeGetTime();
	while(!m_pConnection->IsConnectionTried() && (timeGetTime() - llTime < 10000)) // We wait for the connection for 10 seconds!
	{
		Sleep(100);
	}
	while(m_pConnection->IsConnected())
	{
        CNetworkMessage * pMessage = nullptr;
		while(nullptr != (pMessage = m_pConnection->GetMsg()))
		{
			ProcessMsg(pMessage);
			delete pMessage;
			pMessage = nullptr; // It may not be necessary to set it to nullptr, but its a good habit :)
		}
		Sleep(10); // No messages, lets sleep! :)
	}
    return 0;
}
void 
CNetworkClientMessageDistributer::ProcessMsg(CNetworkMessage * pMsg)
{
    switch (pMsg->GetType())
    {
        /*
    case CNetworkMessage::NWMSG_SET_LEVEL: SetLevel(pMsg); break;
    case CNetworkMessage::NWMSG_SET_WALL: SetWall(pMsg); break;
    case CNetworkMessage::NWMSG_SET_PERSON: SetPerson(pMsg); break;
        //case CNetworkMessage::NWMSG_SET_PERSON_ACTION: SetPersonAction(pMsg); break;
    case CNetworkMessage::NWMSG_SET_FRAME: SetFrame(pMsg); break;*/
        /*
    case CAuggmedMessageIds::EXMS_FIRELOADED: OutputDebugString("Received: EXMS_FIRELOADED\n"); break;
    case CAuggmedMessageIds::EXMS_SIMULATION_READY: OutputDebugString("Received: EXMS_SIMULATION_READY\n"); break;
    case CAuggmedMessageIds::EXMS_SCENARIOFAILURE: OutputDebugString("Received: EXMS_SCENARIOFAILURE\n"); break;
    case CAuggmedMessageIds::EXMS_SCENARIOLOADED: OutputDebugString("Received: EXMS_SCENARIOLOADED\n"); break;
    case CAuggmedMessageIds::EXMS_FIRESTATED: OutputDebugString("Received: EXMS_FIRESTATED\n"); break;
    case CAuggmedMessageIds::EXMS_ALARMSTARTED: OutputDebugString("Received: EXMS_ALARMSTARTED\n"); break;
    case CAuggmedMessageIds::EXMS_ADDED: {OutputDebugString("Received: EXMS_ADDED\n"); SetPersonAddedAuggmed(pMsg);} break;
    case CAuggmedMessageIds::EXMS_MOVED: OutputDebugString("Received: EXMS_MOVED\n"); break;
    case CAuggmedMessageIds::EXMS_OBSTRUCTIONADDED: OutputDebugString("Received: EXMS_OBSTRUCTIONADDED\n"); break;
    case CAuggmedMessageIds::EXMS_OBSTRUCTIONFAILURE: OutputDebugString("Received: EXMS_OBSTRUCTIONFAILURE\n"); break;
    case CAuggmedMessageIds::EXMS_KILLED: OutputDebugString("Received: EXMS_KILLED\n"); break;
    case CAuggmedMessageIds::EXMS_BOMBDETONATED: OutputDebugString("Received: EXMS_BOMBDETONATED\n"); break;
    case CAuggmedMessageIds::EXMS_AGENTSHOT: OutputDebugString("Received: EXMS_AGENTSHOT\n"); break;
    case CAuggmedMessageIds::EXMS_AGENTSTABBED: OutputDebugString("Received: EXMS_AGENTSTABBED\n"); break;
    case CAuggmedMessageIds::EXMS_ADD: {OutputDebugString("Received: EXMS_ADD\n"); SetPersonAuggmed(pMsg);} break;
    case CAuggmedMessageIds::EXMS_MOVE: {SetPersonMoveAuggmed(pMsg); } break;
    case CAuggmedMessageIds::EXMS_TIME: {SetTimeAuggmed(pMsg); } break;
    case CAuggmedMessageIds::EXMS_DELETE: {OutputDebugString("Received: EXMS_DELETE\n"); SetDeleteAuggmed(pMsg);} break;
	    default:
        {
            CStr msg("Received: ");
            msg.Append(pMsg->GetType());
            msg.Append(" (UNKNOWN)\n");
            OutputDebugString(msg.GetArray());
        } break;
        */
	}
}
void 
CNetworkClientMessageDistributer::SetLevel(CNetworkMessage * pMsg)
{
	//CNetworkLevel * pLevel = CMessageClientParser::SetLevel(pMsg);
	//if(nullptr == pLevel) return;
}
void 
CNetworkClientMessageDistributer::SetWall(CNetworkMessage * pMsg)
{
    /*
	CNetworkWall * pWall = CMessageClientParser::SetWall(pMsg);
	if(nullptr == pWall) return;
    */
}
void 
CNetworkClientMessageDistributer::SetPerson(CNetworkMessage * pMsg)
{
    /*
	CNetworkPerson * pPerson = CMessageClientParser::SetPerson(pMsg);
	if(nullptr == pPerson) return;
	pPerson->SetYRotation(180.0f - pPerson->GetYRotation()); // Change direction and turn around :)
    */
}
void
CNetworkClientMessageDistributer::SetPersonAuggmed(CNetworkMessage * pMsg)
{
    /*
    CNetworkPerson * pPerson = CMessageClientParser::SetPersonAuggmed(pMsg);
    if (nullptr == pPerson) return;
    */
    // A special case for listening to player's own addition!
    /*
    if (CPlayerCamera::GetPlayerId() == pPerson->GetId())
    {
        CPlayerCamera::NetworkAdd(pPerson->GetXPosition(), pPerson->GetYPosition(), pPerson->GetZPosition(), pPerson->GetYRotation());
    }
    else
    {
        m_pSimulationHandler->SetPerson(pPerson);
    }*/
}
void
CNetworkClientMessageDistributer::SetPersonAddedAuggmed(CNetworkMessage * pMsg)
{
    int iOriginalId = 0, iNewId = 0;
    std::tie(iOriginalId, iNewId) = CMessageClientParser::SetPersonAddedAuggmed(pMsg);
    if (-1 == iOriginalId || -1 == iNewId) return;
    //CPlayerCamera::PlayerAdded(iOriginalId, iNewId);
}
void
CNetworkClientMessageDistributer::SetTimeAuggmed(CNetworkMessage * pMsg)
{
    int iFrame = CMessageClientParser::SetTimeAuggmed(pMsg);
    if (-1 == iFrame) return;
    /*
    CPlaybackHandler * pPlayback = CPlaybackHandler::GetInstance();
    if (nullptr != pPlayback)
    {
        if (pPlayback->GetMaximumFrame() < iFrame) pPlayback->SetMaximumFrame(iFrame);
        pPlayback->SetFrame(iFrame);
    }*/
}
void
CNetworkClientMessageDistributer::SetDeleteAuggmed(CNetworkMessage * pMsg)
{
    /*
    CNetworkPersonDelete * pPerson = CMessageClientParser::SetPersonDeleteAuggmed(pMsg);
    if (nullptr == pPerson) return;
    // A special case for listening to player's own deletion! 
    if (CPlayerCamera::GetPlayerId() == pPerson->GetId())
    {
        CPlayerCamera::NetworkRemove();
    }
    else
    {
        m_pSimulationHandler->SetDelete(pPerson);
    }*/
}
void
CNetworkClientMessageDistributer::SetPersonMoveAuggmed(CNetworkMessage * pMsg)
{
    /*
    CNetworkPersonMove * pMove = CMessageClientParser::SetPersonMoveAuggmed(pMsg);
    if (nullptr == pMove) return;
    // A special case for listening to player's own movements!
    if (CPlayerCamera::GetPlayerId() == pMove->GetId())
    {
        CPlayerCamera::NetworkMove(pMove->GetX(), pMove->GetY(), pMove->GetZ(), pMove->GetXSpeed(), pMove->GetYSpeed(), pMove->GetZSpeed(), pMove->GetAction1(), pMove->GetStartFrame());
    }
    else
    {
        m_pSimulationHandler->SetPersonMove(pMove);
    }*/
}
void 
CNetworkClientMessageDistributer::SetFrame(CNetworkMessage * pMsg)
{
    /*
	int iFrame = CMessageParser::SetFrame(pMsg);
	if(-1 == iFrame) return;
	CPlaybackHandler * pPlayback = CPlaybackHandler::GetInstance();
	if(nullptr != pPlayback)
	{
		if(pPlayback->GetMaximumFrame() < iFrame) pPlayback->SetMaximumFrame(iFrame); // We make sure we can fit this frame in!
		pPlayback->SetFrame(iFrame);
	}*/
}
/*
void 
CNetworkClientMessageDistributer::SetPersonAction(CNetworkMessage * pMsg)
{
	if(pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON_ACTION) return;
	int iPersonId = 0, iFrame = 0, iLevel = 0, iAction1 = 0, iAction2 = 0, iHazard = 0;
	float fXPosition = 0.0f, fYPosition = 0.0f, fZPosition = 0.0f, fXSpeed = 0.0f, fYSpeed = 0.0f, fZSpeed = 0.0f, fYRotation = 0.0f;
	CMessageParser::SetPersonAction(pMsg, iPersonId, iFrame, fXPosition, fYPosition, fZPosition, iLevel, iAction1, iAction2, fXSpeed, fYSpeed, fZSpeed, fYRotation, iHazard);
	CPeople * pPeople = GetPeople();
	if(nullptr == pPeople) return;
	CPerson * pPerson = pPeople->GetPersonById(iPersonId);
	if(nullptr == pPerson) return; // We don't have a person!
	CPersonNode * pNode = new CPersonNode();
	pNode->SetTimeOut(iFrame);
	pNode->SetLevel(iLevel);
	pNode->SetAction1(iAction1);
	pNode->SetAction2(iAction2);
	pNode->SetHazard(iHazard);
	pNode->SetX(fXPosition);
	pNode->SetZ(fZPosition);
	pNode->SetAltitude(fYPosition);
	pNode->SetXSpeed(fXSpeed);
	pNode->SetYSpeed(fYSpeed);
	pNode->SetZSpeed(fZSpeed);
	pNode->SetRotation(fYRotation);
	pPerson->AddNode(pNode);
}*/
/*
void 
CNetworkClientMessageDistributer::SetPersonAction(CNetworkMessage * pMsg)
{
	if(pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON_ACTION) return;
	int iFrame = 0;
	CArrayList<CPersonUpdate> peopleUpdate;
	CMessageParser::SetPersonAction(pMsg, iFrame, &peopleUpdate);
	CPeople * pPeople = GetPeople();
	if(nullptr == pPeople) return;
	for(int iPerson = 0; iPerson < peopleUpdate.Size(); iPerson++)
	{
		CPersonUpdate * pUpdate = peopleUpdate.Get(iPerson);
		CPerson * pPerson = pPeople->GetPersonById(pUpdate->m_iId);
		if(nullptr == pPerson) continue; // We don't have a person!
		CPersonNode * pNode = new CPersonNode();
		if(pPerson->GetTotalNodes() > 0)
		{
			CPersonNode * pPrevious = pPerson->GetNode(pPerson->GetTotalNodes() - 1);
			// Lets fill the new node with previous information!
			pNode->Set(pPrevious);
			pPrevious->SetTimeNext(iFrame); // We now know when the node ends, which is now! :)
		}
		pNode->SetTimeOut(iFrame);
		for(int iAttrib = 0; iAttrib < pUpdate->m_attributes.Size(); iAttrib++)
		{
			CAttributeUpdate * pAttrib = pUpdate->m_attributes.Get(iAttrib);
			bool isFloat = pAttrib->IsFloat();
			bool isInt = pAttrib->IsInt();
			switch(pAttrib->m_cId)
			{
			case CAttributeUpdate::ACTION_1:
				{
					if(isFloat) pNode->SetAction1(pAttrib->GetFloat());
					if(isInt) pNode->SetAction1(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::ACTION_2:
				{
					if(isFloat) pNode->SetAction2(pAttrib->GetFloat());
					if(isInt) pNode->SetAction2(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::LEVEL_ID:
				{
					if(isFloat) pNode->SetLevel(pAttrib->GetFloat());
					if(isInt) pNode->SetLevel(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::HAZARD:
				{
					if(isFloat) pNode->SetHazard(pAttrib->GetFloat());
					if(isInt) pNode->SetHazard(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::X_LOCATION:
				{
					if(isFloat) pNode->SetX(pAttrib->GetFloat());
					if(isInt) pNode->SetX(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::Y_LOCATION:
				{
					if(isFloat) pNode->SetAltitude(pAttrib->GetFloat());
					if(isInt) pNode->SetAltitude(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::Z_LOCATION:
				{
					if(isFloat) pNode->SetZ(pAttrib->GetFloat());
					if(isInt) pNode->SetZ(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::X_SPEED:
				{
					if(isFloat) pNode->SetXSpeed(pAttrib->GetFloat());
					if(isInt) pNode->SetXSpeed(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::Y_SPEED:
				{
					if(isFloat) pNode->SetYSpeed(pAttrib->GetFloat());
					if(isInt) pNode->SetYSpeed(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::Z_SPEED:
				{
					if(isFloat) pNode->SetZSpeed(pAttrib->GetFloat());
					if(isInt) pNode->SetZSpeed(pAttrib->GetInt());
				} break;
			case CAttributeUpdate::Y_ROTATION:
				{
					if(isFloat) pNode->SetRotation(pAttrib->GetFloat());
					if(isInt) pNode->SetRotation(pAttrib->GetInt());
				} break;
			}
		}
		pPerson->AddNode(pNode);
	}
	peopleUpdate.DeleteAll();
}
*/