// Author/editor/coder: Simo
#include "MessageServerParser.h"
#include "Endianness.h"
// Always use helper functions, they have endian fixed!

/*
bool 
CMessageParser::GetLevelCount(CNetworkMessage * pMsg)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_GET_LEVEL_COUNT) return false;
	return true;
}
bool 
CMessageParser::RetLevelCount(CNetworkMessage * pMsg, int &iLevelCount)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_RET_LEVEL_COUNT) return false;
	iLevelCount = pMsg->GetInt(4);
	return true;
}
bool 
CMessageParser::GetLevel(CNetworkMessage * pMsg, int &iLevel)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_GET_LEVEL) return false;
	iLevel = pMsg->GetInt(4);
	return true;
}
bool 
CMessageParser::RetLevel(CNetworkMessage * pMsg, int &iLevel, int &iLevelHeight)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_RET_LEVEL) return false;
	iLevel = pMsg->GetInt(4);
	iLevelHeight = pMsg->GetInt(8);
	return true;
}
bool 
CMessageParser::GetWallCount(CNetworkMessage * pMsg, int &iLevel)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_GET_WALL_COUNT) return false;
	iLevel = pMsg->GetInt(4);
	return true;
}
bool 
CMessageParser::RetWallCount(CNetworkMessage * pMsg, int &iLevel, int &iCount)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_RET_WALL_COUNT) return false;
	iLevel = pMsg->GetInt(4);
	iCount = pMsg->GetInt(8);
	return true;
}
bool 
CMessageParser::GetWall(CNetworkMessage * pMsg, int &iLevel, int &iWall)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_GET_WALL) return false;
	iLevel = pMsg->GetInt(4);
	iWall = pMsg->GetInt(8);
	return true;
}
bool 
CMessageParser::RetWall(CNetworkMessage * pMsg, int &iLevel, int &iWall, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_RET_WALL) return false;
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
CMessageServerParser::SetLevelHeight(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, float &fLevelHeight)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_LEVEL) return false;
	int i = 0;
	iStructureId = pMsg->GetInt(i+=4);
	iLevelId = pMsg->GetInt(i+=4);
	fLevelHeight = pMsg->GetFloat(i+=4);
	return true;
}
bool 
CMessageServerParser::SetWall(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, int &iWallId, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_WALL) return false;
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
CMessageServerParser::SetPerson(CNetworkMessage * pMsg, int &iPersonId, int &iType, int &iAge, int &iVestColour, int &iEthnicity)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON) return false;
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
CMessageServerParser::SetPersonAction(CNetworkServerMessage * pMsg, int &iPersonId, int &iFrame, float &fXPosition, float &fYPosition, float &fZPosition, int &iLevel, int &iAction1, int &iAction2, float &fXSpeed, float &fYSpeed, float &fZSpeed, float &fYRotation, int &iHazard)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkServerMessage::NWMSG_SET_PERSON_ACTION) return false;
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
CMessageServerParser::SetPersonAction(CNetworkServerMessage * pMsg, int &iFrame, CArrayList<CPersonUpdate> * pUpdateList)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkServerMessage::NWMSG_SET_PERSON_ACTION) return false;
	int offset = 0;
	iFrame = pMsg->GetInt(offset+=4);
	int iCount = pMsg->GetInt(offset+=4);
	for(int iPerson = 0; iPerson < iCount; iPerson++)
	{
		CPersonUpdate * pUpdate = CPersonUpdate::SetAttributeData(pMsg, offset);
		if(NULL != pUpdate) pUpdateList->Add(pUpdate);
		else return false;
	}
	return true;
}
CNetworkPerson *
CMessageServerParser::SetPerson(CNetworkServerMessage * pMsg)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkServerMessage::NWMSG_SET_PERSON) return NULL;
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
CNetworkWall *
CMessageServerParser::SetWall(CNetworkServerMessage * pMsg)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkServerMessage::NWMSG_SET_WALL) return NULL;
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
CMessageServerParser::SetLevel(CNetworkServerMessage * pMsg)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkServerMessage::NWMSG_SET_LEVEL) return NULL;
	CNetworkLevel * pObj = new CNetworkLevel();
	int off = 0;
	pObj->SetId(pMsg->GetInt(off+=4));
	pObj->SetStructureId(pMsg->GetInt(off+=4));
	pObj->SetHeight(pMsg->GetFloat(off+=4));
	return pObj;
}
*/
int 
CMessageServerParser::SetFrame(CNetworkMessage * pMsg)
{
	if(NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_FRAME) return -1;
	int off = 0;
	int iFrame = pMsg->GetInt(off+=4);
	return iFrame;
}
bool
CMessageServerParser::SetPersonMovementTarget(CNetworkMessage * pMsg, int &iPersonId, float &fX, float &fZ, int &iStructure, int &iLevel)
{
	if (NULL == pMsg || pMsg->GetType() != CNetworkMessage::NWMSG_SET_PERSON_MOVEMENT_TARGET) return false;
	int off = 0;
	iPersonId = pMsg->GetInt(off += 4);
	fX = pMsg->GetFloat(off += 4);
	fZ = pMsg->GetFloat(off += 4);
	iStructure = pMsg->GetInt(off += 4);
	iLevel = pMsg->GetInt(off += 4);
	return true;
}

CMessageServerParser::CMessageServerParser(void){}
CMessageServerParser::~CMessageServerParser(void){}
