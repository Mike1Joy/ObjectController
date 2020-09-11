// Author/editor/coder: Simo
#pragma once
#ifndef NULL
#define NULL 0
#endif
#include "ArrayList.h"
#include "NetworkMessage.h"
#include "Defines.h"
class EXODUS_SERVER_DLL_API CMessageServerParser
{
public:
	/*
	static bool GetLevelCount(CNetworkMessage * pMsg);
	static bool RetLevelCount(CNetworkMessage * pMsg, int &iLevelCount);
	static bool GetLevel(CNetworkMessage * pMsg, int &iLevel);
	static bool RetLevel(CNetworkMessage * pMsg, int &iLevel, int &iLevelHeight);
	static bool GetWallCount(CNetworkMessage * pMsg, int &iLevel);
	static bool RetWallCount(CNetworkMessage * pMsg, int &iLevel, int &iCount);
	static bool GetWall(CNetworkMessage * pMsg, int &iLevel, int &iWall);
	static bool RetWall(CNetworkMessage * pMsg, int &iLevel, int &iWall, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight);
	*/
	static bool SetLevelHeight(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, float &fLevelHeight);
	static bool SetWall(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, int &iWallId, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight);
	static bool SetPerson(CNetworkMessage * pMsg, int &iPersonId, int &iType, int &iAge, int &iVestColour, int &iEthnicity);
	//static bool SetPersonAction(CNetworkMessage * pMsg, int &iPersonId, int &iFrame, float &fXPosition, float &fYPosition, float &fZPosition, int &iLevel, int &iAction1, int &iAction2, float &fXSpeed, float &fYSpeed, float &fZSpeed, float &fYRotation, int &iHazard);
    /*
	static bool SetPersonAction(CNetworkServerMessage * pMsg, int &iFrame, CArrayList<CPersonUpdate> * pUpdateList);
	static CNetworkPerson * SetPerson(CNetworkServerMessage * pMsg);
	static CNetworkWall * SetWall(CNetworkServerMessage * pMsg);
	static CNetworkLevel * SetLevel(CNetworkServerMessage * pMsg);
    */

	static int SetFrame(CNetworkMessage * pMsg);
	static bool SetPersonMovementTarget(CNetworkMessage * pMsg, int &iPersonId, float &fX, float &fZ, int &iStructure, int &iLevel);

	// Auxiliary class with static only functions doesn't need to be constructed
private:
	CMessageServerParser(void);
	~CMessageServerParser(void);
};
