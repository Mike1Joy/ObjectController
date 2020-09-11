// Author/editor/coder: Simo
#include "MessageClientComposer.h"
#include "Endianness.h"
// Always use helper functions, they have endian fixed!

/*
CNetworkClientMessage * 
CMessageClientComposer::GetLevelCount()
{
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_GET_LEVEL_COUNT, 4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::RetLevelCount(int iLevelCount)
{
	int iLength = 8;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_RET_LEVEL_COUNT, iLength);
	pMsg->Set(iLevelCount, 4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::GetLevel(int iLevel)
{
	int iLength = 8;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_GET_LEVEL, iLength);
	pMsg->Set(iLevel, 4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::RetLevel(int iLevel, int iLevelHeight)
{
	int iLength = 12;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_RET_LEVEL, iLength);
	pMsg->Set(iLevel, 4);
	pMsg->Set(iLevelHeight, 8);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::GetWallCount(int iLevel)
{
	int iLength = 8;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_GET_WALL_COUNT, iLength);
	pMsg->Set(iLevel, 4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::RetWallCount(int iLevel, int iCount)
{
	int iLength = 12;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_RET_WALL_COUNT, iLength);
	pMsg->Set(iLevel, 4);
	pMsg->Set(iCount, 8);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::GetWall(int iLevel, int iWall)
{
	int iLength = 12;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_GET_WALL, iLength);
	pMsg->Set(iLevel, 4);
	pMsg->Set(iWall, 8);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::RetWall(int iLevel, int iWall, float fX, float fZ, float fLength, float fRotation, float fHeight)
{
	int iLength = 7 * 4 + 4; // type + 7 variables!
	int i = 0;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_RET_WALL, iLength);
	pMsg->Set(iLevel, i+=4);
	pMsg->Set(iWall, i+=4);
	pMsg->Set(fX, i+=4);
	pMsg->Set(fZ, i+=4);
	pMsg->Set(fLength, i+=4);
	pMsg->Set(fRotation, i+=4);
	pMsg->Set(fHeight, i+=4);
	return pMsg;
}*/

