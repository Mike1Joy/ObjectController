// Author/editor/coder: Simo
#pragma once
#include "ArrayList.h"
#include "NetworkMessage.h"
class CMessageClientComposer
{
public:
	/*
	static CNetworkClientMessage * GetLevelCount();
	static CNetworkClientMessage * RetLevelCount(int iLevelCount);
	static CNetworkClientMessage * GetLevel(int iLevel);
	static CNetworkClientMessage * RetLevel(int iLevel, int iLevelHeight);
	static CNetworkClientMessage * GetWallCount(int iLevel);
	static CNetworkClientMessage * RetWallCount(int iLevel, int iCount);
	static CNetworkClientMessage * GetWall(int iLevel, int iWall);
	static CNetworkClientMessage * RetWall(int iLevel, int iWall, float fX, float fZ, float fLength, float fRotation, float fHeight);
	*/
	static CNetworkMessage * SetLevelHeight(int iStructureId, int iLevelId, float fLevelHeight);
	static CNetworkMessage * SetWall(int iStructureId, int iLevelId, int iWallId, float fX, float fZ, float fLength, float fRotation, float fHeight);
	static CNetworkMessage * SetModelVersion(int iModel, int iVersion);
	static CNetworkMessage * SetPerson(int iPersonId, int iType, int iAge, int iVestColour, int iEthnicity);
	//static CNetworkClientMessage * SetPersonAction(int iPersonId, int iFrame, float fXPosition, float fYPosition, float fZPosition, int iLevel, int iAction1, int iAction2, float fXSpeed, float fYSpeed, float fZSpeed, float fYRotation, int iHazard);
    /*
	static CNetworkClientMessage * SetPersonAction(int iFrame, CArrayList<CPersonUpdate> * pUpdateList);
	static CNetworkClientMessage * SetWall(CNetworkWall * pWall);
	static CNetworkClientMessage * SetPerson(CNetworkPerson * pPerson);
	static CNetworkClientMessage * SetLevel(CNetworkLevel * pLevel);
    */
	static CNetworkMessage * SetFrame(int iFrame);
	static CNetworkMessage * SetReset();
	static CNetworkMessage * SetPlay();
	static CNetworkMessage * SetStop();
	static CNetworkMessage * SetRewind();
	static CNetworkMessage * SetPersonMovementTarget(int iPersonId, float fX, float fZ, int iStructure, int iLevel);
    static CNetworkMessage * AddPersonAuggmed(int iPersonId, float fX, float fY, float fZ);
    static CNetworkMessage * MovePersonAuggmed(int iPersonId, float fX, float fY, float fZ, float fXSpeed, float fYSpeed, float fZSpeed, float fTime);
    static CNetworkMessage * RemovePersonAuggmed(int iPersonId);
    static CNetworkMessage * GestureAuggmed(int iPersonId, int iTypeOfGesture, int iGestureId, float fX, float fY, float fZ, float fXDirection, float fYDirection, float fStartTime, float fDuration, float fRadius, float fMinProbHandGesture, float fMaxProbHandGesture, float fMaxComplianceFactor, float fMinAgentDelay, float fMaxAgentDelay, int iApplyToAgentId);
    static CNetworkMessage * FacingAuggmed(int iPersonId, float fXDirection, float fYDirection, float fTime);
private:
	CMessageClientComposer(void);
	~CMessageClientComposer(void);
};
