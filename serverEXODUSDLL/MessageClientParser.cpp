// Author/editor/coder: Simo
#include "MessageClientParser.h"
#include "Endianness.h"
// Always use helper functions, they have endian fixed!

/*
bool 
CMessageClientParser::GetLevelCount(CNetworkClientMessage * pMsg)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_GET_LEVEL_COUNT) return false;
	return true;
}
bool 
CMessageClientParser::RetLevelCount(CNetworkClientMessage * pMsg, int &iLevelCount)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_RET_LEVEL_COUNT) return false;
	iLevelCount = pMsg->GetInt(4);
	return true;
}
bool 
CMessageClientParser::GetLevel(CNetworkClientMessage * pMsg, int &iLevel)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_GET_LEVEL) return false;
	iLevel = pMsg->GetInt(4);
	return true;
}
bool 
CMessageClientParser::RetLevel(CNetworkClientMessage * pMsg, int &iLevel, int &iLevelHeight)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_RET_LEVEL) return false;
	iLevel = pMsg->GetInt(4);
	iLevelHeight = pMsg->GetInt(8);
	return true;
}
bool 
CMessageClientParser::GetWallCount(CNetworkClientMessage * pMsg, int &iLevel)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_GET_WALL_COUNT) return false;
	iLevel = pMsg->GetInt(4);
	return true;
}
bool 
CMessageClientParser::RetWallCount(CNetworkClientMessage * pMsg, int &iLevel, int &iCount)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_RET_WALL_COUNT) return false;
	iLevel = pMsg->GetInt(4);
	iCount = pMsg->GetInt(8);
	return true;
}
bool 
CMessageClientParser::GetWall(CNetworkClientMessage * pMsg, int &iLevel, int &iWall)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_GET_WALL) return false;
	iLevel = pMsg->GetInt(4);
	iWall = pMsg->GetInt(8);
	return true;
}
bool 
CMessageClientParser::RetWall(CNetworkClientMessage * pMsg, int &iLevel, int &iWall, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_RET_WALL) return false;
	int i = 0;
	iLevel = pMsg->GetInt(i+=4);
	iWall = pMsg->GetInt(i+=4);
	fX = pMsg->GetInt(i+=4);
	fZ = pMsg->GetInt(i+=4);
	fLength = pMsg->GetInt(i+=4);
	fRotation = pMsg->GetInt(i+=4);
	fHeight = pMsg->GetInt(i+=4);
	return true;
}
*/
bool 
CMessageClientParser::SetLevelHeight(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, float &fLevelHeight)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_LEVEL) return false;
	int i = 0;
	iStructureId = pMsg->GetInt(i+=4);
	iLevelId = pMsg->GetInt(i+=4);
	fLevelHeight = pMsg->GetFloat(i+=4);
	return true;
}
bool 
CMessageClientParser::SetWall(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, int &iWallId, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_WALL) return false;
	int i = 0;
	iStructureId = pMsg->GetInt(i+=4);
	iLevelId = pMsg->GetInt(i+=4);
	iWallId = pMsg->GetInt(i+=4);
	fX = pMsg->GetFloat(i+=4);
	fZ = pMsg->GetFloat(i+=4);
	fLength = pMsg->GetFloat(i+=4);
	fRotation = pMsg->GetFloat(i+=4);
	fHeight = pMsg->GetFloat(i+=4);
	return true;
}
bool 
CMessageClientParser::SetPerson(CNetworkMessage * pMsg, int &iPersonId, int &iType, int &iAge, int &iVestColour, int &iEthnicity)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON) return false;
	int i = 0;
	iPersonId = pMsg->GetInt(i+=4);
	iType = pMsg->GetInt(i+=4);
	iAge = pMsg->GetInt(i+=4);
	iVestColour = pMsg->GetInt(i+=4);
	iEthnicity = pMsg->GetInt(i+=4);
	return true;
}
/*
bool 
CMessageClientParser::SetPersonAction(CNetworkClientMessage * pMsg, int &iPersonId, int &iFrame, float &fXPosition, float &fYPosition, float &fZPosition, int &iLevel, int &iAction1, int &iAction2, float &fXSpeed, float &fYSpeed, float &fZSpeed, float &fYRotation, int &iHazard)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON_ACTION) return false;
	int i = 0;
	iPersonId = pMsg->GetInt(i+=4);
	iFrame = pMsg->GetInt(i+=4);
	fXPosition = pMsg->GetFloat(i+=4);
	fYPosition = pMsg->GetFloat(i+=4);
	fZPosition = pMsg->GetFloat(i+=4);
	iLevel = pMsg->GetInt(i+=4);
	iAction1 = pMsg->GetInt(i+=4);
	iAction2 = pMsg->GetInt(i+=4);
	fXSpeed = pMsg->GetFloat(i+=4);
	fYSpeed = pMsg->GetFloat(i+=4);
	fZSpeed = pMsg->GetFloat(i+=4);
	fYRotation = pMsg->GetFloat(i+=4);
	iHazard = pMsg->GetInt(i+=4);
	return true;
}*/
/*
bool 
CMessageClientParser::SetPersonAction(CNetworkClientMessage * pMsg, int &iFrame, CArrayList<CPersonUpdate> * pUpdateList)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_SET_PERSON_ACTION) return false;
	int offset = 0;
	iFrame = pMsg->GetInt(offset+=4);
	int iCount = pMsg->GetInt(offset+=4);
	for(int iPerson = 0; iPerson < iCount; iPerson++)
	{
		CPersonUpdate * pUpdate = CPersonUpdate::SetAttributeData(pMsg, offset);
		if(nullptr != pUpdate) pUpdateList->Add(pUpdate);
		else return false;
	}
	return true;
}
CNetworkPerson *
CMessageClientParser::SetPerson(CNetworkClientMessage * pMsg)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_SET_PERSON) return nullptr;
	CNetworkPerson * pObj = new CNetworkPerson();
	int off = 0;
	pObj->SetId(pMsg->GetInt(off+=4));
	pObj->SetFrame(pMsg->GetInt(off+=4));
	pObj->SetStructureId(pMsg->GetInt(off+=4));
	pObj->SetLevelId(pMsg->GetInt(off+=4));
	pObj->SetXPosition(pMsg->GetFloat(off+=4));
	pObj->SetYPosition(pMsg->GetFloat(off+=4));
	pObj->SetZPosition(pMsg->GetFloat(off+=4));
	pObj->SetXSpeed(pMsg->GetFloat(off+=4));
	pObj->SetYSpeed(pMsg->GetFloat(off+=4));
	pObj->SetZSpeed(pMsg->GetFloat(off+=4));
	pObj->SetAction1(pMsg->GetInt(off+=4));
	pObj->SetAction2(pMsg->GetInt(off+=4));
	pObj->SetYRotation(pMsg->GetFloat(off+=4));
	pObj->SetHazard(pMsg->GetInt(off+=4));
	return pObj;
}
CNetworkPerson *
CMessageClientParser::SetPersonAuggmed(CNetworkClientMessage * pMsg)
{
    if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_ADD) return nullptr;
    CNetworkPerson * pObj = new CNetworkPerson();
    int off = 0;
    pObj->SetId(pMsg->GetInt(off += 4));
    pObj->SetXPosition(pMsg->GetFloatLittle(off += 4));
    pObj->SetYPosition(pMsg->GetFloatLittle(off += 4));
    pObj->SetZPosition(pMsg->GetFloatLittle(off += 4));
    pObj->SetYRotation(pMsg->GetFloatLittle(off += 4));

    // Data we don't get, so we guess:
    pObj->SetFrame(0);
    pObj->SetStructureId(0);
    pObj->SetLevelId(0);
    pObj->SetXSpeed(0.0f);
    pObj->SetYSpeed(0.0f);
    pObj->SetZSpeed(0.0f);
    pObj->SetAction1(CPersonNode::A1_NORMAL_MOVE);
    pObj->SetAction2(CPersonNode::A2_FACE_NEXT);
    pObj->SetHazard(0); // ??
    return pObj;
}
CNetworkPersonDelete *
CMessageClientParser::SetPersonDeleteAuggmed(CNetworkClientMessage * pMsg)
{
    if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_DELETE) return nullptr;
    CNetworkPersonDelete * pObj = new CNetworkPersonDelete();
    int off = 0;
    pObj->SetId(pMsg->GetInt(off += 4));
    return pObj;
}
CNetworkPersonMove *
CMessageClientParser::SetPersonMoveAuggmed(CNetworkClientMessage * pMsg)
{
    if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_MOVE) return nullptr;
    CNetworkPersonMove * pObj = new CNetworkPersonMove();
    int off = 0;
    pObj->SetId(pMsg->GetInt(off += 4));
    pObj->SetX(pMsg->GetFloatLittle(off += 4));
    pObj->SetY(pMsg->GetFloatLittle(off += 4));
    pObj->SetZ(pMsg->GetFloatLittle(off += 4));
    pObj->SetXSpeed(pMsg->GetFloatLittle(off += 4));
    pObj->SetYSpeed(pMsg->GetFloatLittle(off += 4));
    pObj->SetZSpeed(pMsg->GetFloatLittle(off += 4));
    pObj->SetStartFrame(CPlaybackHandler::GetFrames(pMsg->GetFloatLittle(off += 4))); // Convert seconds to frames!
    pObj->SetAction1(pMsg->GetInt(off += 4));
    return pObj;
}
*/
int
CMessageClientParser::SetTimeAuggmed(CNetworkMessage * pMsg)
{
    //if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_TIME) return -1;
    int off = 0;
    float fTime = pMsg->GetFloatLittle(off += 4);
    return 0;// CPlaybackHandler::GetFrames(fTime); // We return frame index
}
/*
CNetworkPersonAdded *
CMessageParser::SetPersonAddedAuggmed(CNetworkMessage * pMsg)
{
    if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_ADDED) return nullptr;
    CNetworkPersonAdded * pObj = new CNetworkPersonAdded();
    int off = 0;
    pObj->SetOriginalId(pMsg->GetInt(off += 4));
    pObj->SetNewId(pMsg->GetInt(off += 4));
    return pObj;
}*/
std::tuple<int, int>
CMessageClientParser::SetPersonAddedAuggmed(CNetworkMessage * pMsg)
{
    //if (nullptr == pMsg || pMsg->GetType() != CAuggmedMessageIds::EXMS_ADDED) return std::make_tuple(-1, -1);
    int off = 0;
    int iOriginalId = pMsg->GetInt(off += 4);
    int iNewId = pMsg->GetInt(off += 4);
    return std::make_tuple(iOriginalId, iNewId);
}
/*
CNetworkWall *
CMessageClientParser::SetWall(CNetworkClientMessage * pMsg)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_SET_WALL) return nullptr;
	CNetworkWall * pObj = new CNetworkWall();
	int off = 0;
	pObj->SetId(pMsg->GetInt(off+=4));
	pObj->SetStructureId(pMsg->GetInt(off+=4));
	pObj->SetLevelId(pMsg->GetInt(off+=4));
	pObj->SetX(pMsg->GetFloat(off+=4));
	pObj->SetZ(pMsg->GetFloat(off+=4));
	pObj->SetLength(pMsg->GetFloat(off+=4));
	pObj->SetRotation(pMsg->GetFloat(off+=4));
	pObj->SetHeight(pMsg->GetFloat(off+=4));
	return pObj;
}
CNetworkLevel *
CMessageClientParser::SetLevel(CNetworkClientMessage * pMsg)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkClientMessage::NWMSG_SET_LEVEL) return nullptr;
	CNetworkLevel * pObj = new CNetworkLevel();
	int off = 0;
	pObj->SetId(pMsg->GetInt(off+=4));
	pObj->SetStructureId(pMsg->GetInt(off+=4));
	pObj->SetHeight(pMsg->GetFloat(off+=4));
	return pObj;
}*/
int 
CMessageClientParser::SetFrame(CNetworkMessage * pMsg)
{
	if(nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_FRAME) return -1;
	int off = 0;
	int iFrame = pMsg->GetInt(off+=4);
	return iFrame;
}
bool
CMessageClientParser::SetPersonMovementTarget(CNetworkMessage * pMsg, int &iPersonId, float &fX, float &fZ, int &iStructure, int &iLevel)
{
	if (nullptr == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON_MOVEMENT_TARGET) return false;
	int off = 0;
	iPersonId = pMsg->GetInt(off += 4);
	fX = pMsg->GetFloat(off += 4);
	fZ = pMsg->GetFloat(off += 4);
	iStructure = pMsg->GetInt(off += 4);
	iLevel = pMsg->GetInt(off += 4);
	return true;
}

CMessageClientParser::CMessageClientParser(void){}
CMessageClientParser::~CMessageClientParser(void){}