CNetworkMessage *
CMessageClientComposer::SetLevelHeight(int iStructureId, int iLevelId, float fLevelHeight)
{
	int iLength = 3 * 4 + 4;
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_LEVEL, iLength);
	int i = 0;
	pMsg->Set(iStructureId, i+=4);
	pMsg->Set(iLevelId, i+=4);
	pMsg->Set(fLevelHeight, i+=4);
	return pMsg;
}
CNetworkMessage *
CMessageClientComposer::SetWall(int iStructureId, int iLevelId, int iWallId, float fX, float fZ, float fLength, float fRotation, float fHeight)
{
	int iLength = 8 * 4 + 4; // type + 7 variables!
	int i = 0;
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_WALL, iLength);
	pMsg->Set(iStructureId, i+=4);
	pMsg->Set(iLevelId, i+=4);
	pMsg->Set(iWallId, i+=4);
	pMsg->Set(fX, i+=4);
	pMsg->Set(fZ, i+=4);
	pMsg->Set(fLength, i+=4);
	pMsg->Set(fRotation, i+=4);
	pMsg->Set(fHeight, i+=4);
	return pMsg;
}
CNetworkMessage *
CMessageClientComposer::SetModelVersion(int iModel, int iVersion)
{
	int iLength = 2 * 4 + 4; // type + 7 variables!
	int i = 0;
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_MODEL_VERSION, iLength);
	pMsg->Set(iModel, i+=4);
	pMsg->Set(iVersion, i+=4);
	return pMsg;
}
CNetworkMessage *
CMessageClientComposer::SetPerson(int iPersonId, int iType, int iAge, int iVestColour, int iEthnicity)
{
	int iLength = 5 * 4 + 4; // type + 7 variables!
	int i = 0;
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_PERSON, iLength);
	pMsg->Set(iPersonId, i+=4);
	pMsg->Set(iType, i+=4);
	pMsg->Set(iAge, i+=4);
	pMsg->Set(iVestColour, i+=4);
	pMsg->Set(iEthnicity, i+=4);
	return pMsg;
}
/*
CNetworkClientMessage * 
CMessageClientComposer::SetPersonAction(int iPersonId, int iFrame, float fXPosition, float fYPosition, float fZPosition, int iLevel, int iAction1, int iAction2, float fXSpeed, float fYSpeed, float fZSpeed, float fYRotation, int iHazard)
{
	int iLength = 13 * 4 + 4; // type + 7 variables!
	int i = 0;
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_SET_PERSON_ACTION, iLength);
	pMsg->Set(iPersonId, i+=4);
	pMsg->Set(iFrame, i+=4);
	pMsg->Set(fXPosition, i+=4);
	pMsg->Set(fYPosition, i+=4);
	pMsg->Set(fZPosition, i+=4);
	pMsg->Set(iLevel, i+=4);
	pMsg->Set(iAction1, i+=4);
	pMsg->Set(iAction2, i+=4);
	pMsg->Set(fXSpeed, i+=4);
	pMsg->Set(fYSpeed, i+=4);
	pMsg->Set(fZSpeed, i+=4);
	pMsg->Set(fYRotation, i+=4);
	pMsg->Set(iHazard, i+=4);
	return pMsg;
}*/
/*
CNetworkClientMessage * 
CMessageClientComposer::SetPersonAction(int iFrame, CArrayList<CPersonUpdate> * pUpdateList)
{
	int iUpdateLength = 0;
	for(int iPerson = 0; iPerson < pUpdateList->Size(); iPerson++)
	{
		CPersonUpdate * pUpdate = pUpdateList->Get(iPerson);
		pUpdate->GetAttributeData();
		iUpdateLength += pUpdate->m_iCachedLength;
	}
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_SET_PERSON_ACTION, iUpdateLength + 4 + 4 + 4); // id, length, frame
	int off = 0;
	pMsg->Set(iFrame, off+=4);
	pMsg->Set(iUpdateLength, off+=4);
	off += 4;
	for(int iPerson = 0; iPerson < pUpdateList->Size(); iPerson++)
	{
		CPersonUpdate * pUpdate = pUpdateList->Get(iPerson);
		pMsg->Set(pUpdate->GetAttributeData(), pUpdate->m_iCachedLength, off);
		off += pUpdate->m_iCachedLength;
	}
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::SetWall(CNetworkWall * pWall)
{
	if(nullptr == pWall) return nullptr;
	int iLength = 8 * 4 + 4; // 8 variables and type!
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_SET_WALL, iLength);
	int off = 0;
	pMsg->Set(pWall->GetId(), off+=4);
	pMsg->Set(pWall->GetStructureId(), off+=4);
	pMsg->Set(pWall->GetLevelId(), off+=4);
	pMsg->Set(pWall->GetX(), off+=4);
	pMsg->Set(pWall->GetZ(), off+=4);
	pMsg->Set(pWall->GetLength(), off+=4);
	pMsg->Set(pWall->GetRotation(), off+=4);
	pMsg->Set(pWall->GetHeight(), off+=4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::SetPerson(CNetworkPerson * pPerson)
{
	if(nullptr == pPerson) return nullptr;
	int iLength = 14 * 4 + 4; // 8 variables and type!
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_SET_PERSON, iLength);
	int off = 0;
	pMsg->Set(pPerson->GetId(), off+=4);
	pMsg->Set(pPerson->GetFrame(), off+=4);
	pMsg->Set(pPerson->GetStructureId(), off+=4);
	pMsg->Set(pPerson->GetLevelId(), off+=4);
	pMsg->Set(pPerson->GetXPosition(), off+=4);
	pMsg->Set(pPerson->GetYPosition(), off+=4);
	pMsg->Set(pPerson->GetZPosition(), off+=4);
	pMsg->Set(pPerson->GetXSpeed(), off+=4);
	pMsg->Set(pPerson->GetYSpeed(), off+=4);
	pMsg->Set(pPerson->GetZSpeed(), off+=4);
	pMsg->Set(pPerson->GetAction1(), off+=4);
	pMsg->Set(pPerson->GetAction2(), off+=4);
	pMsg->Set(pPerson->GetYRotation(), off+=4);
	pMsg->Set(pPerson->GetHazard(), off+=4);
	return pMsg;
}
CNetworkClientMessage * 
CMessageClientComposer::SetLevel(CNetworkLevel * pLevel)
{
	if(nullptr == pLevel) return nullptr;
	int iLength = 3 * 4 + 4; // 8 variables and type!
	CNetworkClientMessage * pMsg = new CNetworkClientMessage(CNetworkClientMessage::NWMSG_SET_LEVEL, iLength);
	int off = 0;
	pMsg->Set(pLevel->GetId(), off+=4);
	pMsg->Set(pLevel->GetStructureId(), off+=4);
	pMsg->Set(pLevel->GetHeight(), off+=4);
	return pMsg;
}
*/
CNetworkMessage *
CMessageClientComposer::SetFrame(int iFrame)
{
	int iLength = 4 + 4; // Type and variable!
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_FRAME, iLength);
	int off = 0;
	pMsg->Set(iFrame, off+=4);
	return pMsg;
}
/*
CNetworkClientMessage * 
CMessageClientComposer::SetReset()
{
	return new CNetworkClientMessage(CAuggmedMessageIds::EXMS_SIMULATION_RESET, 4); // Length is id length (32bits)
}
CNetworkClientMessage * 
CMessageClientComposer::SetPlay()
{
	return new CNetworkClientMessage(CAuggmedMessageIds::EXMS_SIMULATION_START, 4); // Length is id length (32bits)
}
CNetworkClientMessage * 
CMessageClientComposer::SetStop()
{
	return new CNetworkClientMessage(CAuggmedMessageIds::EXMS_SIMULATION_PAUSE, 4); // Length is id length (32bits)
}
*/
CNetworkMessage *
CMessageClientComposer::SetRewind()
{
	return new CNetworkMessage(CNetworkMessage::NWMSG_SET_REWIND, 4); // Length is id length (32bits)
}
CNetworkMessage *
CMessageClientComposer::SetPersonMovementTarget(int iPersonId, float fX, float fZ, int iStructure, int iLevel)
{
	int iLength = 5 * 4 + 4; // 5 variables + type
    CNetworkMessage * pMsg = new CNetworkMessage(CNetworkMessage::NWMSG_SET_PERSON_MOVEMENT_TARGET, iLength);
	int off = 0;
	pMsg->Set(iPersonId, off += 4);
	pMsg->Set(fX, off += 4);
	pMsg->Set(fZ, off += 4);
	pMsg->Set(iStructure, off += 4);
	pMsg->Set(iLevel, off += 4);
	return pMsg;
}
CNetworkMessage *
CMessageClientComposer::AddPersonAuggmed(int iPersonId, float fX, float fY, float fZ)
{
    /*
    int iLength = 4 * 4 + 4; // 5 variables + type
    CNetworkClientMessage * pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_ADD, iLength);
    int off = 0;
    pMsg->Set(iPersonId, off += 4);
    pMsg->SetLittle(fX, off += 4);
    pMsg->SetLittle(fY, off += 4);
    pMsg->SetLittle(fZ, off += 4);
    return pMsg;
    */
    return nullptr;
}
CNetworkMessage *
CMessageClientComposer::MovePersonAuggmed(int iPersonId, float fX, float fY, float fZ, float fXSpeed, float fYSpeed, float fZSpeed, float fTime)
{
    /*
    int iLength = 8 * 4 + 4; // 8 variables + type
    CNetworkClientMessage * pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_MOVE, iLength);
    int off = 0;
    pMsg->Set(iPersonId, off += 4);
    pMsg->SetLittle(fX, off += 4);
    pMsg->SetLittle(fY, off += 4);
    pMsg->SetLittle(fZ, off += 4);
    pMsg->SetLittle(fXSpeed, off += 4);
    pMsg->SetLittle(fYSpeed, off += 4);
    pMsg->SetLittle(fZSpeed, off += 4);
    pMsg->SetLittle(fTime, off += 4);
    return pMsg;
    */
    return nullptr;
}
CNetworkMessage *
CMessageClientComposer::RemovePersonAuggmed(int iPersonId)
{
    /*
    int iLength = 1 * 4 + 4; // 1 variables + type
    CNetworkClientMessage * pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_DELETE, iLength);
    int off = 0;
    pMsg->Set(iPersonId, off += 4);
    return pMsg;
    */
    return nullptr;
}
CNetworkMessage *
CMessageClientComposer::GestureAuggmed(int iPersonId, int iTypeOfGesture, int iGestureId, float fX, float fY, float fZ, float fXDirection, float fYDirection, float fStartTime, float fDuration, float fRadius, float fMinProbHandGesture, float fMaxProbHandGesture, float fMaxComplianceFactor, float fMinAgentDelay, float fMaxAgentDelay, int iApplyToAgentId)
{
    /*
    int iLength = 17 * 4 + 4; // 17 variables + type
    CNetworkClientMessage * pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_GESTURE, iLength);
    int off = 0;
    pMsg->Set(iPersonId, off += 4);
    pMsg->Set(iTypeOfGesture, off += 4);
    pMsg->Set(iGestureId, off += 4);
    pMsg->SetLittle(fX, off += 4);
    pMsg->SetLittle(fY, off += 4);
    // 5
    pMsg->SetLittle(fZ, off += 4);
    pMsg->SetLittle(fXDirection, off += 4);
    pMsg->SetLittle(fYDirection, off += 4);
    pMsg->SetLittle(fStartTime, off += 4);
    pMsg->SetLittle(fDuration, off += 4);
    // 10
    pMsg->SetLittle(fRadius, off += 4);
    pMsg->SetLittle(fMinProbHandGesture, off += 4);
    pMsg->SetLittle(fMaxProbHandGesture, off += 4);
    pMsg->SetLittle(fMaxComplianceFactor, off += 4);
    pMsg->SetLittle(fMinAgentDelay, off += 4);
    // 15
    pMsg->SetLittle(fMaxAgentDelay, off += 4);
    pMsg->Set(iApplyToAgentId, off += 4);
    // 17
    return pMsg;
    */
    return nullptr;
}
CNetworkMessage *
CMessageClientComposer::FacingAuggmed(int iPersonId, float fXDirection, float fYDirection, float fTime)
{
    /*
    int iLength = 4 * 4 + 4; // 4 variables + type
    CNetworkClientMessage * pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_PERSONFACE, iLength);
    int off = 0;
    pMsg->Set(iPersonId, off += 4);
    pMsg->Set(fXDirection, off += 4);
    pMsg->Set(fYDirection, off += 4);
    pMsg->Set(fTime, off += 4);
    return pMsg;
    */
    return nullptr;
}


CMessageClientComposer::CMessageClientComposer(void){}
CMessageClientComposer::~CMessageClientComposer(void){}
