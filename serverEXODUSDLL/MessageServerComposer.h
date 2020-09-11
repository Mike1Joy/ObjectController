// Author/editor/coder: Simo
#pragma once
#include "ArrayList.h"
#include "NetworkMessage.h"
class CMessageServerComposer
{
public:
	/*
	static CNetworkServerMessage * GetLevelCount();
	static CNetworkServerMessage * RetLevelCount(int iLevelCount);
	static CNetworkServerMessage * GetLevel(int iLevel);
	static CNetworkServerMessage * RetLevel(int iLevel, int iLevelHeight);
	static CNetworkServerMessage * GetWallCount(int iLevel);
	static CNetworkServerMessage * RetWallCount(int iLevel, int iCount);
	static CNetworkServerMessage * GetWall(int iLevel, int iWall);
	static CNetworkServerMessage * RetWall(int iLevel, int iWall, float fX, float fZ, float fLength, float fRotation, float fHeight);
	*/
	static CNetworkMessage * SetLevelHeight(int iStructureId, int iLevelId, float fLevelHeight);
	static CNetworkMessage * SetWall(int iStructureId, int iLevelId, int iWallId, float fX, float fZ, float fLength, float fRotation, float fHeight);
	static CNetworkMessage * SetModelVersion(int iModel, int iVersion);
	static CNetworkMessage * SetPerson(int iPersonId, int iType, int iAge, int iVestColour, int iEthnicity);
	//static CNetworkServerMessage * SetPersonAction(int iPersonId, int iFrame, float fXPosition, float fYPosition, float fZPosition, int iLevel, int iAction1, int iAction2, float fXSpeed, float fYSpeed, float fZSpeed, float fYRotation, int iHazard);
    /*
	static CNetworkServerMessage * SetPersonAction(int iFrame, CArrayList<CPersonUpdate> * pUpdateList);
	static CNetworkServerMessage * SetWall(CNetworkWall * pWall);
	static CNetworkServerMessage * SetPerson(CNetworkPerson * pPerson);
	static CNetworkServerMessage * SetLevel(CNetworkLevel * pLevel);
    */
	static CNetworkMessage * SetFrame(int iFrame);
	static CNetworkMessage * SetReset();
	static CNetworkMessage * SetPlay();
	static CNetworkMessage * SetStop();
	static CNetworkMessage * SetRewind();
	static CNetworkMessage * SetPersonMovementTarget(int iPersonId, float fX, float fZ, int iStructure, int iLevel);

private:
	CMessageServerComposer(void);
	~CMessageServerComposer(void);
};
